/**
	@file QuadTreeValue.hpp
	@author Licker Nandor

    @brief Scene Graph for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef QUADTREEVALUE_HPP
#define QUADTREEVALUE_HPP

#include "types.hpp"
#include "Color.hpp"
#include "Vec2.hpp"
#include "SceneNode.hpp"

namespace nEngine {
	class QuadTreeValue {
	public:
		QuadTreeValue(SceneNode* data, const Vec2& pos);
		~QuadTreeValue();

		SceneNode* getData()
		{
			return mData;
		}

		void setData(SceneNode* data) 
		{
			mData = data;
		}

		Vec2 getPosition()
		{
			return mPos;
		}

		void setPosition(const Vec2& pos)
		{
			mPos = pos;
		}
	private:
		SceneNode* mData;
		Vec2 mPos;
	};
};

#endif /*QUADTREEVALUE_HPP*/