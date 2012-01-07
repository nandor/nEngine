/**
	@file Image.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Image.hpp"

#define PNGSIGSIZE 8

namespace nEngine {


	// ------------------------------------------------------------------
	void pngReadData(png_structp png_ptr, png_bytep outBytes, png_size_t bytesToRead)
	{

		Image* image = (Image*)png_get_io_ptr(png_ptr);

		if (image == NULL) {
			return;
		}

		unsigned off = image->getFileOffset();
		memcpy(outBytes, image->getFile()->getData() + off, bytesToRead);	
	
		image->setFileOffset(off + bytesToRead);
		return;
	}


	// ------------------------------------------------------------------
	Image::Image(const std::string& fileID)
		:Resource(fileID, RESOURCE_IMAGE)
	{	
		mFileOffset = PNGSIGSIZE;
		mFile =  Resources::inst().require<File> (fileID);

		if (mFile == NULL || mFile->getData() == NULL) {
			throw Error ("Image", fileID, "Cannot open image");
		}


		if (png_sig_cmp((png_bytep)mFile->getData(), 0, PNGSIGSIZE)) {
			throw Error ("Image", fileID, "Image is not a png");
		}

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
		if (png_ptr == NULL) {
			throw Error ("Image", fileID, "[libpng] Init error");
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);

		if (info_ptr == NULL) {
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			throw Error ("Image", fileID, "[libpng] Init error");
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			throw Error ("Image", fileID, "[libpng] IO or Memory error");
		}
	
		png_set_read_fn(png_ptr, this, pngReadData);

		png_set_sig_bytes(png_ptr, PNGSIGSIZE);
		png_read_info(png_ptr, info_ptr);
	
		mWidth = png_get_image_width(png_ptr, info_ptr);
		mHeight = png_get_image_height(png_ptr, info_ptr);

		int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		int channels = png_get_channels(png_ptr, info_ptr);
		unsigned bpp =  bit_depth * channels >> 3;
		
		GLuint imageType;
		switch (png_get_color_type(png_ptr, info_ptr)) {
			case PNG_COLOR_TYPE_RGBA:
				imageType = GL_RGBA;
				break;
			case PNG_COLOR_TYPE_RGB:
				imageType = GL_RGB;
				break;
			case PNG_COLOR_TYPE_GRAY:
				imageType = GL_LUMINANCE;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				imageType = GL_LUMINANCE_ALPHA;
				break;
			default:
				png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
				throw Error ("Image", fileID, "Invalid image type, only grayscale and rgb is supported");
				break;
		}

		png_bytep* rowPtr = new png_bytep[mHeight];
		png_bytep data = new png_byte[mWidth * mHeight * bpp];

		for (size_t i = 0; i < (size_t)mHeight; ++i) { 
			rowPtr[i] = (png_bytep)data + i * mWidth * bpp;
		}
	
		png_read_image(png_ptr, rowPtr);


		glGenTextures(1, &mTexID);
		glBindTexture(GL_TEXTURE_2D, mTexID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, mWidth, mHeight, imageType, GL_UNSIGNED_BYTE, data);

		delete[] data;
		delete[] rowPtr;

		return;
	}


	// ------------------------------------------------------------------
	Image::~Image()
	{

	}

	// -----------------------------------------------------------------
	void Image::bind(int tex)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + tex); 
		glBindTexture(GL_TEXTURE_2D, getTextureID());
	}
};