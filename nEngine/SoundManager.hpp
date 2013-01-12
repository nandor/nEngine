/**
    @file       SoundManager.hpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "types.hpp"
#include "Error.hpp"
#include "Object.hpp"
#include "Singleton.hpp"
#include "Sound.hpp"
#include "Music.hpp"

namespace nEngine {
	class SoundManager : public Singleton<SoundManager> {
	public:
		/**
			Initialise the manager & OpenAL
		*/
		NAPI SoundManager();

		/**
			Deinitialise OpenAL
		*/
		NAPI ~SoundManager();
		
		/**
			Update the sounds & background music
		*/
		NAPI void update();

		/**
			Start the sound manager
			@param volume			Sound volume from 0 to 100
		*/
		NAPI void start(int volume);

		/**
			Add a new song
		*/
		NAPI SoundManager& addSound(Sound* sound);

		/**
			Set the background music
			@param music		Pointer to the music
		*/
		NAPI SoundManager& setMusic(Music* music);

		/**
			Return a pointer to the background music
			@return				Pointer
		*/
		NAPI Music* getMusic()
		{
			return mBackground;
		}

		/**
			Set the master volume
			@param f			Volume (from 0 to 100)
		*/
		NAPI SoundManager& setVolume(int f);

		/**
			Get the master volume
			@return				volume
		*/
		NAPI int getVolume();

		/**
			Set the listener position
			@param vec			Position of the listener
		*/
		NAPI void setListenerPosition(Vec2& v);

		/**
			Set the object to be followed by the listener
		*/
		NAPI void setListener(Object* obj);

	private:

		NAPI static const ALCchar* mDeviceName;
		
		/// OpenAL device (DirectSound3D)
		ALCdevice* mDevice;

		/// OpenAL context
		ALCcontext* mContext;

		/// List of running sounds
		boost::ptr_list<Sound> mSoundList;

		/// Background music
		Music* mBackground;

		/// Is the thread running?
		bool mIsRunning;

		/// Volum
		float mMasterVolume;

		/// Maximum volume
		const float mMaxVolume;
	};

	bool luaRegisterSoundManager(lua_State* L);
};

#endif /*SOUNDMANAGER_HPP*/