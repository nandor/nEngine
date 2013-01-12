/**
	@file Archive.cpp
	@author Licker Nando
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Archive.hpp"

namespace nEngine {
	std::regex dirRegex("[a-zA-Z0-9_/]+/");

	// ------------------------------------------------------------------
	Archive::Archive(const std::string& id)
		:Resource(id, RESOURCE_ARCHIVE)
	{
		int error = 0;
		if (!(mArchive = unzOpen(id.c_str()))) {
			throw Error ("Archive", id, "Cannot open archive");
		}

		if (unzGoToFirstFile (mArchive) != UNZ_OK) {
			throw Error ("Archive", id, "Cannot read archive");
		}

		mNumFiles = 0;

		char buffer[1024];
		do {
			
			mNumFiles++;
			unz_file_info info;
			unzGetCurrentFileInfo(mArchive, &info, buffer, sizeof(buffer), NULL, NULL, NULL, NULL);
			std::string fileName(buffer);
			mFiles.insert(make_pair(fileName, info.uncompressed_size));

		} while (unzGoToNextFile(mArchive) != UNZ_END_OF_LIST_OF_FILE);
	}


	// ------------------------------------------------------------------
	Archive::~Archive()
	{
		unzClose(mArchive);
	}


	// ------------------------------------------------------------------
	void Archive::readFile(File* file, const std::string& fileName)
	{
		if (unzLocateFile(mArchive, fileName.c_str(), true) == UNZ_END_OF_LIST_OF_FILE) {
			throw Error ("Archive", getID(), "Cannot find file '" + fileName + "'");
		}

		if (unzOpenCurrentFile(mArchive) != UNZ_OK) {
			throw Error ("Archive", getID(), "Cannot open file '" + fileName + "'");
		}

		unz_file_info info;
		if (unzGetCurrentFileInfo(mArchive, &info, NULL, NULL, NULL, NULL, NULL, NULL)) {
			unzCloseCurrentFile(mArchive);
			throw Error ("Archive", getID(), "Cannot get file descriptor for'" + fileName + "'");
		}
	
		uint8* buf = new uint8[info.uncompressed_size + 1];
		if (unzReadCurrentFile(mArchive, buf, info.uncompressed_size) < 0) {
			unzCloseCurrentFile(mArchive);
			throw Error ("Archive", getID(), "Cannot read file '" + fileName + "'");
		}
	
		buf[info.uncompressed_size] = '\0';

		file->setData(buf, info.uncompressed_size);

		unzCloseCurrentFile(mArchive);
	}
};