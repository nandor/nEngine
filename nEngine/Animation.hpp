/**
	@file Animation.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "nHeaders.hpp"
#include "types.hpp"
#include "Timer.hpp"

namespace nEngine {
	class Animation {
	public:
		/**
			Constructor
		*/
		NAPI Animation();
		
		/**
			Destructor
		*/
		NAPI ~Animation();

		/**
			Set the number of frames of the animation
			@param frames			Number of frames in the animation
		*/
		NAPI void setFrames(int frames) 
		{
			mFrames = frames;
		}


		/**
			Get the number of frames from the animation
			@return					Number of frames
		*/
		NAPI int getFrames()
		{
			return mFrames;
		}

		/**
			Set the duration of the animation
			@param duration			Duration
		*/
		NAPI void setDuration(int duration)
		{
			mDuration = duration;
		}

		/**
			Get the duration of the animation
			@return					Duration (ms)
		*/
		NAPI int getDuration()
		{
			return mDuration;
		}

		/**
			Set the image name
			@param	name			Name of the image
		*/
		NAPI void setImage(const std::string& name)
		{
			mImage = name;
		}

		/**
			Get the image name		
			@return					Name of the image
		*/
		NAPI std::string getImage()
		{
			return mImage;
		}

		/**
			Draw the current frame
		*/
		NAPI void draw();

		/**
			Get the height of the frames
		*/
		NAPI int getFrameHeight();
		
		/**
			Get the width of the frames
		*/
		NAPI int getFrameWidth();

		/**
			Run the animation for a period of time
			@param time			
		*/	
		NAPI void run(float time);

		/**
			Check if the animation has expired
		*/
		bool isRunning();

		/**
			Set the id of the animation
			@param id			New id
		*/
		void setID(const std::string& id)
		{
			mID = id;
		}

		/**
			Get the id of the animation
			@return				ID
		*/
		std::string getID()
		{
			return mID;
		}
	public:

		/// Number of frames in an animation
		int mFrames;

		// ID of the animation
		std::string mID;

		/// Duration of the animation
		int mDuration;

		/// Image for the animation
		std::string mImage;

		/// Start time of the animation
		float mStartTime;

		/// Runtime
		float mRunTime;
	};
};

#endif /*ANIMATION_HPP*/