/**
	@file Image.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "File.hpp"
#include "Resource.hpp"

namespace nEngine {
    class Image : public Resource {
    public:
        Image(const std::string& fileID);
        ~Image();

        int getWidth()
		{
			return mWidth;
		}

        int getHeight()
		{
			return mHeight;
		}

        unsigned getTextureID()
		{
			return mTexID;
		}

		File* getFile() {
			return mFile;
		}

		void setFileOffset(unsigned off)
		{
			mFileOffset = off;
		}

		unsigned getFileOffset() 
		{
			return mFileOffset;
		}
    public:
        static const ResourceType sType = RESOURCE_IMAGE;

    protected:
        int mWidth;
        int mHeight;
        unsigned mTexID;
		
		File* mFile;
		unsigned mFileOffset;
    };
};


#endif /*IMAGE_HPP*/
