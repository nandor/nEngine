/**
	@file Image.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Image.hpp"
#include "Util.hpp"
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
		loadFromPNG(fileID);
	}
	
	// ------------------------------------------------------------------
	Image::Image(const std::string& fileID, char** xpmData)
		:Resource(fileID, RESOURCE_IMAGE)
	{
		loadFromXPM(xpmData);
	}

	// ------------------------------------------------------------------
	void Image::loadFromPNG(const std::string& fileID) 
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
				throw Error ("Image", fileID, "Invalid image type, only grayscale and RGB is supported");
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
	void Image::loadFromXPM(char** xpm)
	{
		if (xpm == NULL || xpm[0] == NULL) {
			throw Error("Invalid XPM data!");
		}

		int ncolors, cpp;
		int x_hotspot = 0, y_hotspot = 0;
		bool hasXPMEXT = false;

		std::stringstream valuesStream(xpm[0]);
		valuesStream >> mWidth >> mHeight >> ncolors >> cpp;

		if (!valuesStream.eof()) {
			valuesStream >> x_hotspot >> y_hotspot;
		}

		if (!valuesStream.eof()) {
			std::string tmp;
			valuesStream >> tmp;
			if (tmp == "XPMEXT") {
				hasXPMEXT = true;
			}
		}

		struct color {
			GLubyte r, g, b, a;
		};

		std::map<std::string, color> colors;
		for (int i = 1; i <= ncolors; ++i) {
			std::stringstream lineStream(xpm[i]);
			std::string id = "", type, colorString;

			for (int j = 0; j < cpp; ++j) {
				id.push_back(lineStream.get());
			}

			while (!lineStream.eof()) {
				lineStream >> type >> colorString;
				if (type == "c") {
					color newColor;
					memset(&newColor, 0, sizeof(newColor));
					if (boost::algorithm::to_lower_copy(colorString) == "none") {
						newColor.a = 0xFF;
						continue;	
					}

					if (colorString[0] != '#') {
						xpmError("Only RGB colors are supported!", i, cpp + 2);
					}

					colorString = colorString.substr(1, colorString.length());
					boost::algorithm::to_lower(colorString);

					switch (colorString.length()) {
					case 6:
						// #XXYYZZ
						newColor.r = ((charToInt(colorString[0]) * 16) + charToInt(colorString[1]));
						newColor.g = ((charToInt(colorString[2]) * 16) + charToInt(colorString[3]));
						newColor.b = ((charToInt(colorString[4]) * 16) + charToInt(colorString[5]));
						newColor.a = 255;
						break;
					case 8:
						// #XXYYZZAA
						newColor.r = ((charToInt(colorString[0]) << 4) + charToInt(colorString[1]));
						newColor.g = ((charToInt(colorString[2]) << 4) + charToInt(colorString[3]));
						newColor.b = ((charToInt(colorString[4]) << 4) + charToInt(colorString[5]));
						newColor.a = ((charToInt(colorString[6]) << 4) + charToInt(colorString[7]));
						break;
					default:
						xpmError("Invalid color: '#" + colorString + "'", i, cpp + 2);
						break;
					}
					colors.insert(std::make_pair(id, newColor));
					break;
				}
			}
		}

		int pp = sizeof(color);

		GLubyte* imgData = new GLubyte[mWidth * mHeight * sizeof(color)];
		memset(imgData, 0, mWidth * mHeight * sizeof(color));
		GLubyte* ptr = imgData;

		for (int i = ncolors + 1; i < ncolors + mHeight + 1; ++i) {
			for (int j = 0; j < mWidth * cpp; j += cpp) {
				std::string id = "";
				for (int k = 0; k < cpp; ++k) {
					id.push_back(xpm[i][j + k]);
				}

				color c = colors[id];
				memcpy(ptr, &c, sizeof(color));
				if (id == "V ") {
					int a = 5;
				}
				ptr += 4;
			}
		}

		glGenTextures(1, &mTexID);
		glBindTexture(GL_TEXTURE_2D, mTexID);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		delete[] imgData;
	}
	
	
	// ------------------------------------------------------------------
	void Image::draw()
	{
		glBindTexture(GL_TEXTURE_2D, mTexID);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(mWidth, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(mWidth, mHeight);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, mHeight);
		glEnd();
	}

	// ------------------------------------------------------------------
	void Image::xpmError(const std::string& str, int line, int chr)
	{
		throw Error("Image", getID(), "Error reading xpm data: " + str + " at " + 
									  boost::lexical_cast<std::string> (line) + ":" +
									  boost::lexical_cast<std::string> (chr));
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