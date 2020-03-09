/*
 * BmpFile.h
 *
 *  Created on: 07.02.2019
 *      Author: zwax
 */

#ifndef BMPFILE_H_
#define BMPFILE_H_

#include <inttypes.h>
#include <SD.h>


class BmpFile {
public:
	static const int NUM_LINES_IN_BUFFER = 1;
	static const int IMG_WIDTH = 480;

#pragma pack(1)
	struct bmp_file_header_t {
	  uint16_t signature;
	  uint32_t file_size;
	  uint16_t reserved[2];
	  uint32_t image_offset;
	};

	struct bmp_image_header_t {
	  uint32_t header_size;
	  uint32_t image_width;
	  uint32_t image_height;
	  uint16_t color_planes;
	  uint16_t bits_per_pixel;
	  uint32_t compression_method;
	  uint32_t image_size;
	  uint32_t horizontal_resolution;
	  uint32_t vertical_resolution;
	  uint32_t colors_in_palette;
	  uint32_t important_colors;
	};
#pragma pack()

	BmpFile();
	virtual ~BmpFile();

	bool open(const char* path);
	uint16_t* readLine();

private:
	File m_imgFile;
	struct bmp_file_header_t m_fileHeader;
	struct bmp_image_header_t m_imageHeader;
	uint16_t m_imgData[IMG_WIDTH * NUM_LINES_IN_BUFFER];
};

#endif /* BMPFILE_H_ */
