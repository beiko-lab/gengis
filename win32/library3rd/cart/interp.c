/* Example program to interpolate bilinearly between the points of the
 * grid output by cart
 *
 * Written by Mark Newman
 *
 * See http://www.umich.edu/~mejn/ for further details.
 */


#include <stdio.h>
#include <stdlib.h>


/* Function to read in the grid points.  Uses column-major, so the loops
 * are an odd way around.  Returns 0 if successful; 1 if the file ended
 * early */

int readpoints(FILE *stream, double **gridx, double **gridy,
	       int xsize, int ysize)
{
  int ix,iy;

  for (iy=0; iy<=ysize; iy++) {
    for (ix=0; ix<=xsize; ix++) {
      if (fscanf(stream,"%lf %lf\n",&gridx[ix][iy],&gridy[ix][iy])<2) {
	return 1;
      }
    }
  }

  return 0;
}


#define LINELENGTH 1000

main(int argc, char *argv[])
{
  int i;
  int xsize,ysize;
  int ix,iy;
  double xin,yin;
  double xout,yout;
  double dx,dy;
  double **gridx,**gridy;
  char line[LINELENGTH];
  FILE *infp;

  /* Read the command-line parameters */

  if (argc<4) {
    fprintf(stderr,"Usage: %s xsize ysize gridfile\n",argv[0]);
    exit(1);
  }
  xsize = atoi(argv[1]);
  if (xsize<1) {
    fprintf(stderr,"%s: xsize botched\n",argv[0]);
    exit(2);
  }
  ysize = atoi(argv[2]);
  if (ysize<1) {
    fprintf(stderr,"%s: ysize botched\n",argv[0]);
    exit(3);
  }
  infp = fopen(argv[3],"r");
  if (infp==NULL) {
    fprintf(stderr,"%s: unable to open file `%s'\n",argv[0],argv[3]);
    exit(4);
  }

  /* Read in the grid of points */

  gridx = malloc((xsize+1)*sizeof(double*));
  for (i=0; i<=xsize; i++) gridx[i] = malloc((ysize+1)*sizeof(double));
  gridy = malloc((xsize+1)*sizeof(double*));
  for (i=0; i<=xsize; i++) gridy[i] = malloc((ysize+1)*sizeof(double));
  if (readpoints(infp,gridx,gridy,xsize,ysize)!=0) {
    fprintf(stderr,"%s: input file `%s' ended early\n",argv[0],argv[3]);
    exit(5);
  }

  /* Close the input file */

  fclose(infp);

  /* Repeatedly read a point from stdin, transform it, and write it out
   * again.  Points outside the grid space are written out untransformed.
   * Lines that don't contain valid coordinates, including blank lines,
   * are written out unchanged.  Terminates when CTRL-D is read from input.
   */

  while (fgets(line,LINELENGTH,stdin)!=NULL) {

    if (sscanf(line,"%lf %lf",&xin,&yin)!=2) {
      printf("%s",line);
      continue;
    }

    /* Check if we are outside bounds */

    if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
      printf("%g %g\n",xin,yin);
    } else {
      ix = xin;
      dx = xin - ix;
      iy = yin;
      dy = yin - iy;
      xout = (1-dx)*(1-dy)*gridx[ix][iy] + dx*(1-dy)*gridx[ix+1][iy]
             + (1-dx)*dy*gridx[ix][iy+1] + dx*dy*gridx[ix+1][iy+1];
      yout = (1-dx)*(1-dy)*gridy[ix][iy] + dx*(1-dy)*gridy[ix+1][iy]
             + (1-dx)*dy*gridy[ix][iy+1] + dx*dy*gridy[ix+1][iy+1];
      printf("%g %g\n",xout,yout);
    }
  }
}
