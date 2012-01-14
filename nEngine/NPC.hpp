/**
	@file NPC.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef NPC_HPP
#define NPC_HPP

#include "types.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace nEngine {
	class NPC : public Object {
	public:
		NPC(const std::string& id, const std::string& base);
		~NPC();

		void draw();
		void drawMarker();
		void update();

		/**
			Set the spawn point of the NPC
			@param pos			Spawn position
		*/
		void setSpawn(Vec2 pos)
		{
			mSpawn = pos;
			setTile(pos);
		}

	private:
		/// Name of the npc
		std::string mName;

		/// Spawn point
		Vec2 mSpawn;

		/// Wander radius
		int mWanderRadius;

		/// Check if automove is enabled
		bool mWander;
	};
	
	/**
		Register the NPC
	*/
	bool luaRegisterNPC(lua_State* L);
};

#endif /*NPC_HPP*/