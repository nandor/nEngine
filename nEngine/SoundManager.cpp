/**
    @file       SoundManager.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "SoundManager.hpp"

namespace nEngine {
	template<> SoundManager* SoundManager::Singleton<SoundManager>::__inst = NULL;

	const ALCchar* SoundManager::mDeviceName = "DirectSound3D";
	
	// ------------------------------------------------------------------
	SoundManager::SoundManager()
		:mIsRunning(true),
		 mMaxVolume(5.0f)
	{
		mDevice = alcOpenDevice(mDeviceName);
		mContext = alcCreateContext(mDevice, NULL);
		alcMakeContextCurrent(mContext);
			
		ALenum error = alcGetError(mDevice);
		if (error != ALC_NO_ERROR) {
			throw Error("SoundManager", "OpenAL initialisation failed!");
		}

		mBackground = NULL;
	}
	
	// ------------------------------------------------------------------
	SoundManager::~SoundManager()
	{
		mIsRunning = false;

		// close OpenAL
		alcMakeContextCurrent(NULL);
		alcDestroyContext(mContext);
		alcCloseDevice(mDevice);
	}
	
	// -----------------------------------------------------------------
	void SoundManager::start(int volume)
	{
		setVolume(volume);
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		alListener3f(AL_VELOCITY, 1.0f, 0.0f, 0.0f);
	}


	// ------------------------------------------------------------------
	void SoundManager::update()
	{
		boost::ptr_list<Sound>::iterator it = mSoundList.begin();
		while (it != mSoundList.end()) {
			if (it->isStarted() && !it->isPlaying()) {
				mSoundList.erase(it++); 
			} else {
				++it;
			}
		}

		if (mBackground) {
			mBackground->update();
		}
	}

	// ------------------------------------------------------------------
	SoundManager& SoundManager::addSound(Sound* snd)
	{
		if (snd == NULL || snd->isStarted() && !snd->isPlaying()) {
			return *this;
		}

		mSoundList.push_back(snd);
		return *this;
	}

	// ------------------------------------------------------------------
	SoundManager& SoundManager::setMusic(Music* music)
	{
		if (mBackground) {
			delete mBackground;
			mBackground = NULL;
		}

		mBackground = music;
		mBackground->play();
		return *this;
	}

	// ------------------------------------------------------------------
	SoundManager& SoundManager::setVolume(int f)
	{
		float vol = f / 100.0f * mMaxVolume;
		if (f < 0.0 || mMaxVolume < vol) {
			throw Error("SoundManager", "Bad volume: " + boost::lexical_cast<std::string> (f));
		}

		mMasterVolume = vol;
		alListenerf(AL_GAIN, mMasterVolume);
		return *this;
	}

	// ------------------------------------------------------------------
	int SoundManager::getVolume()
	{
		return mMasterVolume / mMaxVolume * 100;
	}
};