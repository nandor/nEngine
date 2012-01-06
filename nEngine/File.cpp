/**
	@file File.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/


#include "nHeaders.hpp"
#include "File.hpp"
#include "Archive.hpp"

namespace nEngine {



	// ------------------------------------------------------------------
	File::File(const std::string& id)
		:Resource(id, RESOURCE_FILE), mSize(0), mData(NULL), mOkay(true)
	{
		boost::regex fileRegex("^fs://[a-zA-Z0-9_/]+\\.[a-zA-Z0-9_]+$");
		boost::regex zipRegex("^zip://[a-zA-Z0-9_/]+\\.zip/[a-zA-Z0-9_/]+\\.[a-zA-Z0-9_]+$");

		if (boost::regex_match(id.c_str(), fileRegex)) {
			mFromArchive = false;
			mFileName = id.substr(5, id.length() - 5);

			loadFromFile(mFileName);
		} else if (boost::regex_match(id.c_str(), zipRegex)) {
			boost::regex packageRegex("[a-zA-Z0-9_][a-zA-Z0-9_/]+\\.zip");
			boost::match_results<std::string::const_iterator> what;
			
			if (boost::regex_search(id, what, packageRegex, boost::match_partial)) {
				if (what[0].matched) {
					mArchiveName = what[0].str();
					mFromArchive = true;

					mFileName.resize(id.end() - what[0].second);
					copy (what[0].second + 1, id.end(), mFileName.begin());
					loadFromArchive(mArchiveName, mFileName);
				}
			}
		} else {
			mOkay = false;
			throw Error ("File", id, "Invalid file name: '" + mFileName + "'");
		}
	}


	// ------------------------------------------------------------------
	File::~File()
	{
		delete[] mData;
	}


	// ------------------------------------------------------------------
	void File::loadFromFile(const std::string& fileName)
	{
		FILE* file;

		if (fopen_s(&file, fileName.c_str(), "rb")) {
			throw Error ("File", getID(), "Cannot open file");
		}

		fseek(file, 0, SEEK_END);
		mSize = ftell(file);
		fseek(file, 0, SEEK_SET);
	
		mData = new uint8[mSize + 1];
		fread(mData, sizeof(uint8), mSize, file);
		mData[mSize] = '\0';

		fclose(file);
	}


	// ------------------------------------------------------------------
	void File::loadFromArchive(const std::string& archiveId, const std::string& fileName)
	{
		Archive* archive =  Resources::inst().require<Archive> (archiveId);
	
		if (!archive) {
			throw Error ("File", getID(), "Invalid archive: " + archiveId); 
		}
	
		archive->readFile(this, fileName);
	}
};