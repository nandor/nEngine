/**
    @file       SoundSource.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "oggReader.hpp"
#include "SoundSource.hpp"
#include "Resources.hpp"

namespace nEngine {
	
	// ------------------------------------------------------------------
	SoundSource::SoundSource(const std::string& id)
		:Resource(id, RESOURCE_SOUND)
	{
		mFileName = id;

		// Load the buffer
		std::vector<char> buffer;
		loadOggBuffer(buffer);

		alGenBuffers(1, &mBuffer);
		alBufferData(mBuffer, mFormat, &buffer[0], (ALsizei)buffer.size(), mFreq);
		
		// Check for erorrs				
		int code = alGetError();
		if (code != AL_NO_ERROR) {
			throw Error("SoundSource", getID(), alGetString(code));
		}
	}
	
	// ------------------------------------------------------------------
	SoundSource::~SoundSource()
	{
		// Delete the buffer
		alDeleteBuffers(1, &mBuffer);
	}

	// ------------------------------------------------------------------
	void SoundSource::loadOggBuffer(std::vector<char>& buffer)
	{
		int endian = 0, bitStream, bytes;
		char arr[sBufferSize];

		OggVorbis_File oggFile;
		vorbis_info* info;

		// Prepare the source
		File* file = Resources::inst().require<File> (mFileName);
		if (ov_open_callbacks(file, &oggFile, NULL, 0, oggReader) < 0) {
			throw Error("SoundSource", getID(), "Ogg read error!");
		}

		// Get sound info
		info = ov_info(&oggFile, -1);
		mFormat = (info->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		mFreq = info->rate;

		// Read the entire data into a buffer
		do {			
			bytes = ov_read(&oggFile, arr, sBufferSize, endian, 2, 1, &bitStream);
			buffer.insert(buffer.end(), arr, arr + bytes);
		} while (bytes > 0);
		
		// Close the file
		ov_clear(&oggFile);
	}
};