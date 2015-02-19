/* Routines to transform a given set of points to a Gastner-Newman cartogram
 *
 * Written by Mark Newman
 *
 * See http://www.umich.edu/~mejn/ for further details.
 */

//using namespace GenGIS;
/* Inclusions */
//#include "../core/Precompiled.hpp"
//#include "../utils/cart.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
//#include "fftw3.h"

/* Constants */

#define INITH 0.001          // Initial size of a time-step
#define TARGETERROR 0.01     // Desired accuracy per step in pixels
#define MAXRATIO 4.0         // Max ratio to increase step size by
#define EXPECTEDTIME 1.0e8   // Guess as to the time it will take, used to
                             // estimate completion

#define PI 3.1415926535897932384626

/* Globals */

double *rhot[5];       // Pop density at time t (five snaps needed)
double *fftrho;        // FT of initial density
double *fftexpt;       // FT of density at time t

double **vxt[5];       // x-velocity at time t
double **vyt[5];       // y-velocity at time t

double *expky;         // Array needed for the Gaussian convolution

fftw_plan rhotplan[5]; // Plan for rho(t) back-transform at time t


/* Function to make space for the density array.  This is done in such a
 * way as to allow rho to be accessed either as a single block (for FFTW)
 * or as a normal double-indexed array (for regular use) */

double** cart_dmalloc(int xsize, int ysize)
{
  int ix;
  double **userrho;

  userrho = malloc(xsize*sizeof(double*));
  *userrho = fftw_malloc(xsize*ysize*sizeof(double));
  for (ix=1; ix<xsize; ix++) userrho[ix] = *userrho + ix*ysize;

  return userrho;
}


/* Function to free space for the density array */

void cart_dfree(double **userrho)
{
  fftw_free(*userrho);
  free(userrho);
}


/* Function to allocate space for the global arrays */

void cart_makews(int xsize, int ysize)
{
  int s,i;

  /* Space for the FFT arrays is allocated single blocks, rather than using
   * a true two-dimensional array, because libfftw demands that it be so */

  for (s=0; s<5; s++) rhot[s] = fftw_malloc(xsize*ysize*sizeof(double));
  fftrho = fftw_malloc(xsize*ysize*sizeof(double));
  fftexpt = fftw_malloc(xsize*ysize*sizeof(double));

  for (s=0; s<5; s++) {
    vxt[s] = malloc((xsize+1)*sizeof(double*));
    for (i=0; i<=xsize; i++) vxt[s][i] = malloc((ysize+1)*sizeof(double));
  }
  for (s=0; s<5; s++) {
    vyt[s] = malloc((xsize+1)*sizeof(double*));
    for (i=0; i<=xsize; i++) vyt[s][i] = malloc((ysize+1)*sizeof(double));
  }

  expky = malloc(ysize*sizeof(double));

  /* Make plans for the back transforms */

  for (i=0; i<5; i++) {
    rhotplan[i] = fftw_plan_r2r_2d(xsize,ysize,fftexpt,rhot[i],
				   FFTW_REDFT01,FFTW_REDFT01,FFTW_MEASURE);
  }
}


/* Function to free up space for the global arrays and destroy the FFT
 * plans */

void cart_freews(int xsize, int ysize)
{
  int s,i;

  for (s=0; s<5; s++) fftw_free(rhot[s]);
  fftw_free(fftrho);
  fftw_free(fftexpt);

  for (s=0; s<5; s++) {
    for (i=0; i<=xsize; i++) free(vxt[s][i]);
    free(vxt[s]);
  }
  for (s=0; s<5; s++) {
    for (i=0; i<=xsize; i++) free(vyt[s][i]);
    free(vyt[s]);
  }

  free(expky);

  for (i=0; i<5; i++) fftw_destroy_plan(rhotplan[i]);
}


/* Function to calculate the discrete cosine transform of the input data.
 * assumes its input is an fftw_malloced array in column-major form with
 * size xsize*ysize */

void cart_forward(double *rho, int xsize, int ysize)
{
  fftw_plan plan;

  plan = fftw_plan_r2r_2d(xsize,ysize,rho,fftrho,
			  FFTW_REDFT10,FFTW_REDFT10,FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);
}


/* Function to calculate the discrete cosine transform of the input data.
 * This function is just a wrapper for forward(), so the user doesn't
 * need to see the fftw-format density array */

void cart_transform(double **userrho, int xsize, int ysize)
{
  cart_forward(*userrho,xsize,ysize);
}


