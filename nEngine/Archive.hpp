/**
	@file Archive.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include "File.hpp"
#include "Resource.hpp"

namespace nEngine {
    class Archive : public Resource {
    public:
        Archive(const std::string& id);
        ~Archive();

        void read(const std::string& fileName);
        void write(const std::string& fileName);

        std::vector<std::string> getFileList();
        void readFile(File* file, const std::string& fileName);
        void writeFile(File* file, const std::string& fileName);
    public:
        static const ResourceType sType = RESOURCE_ARCHIVE;

    private:
        unzFile mArchive;
        int mNumFiles;
        std::map<std::string, int> mFiles;
    };
};

#endif /*ARCHIVE_HPP*/
