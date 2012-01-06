/**
	@file File.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef FILE_HPP
#define FILE_HPP

#include "Error.hpp"
#include "Resource.hpp"
#include "Resources.hpp"

namespace nEngine {
    class File : public Resource {
    public:
        File(const std::string& id);
        ~File();

        void setData(uint8* data, unsigned size)
        {
            mData = data;
            mSize = size;
        }

        uint8* getData()
        {
            return mData;
        }

        unsigned getSize ()
        {
            return mSize;
        }

		int getMemoryUsage()
		{
			return mSize + sizeof(*this);
		}

    private:
        void loadFromFile(const std::string& fileName);
        void loadFromArchive(const std::string& archive, const std::string& fileName);


        uint8* mData;
        unsigned mSize;

        std::string mArchiveName, mFileName;
        bool mFromArchive, mOkay;

    public:
        static const ResourceType sType = RESOURCE_FILE;
    };
};

#endif /*FILE_HPP*/