/* Function to calculate the population density at arbitrary time by back-
 * transforming and put the result in a particular rhot[] snapshot array.
 * Calculates unnormalized densities, since FFTW gives unnormalized back-
 * transforms, but this doesn't matter because the cartogram method is
 * insensitive to variation in the density by a multiplicative constant */

void cart_density(double t, int s, int xsize, int ysize)
{
  int ix,iy;
  double kx,ky;
  double expkx;

  /* Calculate the expky array, to save time in the next part */

  for (iy=0; iy<ysize; iy++) {
    ky = PI*iy/ysize;
    expky[iy] = exp(-ky*ky*t);
  }

  /* Multiply the FT of the density by the appropriate factors */

  for (ix=0; ix<xsize; ix++) {
    kx = PI*ix/xsize;
    expkx = exp(-kx*kx*t);
    for (iy=0; iy<ysize; iy++) {
      fftexpt[ix*ysize+iy] = expkx*expky[iy]*fftrho[ix*ysize+iy];
    }
  }

  /* Perform the back-transform */

  fftw_execute(rhotplan[s]);
}


/* Function to calculate the velocity at all integer grid points for a
 * specified snapshot */

void cart_vgrid(int s, int xsize, int ysize)
{
  int ix,iy;
  double r00,r10;
  double r01,r11;
  double mid;

  /* Do the corners */

  vxt[s][0][0] = vyt[s][0][0] = 0.0;
  vxt[s][xsize][0] = vyt[s][xsize][0] = 0.0;
  vxt[s][0][ysize] = vyt[s][0][ysize] = 0.0;
  vxt[s][xsize][ysize] = vyt[s][xsize][ysize] = 0.0;

  /* Do the top border */

  r11 = rhot[s][0];
  for (ix=1; ix<xsize; ix++) {
    r01 = r11;
    r11 = rhot[s][ix*ysize];
    vxt[s][ix][0] = -2*(r11-r01)/(r11+r01);
    vyt[s][ix][0] = 0.0;
  }

  /* Do the bottom border */

  r10 = rhot[s][ysize-1];
  for (ix=1; ix<xsize; ix++) {
    r00 = r10;
    r10 = rhot[s][ix*ysize+ysize-1];
    vxt[s][ix][ysize] = -2*(r10-r00)/(r10+r00);
    vyt[s][ix][ysize] = 0.0;
  }

  /* Left edge */

  r11 = rhot[s][0];
  for (iy=1; iy<ysize; iy++) {
    r10 = r11;
    r11 = rhot[s][iy];
    vxt[s][0][iy] = 0.0;
    vyt[s][0][iy] = -2*(r11-r10)/(r11+r10);
  }

  /* Right edge */

  r01 = rhot[s][(xsize-1)*ysize];
  for (iy=1; iy<ysize; iy++) {
    r00 = r01;
    r01 = rhot[s][(xsize-1)*ysize+iy];
    vxt[s][xsize][iy] = 0.0;
    vyt[s][xsize][iy] = -2*(r01-r00)/(r01+r00);
  }

  /* Now do all the points in the middle */

  for (ix=1; ix<xsize; ix++) {
    r01 = rhot[s][(ix-1)*ysize];
    r11 = rhot[s][ix*ysize];
    for (iy=1; iy<ysize; iy++) {
      r00 = r01;
      r10 = r11;
      r01 = rhot[s][(ix-1)*ysize+iy];
      r11 = rhot[s][ix*ysize+iy];
      mid = r10 + r00 + r11 + r01;
      vxt[s][ix][iy] = -2*(r10-r00+r11-r01)/mid;
      vyt[s][ix][iy] = -2*(r01-r00+r11-r10)/mid;
    }
  }
}



/* Function to calculate the velocity at an arbitrary point from the grid
 * velocities for a specified snapshot by interpolating between grid
 * points.  If the requested point is outside the boundaries, we
 * extrapolate (ensures smooth flow back in if we get outside by mistake,
 * although we should never actually do this because function cart_twosteps()
 * contains code to prevent it) */

void cart_velocity(double rx, double ry, int s, int xsize, int ysize,
		   double *vxp, double *vyp)
{
  int ix,iy;
  double dx,dy;
  double dx1m,dy1m;
  double w11,w21,w12,w22;

  /* Deal with the boundary conditions */

  ix = rx;
  if (ix<0) ix = 0;
  else if (ix>=xsize) ix = xsize - 1;

  iy = ry;
  if (iy<0) iy = 0;
  else if (iy>=ysize) iy = ysize - 1;

  /* Calculate the weights for the bilinear interpolation */

  dx = rx - ix;
  dy = ry - iy;

  dx1m = 1.0 - dx;
  dy1m = 1.0 - dy;

  w11 = dx1m*dy1m;
  w21 = dx*dy1m;
  w12 = dx1m*dy;
  w22 = dx*dy;

  /* Perform the interpolation for x and y components of velocity */

  *vxp = w11*vxt[s][ix][iy] + w21*vxt[s][ix+1][iy] +
         w12*vxt[s][ix][iy+1] + w22*vxt[s][ix+1][iy+1];
  *vyp = w11*vyt[s][ix][iy] + w21*vyt[s][ix+1][iy] +
         w12*vyt[s][ix][iy+1] + w22*vyt[s][ix+1][iy+1];
}


