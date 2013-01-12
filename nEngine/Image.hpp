/**
	@file Image.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "types.hpp"
#include "Color.hpp"
#include "File.hpp"
#include "Resource.hpp"

namespace nEngine {
    class Image : public Resource {
    public:
		/**
			Load the image data from a file
			@param fileID			ID of the file
		*/
        NAPI Image(const std::string& fileID);

		/**
			Load the image data from an XPM array
			@param id				ID of the image
			@param xpmData			XPM Image data
		*/
		NAPI Image(const std::string& id, char** xpmData);

		/**
			Destructor
		*/
        NAPI ~Image();

		/**
			Bind the texture to the tex position, 0 <= text <= 7
		*/
		NAPI void bind(int tex = 0);
		
		/**
			Draw the image
		*/
		NAPI void draw();

		/**
			Return the width of the image
			@return					Width
		*/
        NAPI int getWidth()
		{
			return mWidth;
		}
		

		/**
			Return the height of the image
			@return					Height
		*/
        NAPI int getHeight()
		{
			return mHeight;
		}

		/**
			Return the OpenGL texture id
			@return					ID of the OpenGL texture
		*/
        NAPI unsigned getTextureID()
		{
			return mTexID;
		}

		/**
			Return the file used to get the data (PNG)
			@return					Pointer to the file or NULL
		*/
		NAPI File* getFile() 
		{
			return mFile;
		}
		

		/**
			Set the offset (PNG)
			@param off				Offset
		*/
		NAPI void setFileOffset(unsigned off)
		{
			mFileOffset = off;
		}
		

		/**
			Return the offset (PNG)
			@return					fileOffset
		*/
		NAPI unsigned getFileOffset() 
		{
			return mFileOffset;
		}
	
	public:

        static const ResourceType sType = RESOURCE_IMAGE;

    protected:
		void loadFromPNG(const std::string& fileName);
		void loadFromXPM(char** xpmData);
		void xpmError(const std::string& msg, int line, int col);

        int mWidth;
        int mHeight;
        unsigned mTexID;
		
		File* mFile;
		unsigned mFileOffset;
    };
};


#endif /*IMAGE_HPP*/
