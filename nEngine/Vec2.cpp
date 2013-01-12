/**
	@file Vec2.cpp
	@author Licker Nandor

    @brief 2-dimensional Vector

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Vec2.hpp"

namespace nEngine {	
	// ------------------------------------------------------------------
	Vec2::Vec2()
	{
		mV[0] = mV[1] = 0;
	}
	
	// ------------------------------------------------------------------
	Vec2::Vec2(float x, float y)
	{
		mV[0] = x, mV[1] = y;
	}
	
	// ------------------------------------------------------------------
	Vec2::~Vec2()
	{

	}

	// ------------------------------------------------------------------
	Vec2 Vec2::operator = (const Vec2& v)
	{
		this->mV[0] = v.mV[0];
		this->mV[1] = v.mV[1];

		return *this;
	}

	// ------------------------------------------------------------------
	Vec2 Vec2::operator + (const Vec2& v) 
	{
		return Vec2(this->mV[0] + v.mV[0], this->mV[1] + v.mV[1]);
	}

	// ------------------------------------------------------------------
	Vec2 Vec2::operator - (const Vec2& v) 
	{
		return Vec2(this->mV[0] - v.mV[0], this->mV[1] - v.mV[1]);
	}
	
	// ------------------------------------------------------------------
	Vec2 Vec2::operator - ()
	{
		return Vec2(-this->mV[0], -this->mV[1]);
	}
	
	// ------------------------------------------------------------------
	float Vec2::operator * (const Vec2& v) 
	{
		return this->mV[0] * mV[0] + this->mV[1] * mV[1];
	}

	// ------------------------------------------------------------------
	Vec2 Vec2::operator += (const Vec2& v) 
	{
		this->mV[0] += v.mV[0];
		this->mV[1] += v.mV[1];
		return *this;
	}

	// ------------------------------------------------------------------
	Vec2 Vec2::operator -= (const Vec2& v) 
	{
		this->mV[0] -= v.mV[0];
		this->mV[1] -= v.mV[1];
		return *this;
	}

	// ------------------------------------------------------------------
	Vec2 Vec2::operator * (float s) 
	{
		return Vec2(this->mV[0] * s, this->mV[1] * s);
	}

	// ------------------------------------------------------------------
	Vec2 Vec2::operator / (float s) 
	{
		return Vec2(this->mV[0] / s, this->mV[1] / s);
	}
	
	// ------------------------------------------------------------------
	Vec2 Vec2::operator *= (float s) 
	{
		return Vec2(this->mV[0] *= s, this->mV[1] *= s);
	}

	// ------------------------------------------------------------------
	Vec2 Vec2::operator /= (float s) 
	{
		return Vec2(this->mV[0] /= s, this->mV[1] /= s);
	}


	// ------------------------------------------------------------------
	float Vec2::length() {
		return sqrt(this->mV[0] * this->mV[0] + this->mV[1] * this->mV[1]);
	}

	// ------------------------------------------------------------------
	bool Vec2::operator == (const Vec2& v) 
	{
		return this->mV[0] == v.mV[0] && this->mV[1] == v.mV[1];
	}

	// ------------------------------------------------------------------
	bool Vec2::operator != (const Vec2& v) 
	{
		return this->mV[0] != v.mV[0] || this->mV[1] != v.mV[1];
	}
	
	// ------------------------------------------------------------------
	bool Vec2::operator < (const Vec2& v)
	{
		return this->mV[0] < v.mV[0] && this->mV[1] < v.mV[1];
	}

	// ------------------------------------------------------------------
	bool Vec2::operator > (const Vec2& v)
	{
		return this->mV[0] > v.mV[0] && this->mV[1] > v.mV[1];
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, new) 
	{
		Vec2* v = new Vec2(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

		luaInstance(L, Vec2, v);
		return 1;
	}


	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, __tostring) 
	{
		Vec2** v = (Vec2**)luaGetInstance(L, 1, "Vec2");

		std::string str("(" + boost::lexical_cast<std::string>((*v)->getX()) + 
						"," + boost::lexical_cast<std::string>((*v)->getY()) + ")");
			

		lua_pushstring(L, str.c_str());
		return 1;
	}


	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, __add)
	{
		Vec2* a = *(Vec2**)luaGetInstance(L, 1, "Vec2");
		Vec2* b = *(Vec2**)luaGetInstance(L, 2, "Vec2");

		Vec2* v = new Vec2(a->getX() + b->getX(), a->getY() + b->getY());

		luaInstance(L, Vec2, v);
		return 1;
	}


	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, __eq)
	{
		Vec2* a = *(Vec2**)luaGetInstance(L, 1, "Vec2");
		Vec2* b = *(Vec2**)luaGetInstance(L, 2, "Vec2");
		
		lua_pushboolean(L, a == b);
		return 1;
	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, __unm)
	{
		Vec2* a = *(Vec2**)luaGetInstance(L, 1, "Vec2");

		Vec2* v = new Vec2(-a->getX(), -a->getY());

		luaInstance(L, Vec2, v);
		return 1;
	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, __sub)
	{
		Vec2* a = *(Vec2**)luaGetInstance(L, 1, "Vec2");
		Vec2* b = *(Vec2**)luaGetInstance(L, 2, "Vec2");

		Vec2* v = new Vec2(a->getX() - b->getX(), a->getY() - b->getY());

		luaInstance(L, Vec2, v);
		return 1;
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, __gc)
	{
		Vec2* a = *(Vec2**)luaGetInstance(L, 1, "Vec2");

		delete a;
		return 0;
	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, __getter)
	{
		Vec2* a = *(Vec2**)luaGetInstance(L, 1, "Vec2");

		std::string value(luaL_checkstring(L, 2));

		if (value == "x") {
			lua_pushnumber(L, a->getX());
			return 1;
		}

		if (value == "y") {
			lua_pushnumber(L, a->getY());
			return 1;
		}

		return 0;
	}
	

	// ------------------------------------------------------------------
	luaDeclareMethod(Vec2, __setter)
	{
		Vec2* a = *(Vec2**)luaGetInstance(L, 1, "Vec2");

		std::string value(luaL_checkstring(L, 2));

		if (value == "x") {
			a->setX(luaL_checkinteger(L, 3));
			return 0;
		}

		if (value == "y") {
			a->setY(luaL_checkinteger(L, 3));
			return 0;
		}

		return 0;
	}	

	luaDeclareMethod(Vec2, length)
	{
		Vec2* a = *(Vec2**)luaGetInstance(L, 1, "Vec2");

		lua_pushnumber(L, a->length());
		return 1;
	}

	luaBeginMeta(Vec2)
		luaMethod(Vec2, length)
		luaMethod(Vec2, __tostring)
		luaMethod(Vec2, __gc)
		luaMethod(Vec2, __add)
		luaMethod(Vec2, __eq)
		luaMethod(Vec2, __unm)
		luaMethod(Vec2, __sub)
		luaMethod(Vec2, __getter)
		luaMethod(Vec2, __setter)
	luaEndMeta()


	luaBeginMethods(Vec2)
		luaMethod(Vec2, new)
	luaEndMethods()

	// ------------------------------------------------------------------
	bool Vec2::luaRegister(lua_State* L)
	{
		luaClass(L, Vec2);
		return true;
	}
};
