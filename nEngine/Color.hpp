/**
    @file       Color.hpp
	@author		Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef COLOR_HPP
#define COLOR_HPP

namespace nEngine {
	class Color {
	public:
		Color();
		Color(float l);
		Color(float l, float a);
		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);

		Color operator * (float f);
		Color operator / (float f);

		void glUse();
		float* getVec();
	private:
		float mV[4];
	};
};

#endif /*COLOR_HPP*/