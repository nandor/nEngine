/**
    Character.hpp

    (c) 2011 Licker Nandor
*/

#ifndef CHARACTER_H
#define CHARACTER_H

#include "nEngine/Object.hpp"
using namespace nEngine;

class Character : public nEngine::Object {
public:
	Character(const std::string& id);
	~Character();

	void draw ();
	void drawMarker();
	void update ();
};

bool luaRegisterCharacter(lua_State* L);

#endif /*ImageManager_H*/

