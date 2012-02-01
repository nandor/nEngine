/**
    @file       Lua.hpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef LUAHELPER_H
#define LUAHELPER_H

#include "File.hpp"
#include "Resources.hpp"

#define luaNewMethod(cls, func) static int cls ## _ ## func (lua_State* L)

#define luaBeginMethods(cls) static luaL_Reg cls ## _methods [] = {
#define luaMethod(cls, func) {#func, cls ## _ ## func},
#define luaEndMethods() {NULL, NULL}};

#define luaBeginMeta(cls) \
	static luaL_Reg cls ## _meta [] = {\
		{"__index", nEngine::luaClassIndex},\
		{"__newindex", nEngine::luaClassNewIndex},\

#define luaMetaMethod(cls, func) {#func, cls ## _ ## func},
#define luaEndMeta() {NULL, NULL}};

#define luaClass(L, cls)\
	luaL_newmetatable(L, #cls);\
	luaL_register(L, NULL, cls ## _meta);\
	lua_pushstring(L, "__name");\
	lua_pushstring(L, #cls);\
	lua_settable(L, -3);\
	luaL_register(L, #cls, cls ## _methods);
	
#define luaInheritClass(L, cls, base)\
	luaL_newmetatable(L, #cls);\
	luaL_register(L, NULL, cls ## _meta);\
	lua_pushstring(L, "__name");\
	lua_pushstring(L, #cls);\
	lua_settable(L, -3);\
	lua_pushstring(L, "__base");\
	luaL_getmetatable(L, #base);\
	lua_settable(L, -3);\
	luaL_register(L, #cls, cls ## _methods);

#define luaInstance(L, cls, val)\
	cls** __c = (cls**)lua_newuserdata(L, sizeof(cls*));\
	(*__c) = val;\
	luaL_getmetatable(L, #cls);\
	lua_setmetatable(L, -2);\
	
#define luaError(L, message)\
    lua_pushstring(L, message);\
    lua_error(L);

namespace nEngine {
    /**
        Init the main lua state
		@param L			Lua global state
    */
    NAPI lua_State* initLua();

    /**
        Close the lua state
    */
    NAPI void closeLua();

    /**
        Load and execute a lua script
        @param fileName     Name of the file
        @return             True on success
    */
    NAPI bool luaReadFile (const std::string& fileName);

    /**
        Register an error callback
        @param func         Function to call when something goes wrong
    */
    NAPI void luaErrorCallback(void (*func)(const std::string& msg));

    /**
        Get the global lua state
        @return             Global state
    */
    NAPI lua_State* luaGlobalState();
		 
	/**
        Get a pointer to a class intance
        @param L            Lua state
		@param index		Index of the udata in the stack
		@param name			Name of the class
        @return             Pointer to userdata
    */
    NAPI void* luaGetInstance(lua_State* L, int index, const std::string& name);

	/**
		Get a global lua integer
		@param id			Identifier of the variable
		@return				Value of the integer
	*/
	NAPI int luaGetGlobalInt(const std::string& id);

	/**
		Get a global lua boolean
		@param id			Identifier of the variable
		@return				Value of the integer
	*/
	NAPI bool luaGetGlobalBoolean(const std::string& id);

	/**
		Get a global lua string
		@param 
		@return				Value of the integer
	*/
	NAPI std::string luaGetGlobalString(const std::string& id);

	/**
		Default lua __index metamethod
		@param L			Lua state
		@return				Number of return values
	*/
	NAPI int luaClassIndex(lua_State* L);

	/**
		Default lua __newindex metamethod
		@param L			Lua state
		@return				Number of return values
	*/
	NAPI int luaClassNewIndex(lua_State* L);

	/**
		Lua error callback function
	*/
	NAPI int luaErrorCallback(lua_State* L);

	NAPI lua_State* luaRegisterEngine(lua_State* L);
};

#endif /*LUAHELPER_H*/
