/**
    @file       Color.hpp
	@author		Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "Color.hpp"

namespace nEngine {
	Color::Color()
	{
		mV[0] = mV[1] = mV[2] = mV[3] = 0.0f;
	}
	
	// ------------------------------------------------------------------
	Color::Color (float l)
	{
		mV[0] = mV[1] = mV[2] = l;
		mV[3] = 1.0f;
	}
	
	// ------------------------------------------------------------------
	Color::Color(float l, float a)
	{
		mV[0] = mV[1] = mV[2] = l;
		mV[3] = a;
	}
	
	// ------------------------------------------------------------------
	Color::Color(float r, float g, float b)
	{
		mV[0] = r;
		mV[1] = g;
		mV[2] = b;
		mV[3] = 1.0f;
	}
	
	// ------------------------------------------------------------------
	Color::Color(float r, float g, float b, float a)
	{
		mV[0] = r;
		mV[1] = g;
		mV[2] = b;
		mV[3] = a;
	}
	
	
	// ------------------------------------------------------------------
	Color Color::operator* (float f)
	{
		return Color(this->mV[0] * f, this->mV[1] * f, this->mV[2] * f, this->mV[3] * f);
	}
	
	// ------------------------------------------------------------------
	Color Color::operator/ (float f)
	{
		return Color(this->mV[0] / f, this->mV[1] / f, this->mV[2] / f, this->mV[3] / f);
	}
	
	// ------------------------------------------------------------------
	void Color::glUse()
	{
		glColor4fv(mV);
	}
	
	// ------------------------------------------------------------------
	float* Color::getVec()
	{
		return mV;
	}

};