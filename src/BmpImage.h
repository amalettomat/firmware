/*
 * BmpImage.h
 *
 *  Created on: 06.02.2019
 *      Author: zwax
 */

#ifndef BMPIMAGE_H_
#define BMPIMAGE_H_

struct BmpImage {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
	const char*	     pixel_data;
};

#endif /* BMPIMAGE_H_ */
