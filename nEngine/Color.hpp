/**
    @file       Color.hpp
	@author		Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef COLOR_HPP
#define COLOR_HPP

#include "types.hpp"

namespace nEngine {
	class Color {
	public:
		NAPI Color();
		NAPI Color(float l);
		NAPI Color(float l, float a);
		NAPI Color(float r, float g, float b);
		NAPI Color(float r, float g, float b, float a);
		NAPI Color(const std::string& str);

		NAPI Color operator * (float f);
		NAPI Color operator / (float f);
	
		NAPI void glUse();
		NAPI float* getVec();

		NAPI float getR() 
		{
			return mV[0];
		}

		NAPI float getG()
		{
			return mV[1];
		}

		NAPI float getB()
		{
			return mV[2];
		}

		NAPI float getA()
		{
			return mV[3];
		}

		NAPI Color& setA(float a)
		{
			mV[3] = a;
			return *this;
		}
	private:

		NAPI void HSVtoRGB();

		float mV[4];
	};
};

#endif /*COLOR_HPP*/