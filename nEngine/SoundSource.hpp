/**
    @file       SoundSource.hpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef SOUNDSOURCE_HPP
#define SOUNDSOURCE_HPP

#include "Resource.hpp"

namespace nEngine {
	/**
		Class to load ogg file and convert it 
		to an OpenAL buffer
	*/
	class SoundSource : public Resource {
	public:
		
		/**
			Load an off file and create a buffer
		*/
		SoundSource(const std::string& id);
		
		/**
			Delete the buffer
		*/
		~SoundSource();

		/**
			Return a pointer to the buffer
			@return			ALuint
		*/
		ALuint getBuffer()
		{
			return mBuffer;
		}
	
	private:

		void loadOggBuffer(std::vector<char>& buffer);

	private:

		/// ID of the buffer
		ALuint			mBuffer;

		/// Format (mono or stereo)
		ALenum			mFormat;

		/// Frequency of the sound
		ALsizei			mFreq;

		/// File name
		std::string		mFileName;

	public:
		
		/// Resource type
        NAPI static const ResourceType sType = RESOURCE_SOUND;

	private:

		/// Size of the buffer
		static const int sBufferSize = 4096 * 8;
	};
};

#endif /*SOUNDSOURCE_HPP*/