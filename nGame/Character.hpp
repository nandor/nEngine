/**
    Character.hpp

    (c) 2011 Licker Nandor
*/

#ifndef CHARACTER_H
#define CHARACTER_H

#include "nEngine/Object.hpp"
using namespace nEngine;

class Character : public Object {
public:
	Character(const std::string& id);
	~Character();

	void draw ();
	void drawMarker();
	void update ();
	void attack (Object* target);

	/**
		Check if the character is on a certain position
		@param v				Cursor position
		@return					Always true
	*/
	bool intersects(Vec2& v)
	{
		return false;
	}

private:
	int mDamage;
};

bool luaRegisterCharacter(lua_State* L);

#endif /*ImageManager_H*/

