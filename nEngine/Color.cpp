/**
    @file       Color.hpp
	@author		Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "Util.hpp"
#include "Error.hpp"
#include "Color.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
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
	Color::Color(const std::string& str)
	{
		mV[0] = mV[1] = mV[2] = mV[3] = 0.0f;

		bool isHSV;
		if (str[0] == '#') {
			isHSV = false;
		} else if (str[0] == '%') {
			isHSV = true;
		} else {
			if (boost::algorithm::to_lower_copy(str) != "none") {
				throw Error("Color", "Invalid value: '" + str + "'");
			}
			return;
		}
		
		// remove the @ or %
		std::string value = str.substr(1, str.length());
		boost::algorithm::to_lower(value);

		if (value.find_first_of("01234567890abcdef") == std::string::npos) {
			throw Error("Color", "Invalid value: '" + str + "'");
		}
		
		switch (value.length()) {
		case 3:
			// XYZ
			mV[0] = charToInt(value[0]) / 15.0f;
			mV[1] = charToInt(value[1]) / 15.0f;
			mV[2] = charToInt(value[2]) / 15.0f;
			mV[3] = 1.0f;
			break;
		case 4:
			// XYZA
			mV[0] = charToInt(value[0]) / 15.0f;
			mV[1] = charToInt(value[1]) / 15.0f;
			mV[2] = charToInt(value[2]) / 15.0f;
			mV[3] = charToInt(value[4]) / 15.0f;
			break;
		case 6:
			// #XXYYZZ
			mV[0] = ((charToInt(value[0]) << 4) + charToInt(value[1])) / 255.0f;
			mV[1] = ((charToInt(value[2]) << 4) + charToInt(value[3])) / 255.0f;
			mV[2] = ((charToInt(value[4]) << 4) + charToInt(value[5])) / 255.0f;
			mV[3] = 1.0f;
			break;
		case 8:
			// #XXYYZZAA
			mV[0] = ((charToInt(value[0]) << 4) + charToInt(value[1])) / 255.0f;
			mV[1] = ((charToInt(value[2]) << 4) + charToInt(value[3])) / 255.0f;
			mV[2] = ((charToInt(value[4]) << 4) + charToInt(value[5])) / 255.0f;
			mV[3] = ((charToInt(value[6]) << 4) + charToInt(value[7])) / 255.0f;
			break;
		default:
			throw Error("Color", "Invalid value: '" + str + "'");
		}

		if (isHSV) {
			HSVtoRGB();
		}
	}
	
	// ------------------------------------------------------------------
	void Color::HSVtoRGB()
	{
		// HR SG VB
		float chroma = mV[1] / mV[2];
		float hdash = mV[0] / 60.0f;
		float x = chroma * (1.0 - abs((hdash - (((int)(hdash / 2.0)) << 1)) - 1));
		float min = mV[2] - chroma;
 
		if(hdash < 1.0)
		{
			mV[0] = chroma;
			mV[1] = x;
		}
		else if(hdash < 2.0)
		{
			mV[0] = x;
			mV[1] = chroma;
		}
		else if(hdash < 3.0)
		{
			mV[1] = chroma;
			mV[2] = x;
		}
		else if(hdash < 4.0)
		{
			mV[1] = x;
			mV[2] = chroma;
		}
		else if(hdash < 5.0)
		{
			mV[0] = x;
			mV[2] = chroma;
		}
		else if(hdash < 6.0)
		{
			mV[0] = chroma;
			mV[2] = x;
		}
 
		mV[0] += min;
		mV[1] += min;
		mV[2] += min;
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

	// ------------------------------------------------------------------
	luaDeclareMethod(Color, new)
	{
		Color* c;
	
		switch (lua_gettop(L)) {
			case 0:
				c = new Color();
				break;
			case 1:
				c = new Color(luaL_checknumber(L, 1));
				break;
			case 2:
				c = new Color(
					luaL_checknumber(L, 1),
					luaL_checknumber(L, 2)
				);
				break;
			case 3:
				c = new Color(
					luaL_checknumber(L, 1),
					luaL_checknumber(L, 2),
					luaL_checknumber(L, 3)
				);
				break;
			case 4:
				c = new Color(
					luaL_checknumber(L, 1),
					luaL_checknumber(L, 2),
					luaL_checknumber(L, 3),
					luaL_checknumber(L, 4)
				);
				break;
			default:
				throw Error("Color", "lua", "Invalid arguments for 'new'");
				break;
		};

		luaInstance(L, Color, c);
		return 1;
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(Color, __gc)
	{
		delete *(Color**)luaGetInstance(L, 1, "Color");
		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMethods(Color)
		luaMethod(Color, new)
	luaEndMethods()

	luaBeginMeta(Color)
	luaEndMeta()

	// ------------------------------------------------------------------
	bool luaRegisterColor(lua_State* L)
	{
		luaClass(L, Color);
		return true;
	}
};