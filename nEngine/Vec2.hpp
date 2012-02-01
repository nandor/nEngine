/**
    @file       Lua.hpp
    @author     Licker Nandor

    @brief 2-dimensional Vector

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef VEC2_HPP
#define VEC2_HPP

#include "types.hpp"
#include "Lua.hpp"

namespace nEngine {
	/**
		2D Vector
	*/
	class Vec2 {
	public:
		NAPI Vec2(float x, float y);
		NAPI Vec2();
		
		NAPI ~Vec2();

		NAPI float getX() const {
			return mV[0];
		}

		NAPI float getY() const {
			return mV[1];
		}

		NAPI void setX(float x) {
			mV[0] = x;
		}

		NAPI void setY (float y) {
			mV[1] = y;
		}

		NAPI float* getVec() {
			return mV;
		}

		NAPI float length ();

		Vec2 operator = (const Vec2& v);
		Vec2 operator + (const Vec2& v);
		Vec2 operator - (const Vec2& v);
		Vec2 operator - ();

		float operator * (const Vec2& v);
		Vec2 operator += (const Vec2& v);
		Vec2 operator -= (const Vec2& v);

		Vec2 operator * (float s);
		Vec2 operator / (float s);
		Vec2 operator *= (float s);
		Vec2 operator /= (float s);

		bool operator == (const Vec2& v);
		bool operator != (const Vec2& v);

		bool operator < (const Vec2& v);
		bool operator > (const Vec2& v);

		static bool luaRegister(lua_State* L);

	private:
		float mV[2];
	};
};

#endif /*VEC2_HPP*/