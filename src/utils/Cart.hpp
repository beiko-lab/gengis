/* Header file for cartogram.c
 *
 * Written by Mark Newman
 *
 * See http://www.umich.edu/~mejn/ for further details.
 */

#ifndef _GENGIS_CART_H
#define _GENGIS_CART_H

class GenGIS::Cart;

namespace GenGIS
{
	class Cart
	{
		public:
			explicit Cart();
			~Cart();

			double** cart_dmalloc(int xsize, int ysize);
			void cart_dfree(double **userrho);
			void cart_makews(int xsize, int ysize);
			void cart_freews(int xsize, int ysize);
			void cart_transform(double **userrho, int xsize, int ysize);
			void cart_makecart(double *pointx, double *pointy, int npoints,
					   int xsize, int ysize, double blur);
	};
}
#endif
