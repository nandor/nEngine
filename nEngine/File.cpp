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
	/// Regexes used to identify file types
	std::regex fileRegex("^fs://[a-zA-Z0-9_/]+\\.[a-zA-Z0-9_]+$");
	std::regex zipRegex("^zip://[a-zA-Z0-9_/]+\\.zip/[a-zA-Z0-9_/]+[a-zA-Z0-9_/\\.]+$");
	
	// Statics
	std::map<std::string, std::string> File::sEnvironment;
	
	// ------------------------------------------------------------------
	File::File(const std::string& id)
		:Resource(id, RESOURCE_FILE), mData(NULL), mSize(0),
		 mPosition(0)
	{
		const std::string& fileID = processName(id);

		if (std::regex_match(fileID.c_str(), fileRegex)) {
			mFileName = processName (fileID.substr(5, fileID.length() - 5));

			loadFromFile();
			return;
		} 
		
		if (std::regex_match(fileID.c_str(), zipRegex)) {
			std::regex packageRegex("[a-zA-Z0-9_][a-zA-Z0-9_/]+\\.zip");
			std::match_results<std::string::const_iterator> what;
			
			if (!std::regex_search(fileID, what, packageRegex) || !what[0].matched) {
				throw Error("File", getID(), "Invalid archive name");
			}
			
			mFileName.resize(fileID.end() - what[0].second);
			copy (what[0].second + 1, fileID.end(), mFileName.begin());

			mArchiveName = processName (what[0].str());
			mFileName = processName(mFileName);

			loadFromArchive();
			return;
		}

		mFileName = fileID;
		loadFromFile();
	}
	
	// ------------------------------------------------------------------
	File::File(const std::string& id, const std::string& fileName)
		:Resource(id, RESOURCE_FILE), mSize(0), mData(NULL)
	{
		mFromArchive = false;
		mFileName = processName(fileName);
		loadFromFile();
	}

	// ------------------------------------------------------------------
	File::~File()
	{
		delete[] mData;
	}


	// ------------------------------------------------------------------
	void File::loadFromFile()
	{
		mFromArchive = false;

		FILE* file;
		if (fopen_s(&file, mFileName.c_str(), "rb")) {
			throw Error ("File", getID(), "Cannot open file");
		}

		fseek(file, 0, SEEK_END);
		mSize = ftell(file);
		fseek(file, 0, SEEK_SET);
	
		mData = new uint8[mSize + 1];
		fread(mData, sizeof(uint8), mSize, file);
		((char*)mData)[mSize] = '\0';

		fclose(file);
	}


	// ------------------------------------------------------------------
	void File::loadFromArchive()
	{
		mFromArchive = true;
		Archive* archive =  Resources::inst().require<Archive> (mArchiveName);
	
		if (!archive) {
			throw Error ("File", getID(), "Invalid archive: " + mArchiveName); 
		}
	
		archive->readFile(this, mFileName);
	}
	
	// ------------------------------------------------------------------
	void File::copyData(const void* data, unsigned size)
    {
		if (mData != NULL) {
			delete[] mData;
		}
		
		mData = new char[size + 1];
		mSize = size;

		memcpy(mData, data, mSize);
		((char*)mData)[size] = '\0';
    }

	// ------------------------------------------------------------------
	void File::write()
	{
		if (!mFromArchive) {
			FILE* fout = fopen(mFileName.c_str(), "wb");
			if (!fout) {
				throw Error("Cannot open output file!");
			}

			fwrite(mData, sizeof(uint8), mSize, fout);
			fclose(fout);
		} else {
			throw Error("File", getID(), "Cannot write file to archive!");
		}
	}

	// ------------------------------------------------------------------
	std::string File::processName(const std::string& name)
	{
		std::string fileName = "";

		for (unsigned i = 0; i < name.size(); ++i) {
			if (name[i] == '%') {
				std::string envVar = "";

				while (name[++i] != '%') {
					envVar.push_back(name[i]);

					if (i == name.size()) {
						throw Error("File", "Invalid name: '" + name + "'");
					}
				}

				fileName.append(File::getenv(envVar.c_str()));
			} else {
				fileName.push_back(name[i]);
			}
		}

		return fileName;
	}

	// ------------------------------------------------------------------
	std::string File::strip(const std::string& name, const std::string& pack)
	{
		std::string fileName = "";

		for (unsigned i = 0; i < name.size(); ++i) {
			if (name[i] == '%') {
				std::string envVar = "";

				while (name[++i] != '%') {
					envVar.push_back(name[i]);

					if (i == name.size()) {
						throw Error("File", "Invalid name: '" + name + "'");
					}
				}

				if (envVar != pack) {
					fileName.append(File::getenv(envVar.c_str()));
				}
			} else {
				fileName.push_back(name[i]);
			}
		}

		if (fileName[0] == '/') {
			return fileName.substr(1, fileName.size());
		}

		return fileName;
	}

	// ------------------------------------------------------------------
	std::string File::getenv(const std::string& name)
	{
		std::map<std::string, std::string>::const_iterator it = File::sEnvironment.find(name);

		if (it == sEnvironment.end()) {
			throw Error("Environment", "Variable '" + name + "' does not exist!");
		}

		return it->second;
	}

	// ------------------------------------------------------------------
	void File::setenv(const std::string& name, const std::string& value)
	{
		sEnvironment[name] = value;
	}
	
	// ------------------------------------------------------------------
	std::string File::getRawName(const std::string& fileName)
	{
		if (std::regex_match(fileName.c_str(), fileRegex)) {
			std::string processed = processName(fileName.substr(5, fileName.length() - 5));
			
			while (!processed.empty() && *(processed.end() - 1) == '/') {
				processed.pop_back();
			}
			return processed;
		} 

		throw Error("File", "Invalid file name!");
	}

	// ------------------------------------------------------------------
	int File::read(void* buf, unsigned length)
	{
		unsigned numBytes = std::min (mSize - mPosition, length);
		
		memcpy(buf, (char*)mData + mPosition, numBytes);
		mPosition += numBytes;

		return numBytes;
	}
	
	// ------------------------------------------------------------------
	bool File::set(unsigned howMany, FileOffset offset)
	{
		switch (offset) {
		case FILEOFFSET_SET:
			mPosition = howMany;
			break;
		case FILEOFFSET_END:
			mPosition = mSize - howMany;
			break;
		case FILEOFFSET_CUR:
			mPosition += howMany;
			break;
		}

		return 0 <= mPosition && mPosition < mSize;
	}
};