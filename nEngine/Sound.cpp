/**
    @file       Sound.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "SoundManager.hpp"
#include "Sound.hpp"
#include "File.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	Sound::Sound(const std::string& id)
		:mSourceName(id),
		 mStarted(false)
	{
		SoundManager::inst().addSound(this);
		
		/// Create the source
		alGenSources(1, &mSource);

		/// Set default values
		alSource3f(mSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
				
		int code = alGetError();
		if (code != AL_NO_ERROR) {
			throw Error("OpenAL", alGetString(code));
		}
	}

	// ------------------------------------------------------------------
	Sound::~Sound()
	{
		alSourceStop(mSource);
		alDeleteSources(1, &mSource);
	}

	// ------------------------------------------------------------------
	void Sound::play()
	{
		alSourceStop(mSource);
		SoundSource* source = nEngine::Resources::inst().require<SoundSource> (mSourceName);		
		mStarted = true;

		alSourcei(mSource, AL_BUFFER, source->getBuffer());
		alSourcePlay(mSource); 
		
	}
	
	// ------------------------------------------------------------------
	void Sound::loop()
	{
		alSourceStop(mSource);
		SoundSource* source = nEngine::Resources::inst().require<SoundSource> (mSourceName);
		mStarted = true;

		alSourcei(mSource, AL_BUFFER, source->getBuffer());
		alSourcei(mSource, AL_LOOPING, AL_TRUE);
		alSourcePlay(mSource);
	}
	
	// ------------------------------------------------------------------
	void Sound::stop()
	{
		alSourceStop(mSource);
	}

	// ------------------------------------------------------------------
	bool Sound::isPlaying() const
	{
		int isPlaying;
		alGetSourcei(mSource, AL_SOURCE_STATE, &isPlaying);
		return isPlaying == AL_PLAYING;
	}

	// ------------------------------------------------------------------
	void Sound::setPosition(Vec2& v)
	{
		alSource3f(mSource, AL_POSITION, v.getX(), v.getY(), 0.0f);
	}
	
	// ------------------------------------------------------------------
	luaBeginMethods(Sound)

	luaEndMethods(Sound)

	luaBeginMeta(Sound)

	luaEndMeta(Sound)
	
	// ------------------------------------------------------------------
	bool luaRegisterSound(lua_State* L)
	{
		luaClass(L, Sound);
		return true;
	}
};