/* Function to integrate 2h time into the future two different ways using
 * four-order Runge-Kutta and compare the differences for the purposes of
 * the adaptive step size.  Parameters are:
 *   *pointx = array of x-coords of points
 *   *pointy = array of y-coords of points
 *   npoints = number of points
 *   t = current time, i.e., start time of these two steps
 *   h = delta t
 *   s = snapshot index of the initial time
 *   xsize, ysize = size of grid
 *   *errorp = the maximum integration error found for any polygon vertex for
 *             the complete two-step process
 *   *drp = maximum distance moved by any point
 *   *spp = the snapshot index for the final function evaluation
 */

void cart_twosteps(double *pointx, double *pointy, int npoints,
		   double t, double h, int s, int xsize, int ysize,
		   double *errorp, double *drp, int *spp)
{
  int s0,s1,s2,s3,s4;
  int p;
  double rx1,ry1;
  double rx2,ry2;
  double rx3,ry3;
  double v1x,v1y;
  double v2x,v2y;
  double v3x,v3y;
  double v4x,v4y;
  double k1x,k1y;
  double k2x,k2y;
  double k3x,k3y;
  double k4x,k4y;
  double dx1,dy1;
  double dx2,dy2;
  double dx12,dy12;
  double dxtotal,dytotal;
  double ex,ey;
  double esq,esqmax;
  double drsq,drsqmax;

  s0 = s;
  s1 = (s+1)%5;
  s2 = (s+2)%5;
  s3 = (s+3)%5;
  s4 = (s+4)%5;

  /* Calculate the density field for the four new time slices */

  cart_density(t+0.5*h,s1,xsize,ysize);
  cart_density(t+1.0*h,s2,xsize,ysize);
  cart_density(t+1.5*h,s3,xsize,ysize);
  cart_density(t+2.0*h,s4,xsize,ysize);

  /* Calculate the resulting velocity grids */

  cart_vgrid(s1,xsize,ysize);
  cart_vgrid(s2,xsize,ysize);
  cart_vgrid(s3,xsize,ysize);
  cart_vgrid(s4,xsize,ysize);

  /* Do all three RK steps for each point in turn */

  esqmax = drsqmax = 0.0;

  for (p=0; p<npoints; p++) {

    rx1 = pointx[p];
    ry1 = pointy[p];

    /* Do the big combined (2h) RK step */

    cart_velocity(rx1,ry1,s0,xsize,ysize,&v1x,&v1y);
    k1x = 2*h*v1x;
    k1y = 2*h*v1y;
    cart_velocity(rx1+0.5*k1x,ry1+0.5*k1y,s2,xsize,ysize,&v2x,&v2y);
    k2x = 2*h*v2x;
    k2y = 2*h*v2y;
    cart_velocity(rx1+0.5*k2x,ry1+0.5*k2y,s2,xsize,ysize,&v3x,&v3y);
    k3x = 2*h*v3x;
    k3y = 2*h*v3y;
    cart_velocity(rx1+k3x,ry1+k3y,s4,xsize,ysize,&v4x,&v4y);
    k4x = 2*h*v4x;
    k4y = 2*h*v4y;

    dx12 = (k1x+k4x+2.0*(k2x+k3x))/6.0;
    dy12 = (k1y+k4y+2.0*(k2y+k3y))/6.0;

    /* Do the first small RK step.  No initial call to cart_velocity() is done
     * because it would be the same as the one above, so there's no need
     * to do it again */

    k1x = h*v1x;
    k1y = h*v1y;
    cart_velocity(rx1+0.5*k1x,ry1+0.5*k1y,s1,xsize,ysize,&v2x,&v2y);
    k2x = h*v2x;
    k2y = h*v2y;
    cart_velocity(rx1+0.5*k2x,ry1+0.5*k2y,s1,xsize,ysize,&v3x,&v3y);
    k3x = h*v3x;
    k3y = h*v3y;
    cart_velocity(rx1+k3x,ry1+k3y,s2,xsize,ysize,&v4x,&v4y);
    k4x = h*v4x;
    k4y = h*v4y;

    dx1 = (k1x+k4x+2.0*(k2x+k3x))/6.0;
    dy1 = (k1y+k4y+2.0*(k2y+k3y))/6.0;

    /* Do the second small RK step */

    rx2 = rx1 + dx1;
    ry2 = ry1 + dy1;

    cart_velocity(rx2,ry2,s2,xsize,ysize,&v1x,&v1y);
    k1x = h*v1x;
    k1y = h*v1y;
    cart_velocity(rx2+0.5*k1x,ry2+0.5*k1y,s3,xsize,ysize,&v2x,&v2y);
    k2x = h*v2x;
    k2y = h*v2y;
    cart_velocity(rx2+0.5*k2x,ry2+0.5*k2y,s3,xsize,ysize,&v3x,&v3y);
    k3x = h*v3x;
    k3y = h*v3y;
    cart_velocity(rx2+k3x,ry2+k3y,s4,xsize,ysize,&v4x,&v4y);
    k4x = h*v4x;
    k4y = h*v4y;

    dx2 = (k1x+k4x+2.0*(k2x+k3x))/6.0;
    dy2 = (k1y+k4y+2.0*(k2y+k3y))/6.0;

    /* Calculate the (squared) error */

    ex = (dx1+dx2-dx12)/15;
    ey = (dy1+dy2-dy12)/15;
    esq = ex*ex + ey*ey;
    if (esq>esqmax) esqmax = esq;

    /* Update the position of the vertex using the more accurate (two small
     * steps) result, and deal with the boundary conditions.  This code
     * does 5th-order "local extrapolation" (which just means taking
     * the estimate of the 5th-order term above and adding it to our
     * 4th-order result get a result accurate to the next highest order) */

    dxtotal = dx1 + dx2 + ex;   // Last term is local extrapolation
    dytotal = dy1 + dy2 + ey;   // Last term is local extrapolation
    drsq = dxtotal*dxtotal + dytotal*dytotal;
    if (drsq>drsqmax) drsqmax = drsq;

    rx3 = rx1 + dxtotal;
    ry3 = ry1 + dytotal;

    if (rx3<0) rx3 = 0;
    else if (rx3>xsize) rx3 = xsize;
    if (ry3<0) ry3 = 0;
    else if (ry3>ysize) ry3 = ysize;

    pointx[p] = rx3;
    pointy[p] = ry3;

  }

  *errorp = sqrt(esqmax);
  *drp =  sqrt(drsqmax);
  *spp = s4;
}


