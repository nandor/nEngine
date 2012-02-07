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
		void setSpawn(Vec2& pos);

		/**
			Let NPCs to wander around
			@param wander		True / False
		*/
		void setWander(bool wander)
		{
			mWander = wander;
		}

		/**
			Check intersection with a point
			@return intersection point
		*/
		bool intersects(Vec2& v);

		/**
			Set the wander radius
			@param radius		Distance the npc is allowed to travel
		*/
		void setWanderRadius(int radius)
		{
			mWanderRadius = radius;
		}

		/**
			Select the NPC. If he is selected, a border
			will be drawn around him
			@param select		True or false
		*/
		NAPI void setSelection(bool select)
		{
			mSelected = select;
		}
	private:
		/// Name of the npc
		std::string mName;

		/// Namespace of the NPC
		std::string mNamespace;

		/// Spawn point
		Vec2 mSpawn;

		/// Wander radius
		int mWanderRadius;

		/// Check if automove is enabled
		bool mWander;

		/// Is he chatty?
		bool mChats;

		/// Is he selected?
		bool mSelected;
	};
	
	/**
		Register the NPC
	*/
	bool luaRegisterNPC(lua_State* L);
};

#endif /*NPC_HPP*/