/**
    @file       Music.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "oggCallbacks.hpp"
#include "Music.hpp"

namespace nEngine {
		
	// ------------------------------------------------------------------
	Music::Music(const std::string& source)
		:mPlaying(false)
	{
		mSourceName = source;
		
		openOgg();
		openBuffers();
	}
		
	// ------------------------------------------------------------------
	Music::~Music()
	{ 
		stop();
		
		alDeleteSources(1, &mSource);
		alDeleteBuffers(sNumBuffers, mBuffers);
 
		ov_clear(&mOggStream);
	}
	
	// ------------------------------------------------------------------
	void Music::openOgg()
	{
		File* file = Resources::inst().require<File> (mSourceName);
		if (ov_open_callbacks(file, &mOggStream, NULL, 0, oggReader) < 0) {
			throw Error ("Music", mSourceName, "Cannot open ogg file: '" + mSourceName + "'");
		}

		mVorbisInfo = ov_info(&mOggStream, -1);
		mVorbisComment = ov_comment(&mOggStream, -1);

		mFormat = (mVorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		mRate = mVorbisInfo->rate;
	}
	
	// ------------------------------------------------------------------
	void Music::openBuffers()
	{
		alGenBuffers(sNumBuffers, mBuffers); checkErrors();
		alGenSources(1, &mSource); checkErrors();
    
		alSource3f(mSource, AL_POSITION,        0.0, 0.0, 0.0);
		alSource3f(mSource, AL_VELOCITY,        0.0, 0.0, 0.0);
		alSource3f(mSource, AL_DIRECTION,       0.0, 0.0, 0.0);
		alSourcef (mSource, AL_ROLLOFF_FACTOR,  0.0          );
		alSourcei (mSource, AL_SOURCE_RELATIVE, AL_TRUE      );
		
		int code = alGetError();
		if (code != AL_NO_ERROR) {
			throw Error("Music", mSourceName, alGetString(code));
		}
	}
	
	// ------------------------------------------------------------------
	void Music::stop()
	{
		int queued;
		alSourceStop(mSource);
		alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued); checkErrors();
    
		while (queued--) {
			ALuint buffer;
			alSourceUnqueueBuffers(mSource, 1, &buffer); checkErrors();
		}
		mPlaying = false;
	}


	// ------------------------------------------------------------------
	void Music::update()
	{
		if (!mPlaying) {
			return;
		}

		int processed;
		bool active = true;
 
		alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);

		while (processed--) {
			ALuint buffer;
        
			alSourceUnqueueBuffers(mSource, 1, &buffer); checkErrors();
			active = stream(buffer); 
			alSourceQueueBuffers(mSource, 1, &buffer); checkErrors();
		}

		if (!isPlaying()) {
			stop();
			play();
		}
	}
	
	// ------------------------------------------------------------------
	void Music::play()
	{
		if (mPlaying) {
			return;
		}
		
		for (int i = 0; i < sNumBuffers; ++i) {
			if (!stream (mBuffers[i])) {
				return;
			}
		}
			
		alSourceQueueBuffers(mSource, sNumBuffers, mBuffers); checkErrors();
		alSourcePlay(mSource); checkErrors();

		mPlaying = true;
	}
	
	// ------------------------------------------------------------------
	bool Music::isPlaying()
	{
		ALenum state;
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING) && mPlaying;
	}
	
	// ------------------------------------------------------------------
	bool Music::stream(ALuint buffer)
	{
		char data[sBufferSize];
		int size = 0, section, result;
 
		while(size < sBufferSize)
		{
			result = ov_read(&mOggStream, data + size, sBufferSize - size, 0, 2, 1, &section);
    
			if(result > 0) {
				size += result;
				continue;
			}

			if (result == 0) {
				ov_pcm_seek(&mOggStream, 0);
				break;
			}

			if(result < 0) {
				throw Error ("Music", mSourceName, "Streaming error!");
			}
		}
    
		if (size == 0) {
			return false;
		}

		alBufferData(buffer, mFormat, data, size, mRate); checkErrors();
		return true;
	}
	
	// ------------------------------------------------------------------
	void Music::checkErrors()
	{
		int code = alGetError();
		if (code != AL_NO_ERROR) {
			throw Error("Music", mSourceName, alGetString(code));
		}
	} 
};