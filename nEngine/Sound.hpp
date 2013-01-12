/**
    @file       Sound.hpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef SOUND_HPP
#define SOUND_HPP

#include "types.hpp"
#include "Resource.hpp"
#include "SoundSource.hpp"
#include "Vec2.hpp"
#include "File.hpp"

namespace nEngine {
	class Sound {
	public:
		/**
			Create a new sound source
			@param id of the sound source
		*/
		NAPI Sound (const std::string& source);

		/**
			Delete the sound source
		*/
		NAPI ~Sound();
		
		/**
			Check if the sound is playing
			@return				Sound state
		*/
		NAPI bool isPlaying() const;
		
		/**
			Check if the sound was started
			@return				t / f
		*/
		NAPI bool isStarted() const
		{
			return mStarted;
		}
		
		/**
			Set the position of the listener
			@param pos			Position
		*/
		NAPI void setPosition(Vec2& pos);
		
		/**
			Play the sound
		*/
		NAPI void play();

		/**
			Play the sound forever!
		*/
		NAPI void loop();

		/**
			Stop the sound from playing
		*/
		NAPI void stop();
	private:

		/// Source id
		ALuint			mSource;

		/// Sound source name
		std::string		mSourceName;

		/// Was the sound started? (started & finished songs are dumped)
		bool			mStarted;
	};

	bool luaRegisterSound(lua_State* L);
};

#endif /*SOUND_HPP*/