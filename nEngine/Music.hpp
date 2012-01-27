/**
    @file       Music.hpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef MUSIC_HPP
#define MUSIC_HPP

#include "types.hpp"
#include "File.hpp"

namespace nEngine {
	class Music {
	public:
		
		/**
			Load a streamed sound as background music
		*/
		NAPI Music(const std::string& source);
		
		/**
			Unload the music & stream
		*/
		NAPI ~Music();

		/**
			Update the music
		*/
		void update();

		/**
			Play the music
		*/
		void play();

		/**
			Check if the music is playing
			@return			Is the music playing?
		*/
		bool isPlaying();

		/**
			Stop the music
		*/
		void stop();

	private:

		/**
			Load the ogg stream
		*/
		void openOgg();
		
		/**
			Initialise the buffers & listeners
		*/
		void openBuffers();

		/**
			Stream a piece of music
			@param buffer			Destination buffer
			@return					Success
		*/
		bool stream(ALuint buffer);

		/**
			Check if OpenAL stuff went okay
		*/
		void checkErrors();

	private:

		/// Samples in a buffers
		static const int sBufferSize = 1024 * 32;

		/// Number of buffers
		static const int sNumBuffers = 2;
	private:
		
		/// nEngine source file
		File* mFile;

		/// Ogg stream handle
		OggVorbis_File mOggStream;

		/// Sound info
		vorbis_info* mVorbisInfo;

		/// Sound comment
		vorbis_comment* mVorbisComment;

		/// 2 buffers - 1 playin', 1 loadin'
		ALuint mBuffers[sNumBuffers];

		/// Audio source
		ALuint mSource;

		/// Audio format (stereo or audio)
		ALenum mFormat;

		/// Sample rate
		ALuint mRate;

		/// Is the music playing?
		bool mPlaying;

		/// Name of the source file
		std::string mSourceName;
	};
};

#endif /*MUSIC_HPP*/