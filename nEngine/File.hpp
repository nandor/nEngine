/**
	@file File.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef FILE_HPP
#define FILE_HPP

#include "types.hpp"
#include "Error.hpp"
#include "Resource.hpp"
#include "Resources.hpp"

namespace nEngine {

	enum FileOffset {
		FILEOFFSET_SET = SEEK_SET,
		FILEOFFSET_END = SEEK_END,
		FILEOFFSET_CUR = SEEK_CUR
	};

	/**
		File 

		Files are loaded only when they are required. The id can express their location to.
		File IDs can have the following forms:
			fs://path_to_file
			zip://path_to_archive/file_in_archive
	*/
    class File : public Resource {
    public:
		/**
			Create a new file
			@param id			a very special id
		*/
        NAPI File(const std::string& id);

		/**
			Create a new file
			@param id			ID of the resource
			@param file			URI of the file
		*/
		NAPI File(const std::string& id, const std::string& file);
		
		/**
			Destroy the file
		*/
        NAPI ~File();
		
		/**
			Write the data to a file or archive (not implemented yet)
			@param data			String containing data
		*/
		NAPI void write();
		
		/**
			Set the data
			@param data			Pointer to data
			@param size			Length of the file
		*/
		NAPI void setData(void* data, unsigned size)
		{
			mData = data;
			mSize = size;
		}

		/**
			Copy data into the file
			@param data			Pointer to data
			@param size			Length of the file
		*/
        NAPI void copyData(const void* data, unsigned size);

		/**
			Return the data
			@param				Pointer to the data
		*/
        NAPI void* getData()
        {
            return mData;
        }

		/**
			Return the size of a file
			@return				Size of file (bytes)
		*/
        NAPI unsigned getSize ()
        {
            return mSize;
        }

		/**
			Return the ammount of memory used by the resource
			@return				Memory in bytes
		*/
		NAPI int getMemoryUsage()
		{
			return mSize + sizeof(*this);
		}

		/**
			Get the position
			@return				Position (bytes from the start)
		*/
		NAPI long tell()
		{
			return mPosition;
		}
	
		/**
			Set the pointer
			@param from			Position
			@param offset		Offset
		*/
		NAPI bool set(unsigned howMany, FileOffset offset);

		/**
			Read some bytes
			@param buffer		Buffer to read in
			@param length		Lenght of the buffer
			@return				Number of bytes read
		*/
		NAPI int read(void* buf, unsigned length);

	public:
		
		/**
			Strip the prefix of a filename (fs://test.zip -> test.zip)
			@param fileName				Input name
			@return						Filename without header
		*/
		NAPI static std::string getRawName(const std::string& fileName);

		/**
			Strip an environment variable
			@param fileName				Input name
			@return						Filename without the var
		*/
		NAPI static std::string strip(const std::string& fileName, const std::string& pack = "path");

		/**
			Set an environment 
			@param name					Name of the variable
			@param value				Value
		*/
		NAPI static void setenv(const std::string& name, const std::string& value);

		/**
			Get the value of an environment variable's value
			@param name					The name of the environment variable
			@return						The value
		*/
		NAPI static std::string getenv(const std::string& name);
				
		/**
			Replace environment variables in a file name
			@param str			Reference to the name
		*/
		NAPI static std::string processName(const std::string& name);

	public:

		/// Type of the resource
        static const ResourceType sType = RESOURCE_FILE;

		/// Environment variables
		static std::map<std::string, std::string> sEnvironment;

    private:

		/**
			Load the resource from the file
		*/
        void loadFromFile();

		/**
			Load the resource from an archive
		*/
        void loadFromArchive();

		/**
			Write the data
		*/
		void writeData();

	private:

		/// Pointer to the data
        void* mData;

		/// Size of the data
        unsigned mSize;

		/// Name of the archive
        std::string mArchiveName;

		/// Name of the file
		std::string mFileName;

		/// Is the resource loaded from the archive?
        bool mFromArchive;

		/// Position
		unsigned mPosition;
    };
};

#endif /*FILE_HPP*/
