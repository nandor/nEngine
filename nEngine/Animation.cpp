/**
	@file Animation.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "Animation.hpp"
#include "Shader.hpp"
#include "Image.hpp"

namespace nEngine {	
	// ------------------------------------------------------------------
	Animation::Animation()
	{
		mStartTime = 0.0f;
		mRunTime = 0.0f;
	}

	
	// ------------------------------------------------------------------
	Animation::~Animation()
	{

	}
	
	// ------------------------------------------------------------------
	void Animation::draw() 
	{			
		float fFrames = mFrames, time = Timer::inst().getTime();
		float fCurrentFrame = (mRunTime != -1.0 && (mRunTime == 0.0f || (mStartTime && mStartTime + mRunTime >= time))) ? 
			(int)((time - mStartTime) / mDuration) % mFrames : 0;
		
		Shader::setUniformf("uAnimLength", 1, &fFrames);
		Shader::setUniformf("uAnimFrame", 1, &fCurrentFrame);

		Image* img =  Resources::inst().require<Image>(mImage);
		
		glActiveTextureARB(GL_TEXTURE0_ARB); 
		glBindTexture(GL_TEXTURE_2D, img->getTextureID());
		glColor3f(1.0f, 0.0, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f / fFrames * fCurrentFrame, 0.0f); 
			glVertex2i (0, -img->getHeight());

			glTexCoord2f(1.0f / fFrames * (fCurrentFrame + 1.0f), 0.0f); 
			glVertex2i (img->getWidth() / mFrames, -img->getHeight());

			glTexCoord2f(1.0f / fFrames * (fCurrentFrame + 1.0f), 1.0f); 
			glVertex2i (img->getWidth() / mFrames, 0);

			glTexCoord2f(1.0f / fFrames * fCurrentFrame, 1.0f);
			glVertex2i (0, 0);
		glEnd();
	}
	
	
	// ------------------------------------------------------------------
	int Animation::getFrameHeight()
	{
		Image* img =  Resources::inst().require<Image>(mImage);
		return img->getHeight();
	}
	
	// ------------------------------------------------------------------
	int Animation::getFrameWidth()
	{
		Image* img = Resources::inst().require<Image>(mImage);
		return img->getWidth() / mFrames;
	}
	
	void Animation::run(float time)
	{
		mStartTime = Timer::inst().getTime();
		mRunTime = time;
	}

	// ------------------------------------------------------------------
	bool Animation::isRunning()
	{
		return mStartTime + mRunTime >= Timer::inst().getTime();
	}
};