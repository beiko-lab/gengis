//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#ifndef _GENGIS_IMAGE_IO_
#define _GENGIS_IMAGE_IO_

#include "../core/Precompiled.hpp"

// ~mikep: This was giving erros on ubuntu. Have yet to look into why.
// Added the #define PNG_SKIP_SETJMP_CHECK to fix it.

#define PNG_SKIP_SETJMP_CHECK
#include <png.h>

namespace GenGIS
{
	/**
	* @brief Singleton class for holding project specific settings and widely access system level objects.
	*/
	class ImageIO
	{
	public:
		/**
		* @brief Write image data to file as PNG
		* @param filename Name of output file.
		* @param img Image data.
		* @param width Width of image.
		* @param height Height of image.
		*/
		static void WritePNG(const std::string &filename, const byte *img, const int width, const int height)
		{
			FILE *fp;
			png_structp png_ptr;
			png_infop info_ptr;

			/* open the file */
			fp = fopen(filename.c_str(), "wb");
			if (fp == NULL) {
				throw std::string("Cannot create file \"") + filename + std::string("\"");
			}

			/* Create and initialize the png_struct with the desired error handler
			* functions.  If you want to use the default stderr and longjump method,
			* you can supply NULL for the last three parameters.  We also check that
			* the library version is compatible with the one used at compile time,
			* in case we are using dynamically linked libraries.  REQUIRED.
			*/
			png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
				NULL, NULL, NULL);

			if (png_ptr == NULL) {
				fclose(fp);
				throw std::string("Cannot create PNG write structure for file \"") + filename + std::string("\"");
			}

			/* Allocate/initialize the image information data.  REQUIRED */
			info_ptr = png_create_info_struct(png_ptr);
			if (info_ptr == NULL) {
				fclose(fp);
				png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
				throw std::string("Out of memory in RGBA::PNG::Write()");
			}

			/* Set error handling.  REQUIRED if you aren't supplying your own
			* error handling functions in the png_create_write_struct() call.
			*/
			if (setjmp(png_jmpbuf(png_ptr)))
			{
				/* If we get here, we had a problem reading the file */
				fclose(fp);
				png_destroy_write_struct(&png_ptr, &info_ptr);
				throw std::string("Could not set up error handling for PNG");
			}

			/* set up the output control if you are using standard C streams */
			png_init_io(png_ptr, fp);

			/* This is the easy way.  Use it if you already have all the
			* image info living info in the structure.  You could "|" many
			* PNG_TRANSFORM flags into the png_transforms integer here.
			*/
			//   png_write_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL);


			/* This is the hard way */

			/* Set the image information here.  Width and height are up to 2^31,
			* bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
			* the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
			* PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
			* or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
			* PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
			* currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
			*/
			png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

			/* Write the file header information.  REQUIRED */
			png_write_info(png_ptr, info_ptr);

			/* If you want, you can write the info in two steps, in case you need to
			* write your private chunk ahead of PLTE:
		    *
		    *   png_write_info_before_PLTE(write_ptr, write_info_ptr);
		    *   write_my_chunk();
		    *   png_write_info(png_ptr, info_ptr);
		    *
		    * However, given the level of known- and unknown-chunk support in 1.1.0
		    * and up, this should no longer be necessary.
		    */

			/* Once we write out the header, the compression type on the text
			* chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
			* PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
			* at the end.
			*/

			/* set up the transformations you want.  Note that these are
			* all optional.  Only call them if you want them.
			*/

			/* The easiest way to write the image (you may have a different memory
			* layout, however, so choose what fits your needs best).  You need to
			* use the first method if you aren't handling interlacing yourself.
			*/
			png_bytep *row_pointers = new png_bytep[height];

			if( (png_uint_32)height > (png_uint_32)(PNG_UINT_32_MAX/png_sizeof(png_bytep)) ) {
				fclose(fp);
				png_destroy_write_struct(&png_ptr, &info_ptr);
				throw std::string("Image is too tall to process in memory");
			}

			for(int k = 0; k < height; k++)
				row_pointers[k] = (byte*)img + k*width*4;

			/* write out the entire image data in one call */
			png_write_image(png_ptr, row_pointers);

			/* You can write optional chunks like tEXt, zTXt, and tIME at the end
			* as well.  Shouldn't be necessary in 1.1.0 and up as all the public
			* chunks are supported and you can use png_set_unknown_chunks() to
			* register unknown chunks into the info structure to be written out.
			*/

			/* It is REQUIRED to call this to finish writing the rest of the file */
			png_write_end(png_ptr, info_ptr);

			/* clean up after the write, and free any memory allocated */
			png_destroy_write_struct(&png_ptr, &info_ptr);

			delete[] row_pointers;
			row_pointers = NULL;

			/* close the file */
			fclose(fp);
		}
	};

}

#endif
