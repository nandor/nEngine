/**
	@file Application.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Lua.hpp"
#include "Chat.hpp"
#include "Console.hpp"
#include "GUI.hpp"
#include "Object.hpp"
#include "NPC.hpp"
#include "ChatBox.hpp"
#include "Particles.hpp"

namespace nEngine {
	lua_State* L = NULL;
	void (*error_call)(const std::string&) = NULL;
	int errorFuncRef;
		
	// ------------------------------------------------------------------
	int luaErrorCallback (lua_State* L)
	{	
		std::string msg ("[lua error]" + std::string (lua_tostring(L, -1)));

		if (error_call) {
			error_call(msg);
		} else {
			std::cerr << msg << std::endl;
		}

		return 1;
	}


	// ------------------------------------------------------------------
	static int luaImport(lua_State* L)
	{
		luaReadFile(std::string(luaL_checkstring(L, 1)));
		return 0;
	}
		
	
		
	// ------------------------------------------------------------------
	lua_State* initLua()
	{
		L = lua_open ();
		luaL_openlibs (L);

		lua_pushcfunction(L, luaImport);
		lua_setglobal(L, "import");

		return L;
	}
	
		
	// ------------------------------------------------------------------
	void closeLua()
	{
		if (L != NULL) {
			lua_close (L);
			L = NULL;
		}
	}
	
		
	// ------------------------------------------------------------------
	lua_State* luaGlobalState()
	{
		return L;
	}
	
		
	// ------------------------------------------------------------------
	void luaErrorCallback(void (*func)(const std::string& msg)) {
		error_call = func;
	}


	// ------------------------------------------------------------------
	bool luaReadFile(const std::string& fileName)
	{
		File* file =  Resources::inst().require<File> (fileName);

		if (!file || !file->getData()) {
			return false;
		}

		lua_pushcfunction (L, luaErrorCallback);
		luaL_loadstring(L, (const char*)file->getData());
		return lua_pcall(L, 0, 0, -2);
	}

	// ------------------------------------------------------------------
	void luaStackDump (lua_State *L) {
		std::cout << "Lua Stack Dump" << std::endl;
		for (int i = 1, top = lua_gettop(L); i <= top; i++) {
			switch (lua_type(L, i)) {    
				case LUA_TSTRING:
				std::cout << "\"" << lua_tostring(L, i) << "\"" << std::endl;
				break;
    
				case LUA_TBOOLEAN:
				std::cout << (lua_toboolean(L, i) ? "true" : "false") << std::endl;
				break;
    
				case LUA_TNUMBER:
				std::cout << lua_tonumber(L, i) << std::endl;
				break;
    
				default: 
				std::cout << lua_typename(L, i) << std::endl;
				break;    
			}
		}
		std::cout << "---------------------------" << std::endl;
    }		

	// ------------------------------------------------------------------
	void* luaGetInstance(lua_State* L, int index, const std::string& name)
	{
		if(!lua_isuserdata(L, index)) {
			luaError(L, "An object is required!");
			return NULL;
		}

		lua_getmetatable(L, index);
		
		while (lua_istable(L, -1)) {
			lua_pushstring(L, "__name");
			lua_rawget(L, -2);
		
			std::string objName(luaL_checkstring(L, -1));
			lua_pop(L, 1);

			if (objName == name) {
				return lua_touserdata(L, index);
			}

			lua_pushstring(L, "__base");
			lua_rawget(L, -2);
		}

		luaError(L, std::string("Argument must be of type '" + name + "'").c_str());
		return NULL;
	}
	
		
	// ------------------------------------------------------------------
	int luaGetGlobalInt(const std::string& id)
	{
		lua_State* L = luaGlobalState();

		lua_getglobal(L, id.c_str());

		return luaL_checknumber(L, -1);
	}
		
	// ------------------------------------------------------------------
	bool luaGetGlobalBoolean(const std::string& id)
	{
		lua_State* L = luaGlobalState();

		lua_getglobal(L, id.c_str());
		
		return lua_toboolean(L, -1);
	}
		
		
	// ------------------------------------------------------------------
	int luaClassIndex(lua_State* L)
	{
		std::string name = std::string(luaL_checkstring(L, 2));
		lua_getmetatable(L, 1);

		while (lua_istable(L, -1)) {
			lua_pushstring(L, name.c_str());
			lua_gettable(L, -2);

			if (!lua_isnil(L, -1)) {
				return 1;
			}

			lua_pop(L, 1);
			lua_pushstring(L, "__getter");
			lua_gettable(L, -2);

			if (!lua_isnil(L, -1)) {
				lua_pushvalue(L, 1);
				lua_pushvalue(L, 2);
				lua_pcall(L, 2, 1, 0);
				return 1;
			} else {
				lua_pop(L, 1);
				lua_pushstring(L, "__base");
				lua_gettable(L, -2);
			}
		}

		return 0;
	}
	
		
	// ------------------------------------------------------------------
	int luaClassNewIndex(lua_State* L)
	{
		lua_getmetatable(L, 1);

		while(lua_istable(L, -1)) {			
			lua_pushstring(L, "__setter");
			lua_gettable(L, -2);

			if (!lua_isnil(L, -1)) {
				lua_pushvalue(L, 1);
				lua_pushvalue(L, 2);
				lua_pushvalue(L, 3);

				lua_pcall(L, 3, 1, 0);
				return 1;
			}
			
			lua_pop(L, 1);
			lua_pushstring(L, "__base");
			lua_gettable(L, -2);
		}

		return 0;
	}
	
	// -----------------------------------------------------------------
	lua_State* luaRegisterEngine()
	{	
		lua_State* L = initLua();

		Vec2::luaRegister(L);
		Console::luaRegister(L);
		Tile::luaRegister(L);
		Font::luaRegister(L);
		Shader::luaRegister(L);
		Chat::luaRegister(L);
		 
		luaRegisterMap(L);
		luaRegisterResources(L);
		luaRegisterScene(L);
		luaRegisterSceneNode(L);
		luaRegisterObject(L);
		luaRegisterNPC(L);
		luaRegisterParticles(L);
		luaRegisterCamera(L);
		luaRegisterGUI(L);
		luaRegisterTimer(L);

		return L;
	}
};