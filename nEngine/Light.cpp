/**
	@file Camera.cpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Light.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	Light::Light(const std::string& id)
		:mID(id)
	{

	}
	
	// ------------------------------------------------------------------
	Light::~Light()
	{

	}
	
	
	// ------------------------------------------------------------------
	void Light::glUse(unsigned id)
	{
		mGLID = id;
		GLuint lightID = GL_LIGHT0 + mGLID;
		float properties[4] = {0.0f};
		float position[4] = {0.0f, 1.0f, 0.0f, 1.0f};

		// Position
		properties[0] = mTile.getX();
		properties[1] = mTile.getY();
		
		// Attenuation
		properties[2] = mRange;

		glEnable(lightID);	
		glLightfv(lightID, GL_POSITION, position);
		glLightfv(lightID, GL_DIFFUSE, mColor.getVec());
		glLightfv(lightID, GL_SPECULAR, properties);
	}

	// ------------------------------------------------------------------
	luaNewMethod(Light, new)
	{
		Light* l = new Light(luaL_checkstring(L, 1));
		luaInstance(L, Light, l);
		return 1;
	}
	
	// ------------------------------------------------------------------
	luaNewMethod(Light, __setter)
	{
		Light* obj = *(Light**)luaGetInstance(L, 1, "Light");
		std::string field(luaL_checkstring(L, 2));

		if (field == "tile") {
			Vec2* tile = *(Vec2**)luaGetInstance(L, 3, "Vec2");
			obj->setTile(*tile);
			return 0;
		}

		if (field == "color") {
			Color* c = *(Color**)luaGetInstance(L, 3, "Color");
			obj->setColor(*c);
			return 0;
		}
		
		if (field == "range") {
			obj->setRange(luaL_checknumber(L, 3));
			return 0;
		}

		return 0;
	}
	
	// ------------------------------------------------------------------
	luaNewMethod(Light, __getter)
	{
		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(Light)
		luaMethod(Light, __setter)
		luaMethod(Light, __getter)
	luaEndMeta()

	luaBeginMethods(Light)
		luaMethod(Light, new)
	luaEndMethods()

	// ------------------------------------------------------------------
	bool luaRegisterLight(lua_State* L)
	{
		luaClass(L, Light)
		return false;
	}
};