/* Function to estimate the percentage completion */

int cart_complete(double t)
{
  int res;

  res = 100*log(t/INITH)/log(EXPECTEDTIME/INITH);
  if (res>100) res = 100;

  return res;
}


/* Function to do the transformation of the given set of points
 * to the cartogram */

void cart_makecart(double *pointx, double *pointy, int npoints,
		   int xsize, int ysize, double blur)
{
  int i;
  int s,sp;
  int step;
  int done;
  double t,h;
  double error,dr;
  double desiredratio;

  /* Calculate the initial density and velocity for snapshot zero */

  cart_density(0.0,0,xsize,ysize);
  cart_vgrid(0,xsize,ysize);
  s = 0;

  /* Now integrate the points in the polygons */

  step = 0;
  t = 0.5*blur*blur;
  h = INITH;

  do {

    /* Do a combined (triple) integration step */

    cart_twosteps(pointx,pointy,npoints,t,h,s,xsize,ysize,&error,&dr,&sp);

    /* Increase the time by 2h and rotate snapshots */

    t += 2.0*h;
    step += 2;
    s = sp;

    /* Adjust the time-step.  Factor of 2 arises because the target for
     * the two-step process is twice the target for an individual step */

    desiredratio = pow(2*TARGETERROR/error,0.2);
    if (desiredratio>MAXRATIO) h *= MAXRATIO;
    else h *= desiredratio;

    done = cart_complete(t);
#ifdef PERCENT
    fprintf(stdout,"%i\n",done);
#endif
#ifndef NOPROGRESS
    fprintf(stderr,"  %3i%%  |",done);
    for (i=0; i<done/2; i++) fprintf(stderr,"=");
    for (i=done/2; i<50; i++) fprintf(stderr," ");
    fprintf(stderr,"|\r");
#endif

    /* If no point moved then we are finished */

  } while (dr>0.0);

#ifdef PERCENT
  fprintf(stdout,"\n");
#endif
#ifndef NOPROGRESS
  fprintf(stderr,"  100%%  |==================================================|\n");
#endif
}
