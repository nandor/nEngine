/**
    @file       MapHelper.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Map.hpp"
#include "Scene.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	NAPI bool MapHelper::neighbours(Vec2& first, Vec2& second)
	{
		int dx[] = {0, -1, 0, 1, 1, 1, -1, -1};
		int dy[] = {1, 0, -1, 0, 1, -1, 1, -1};

		for (int i = 0; i < 4; ++i) {
			if (first + Vec2(dx[i], dy[i]) == second)
				return true;
		}
		return false;
	}

	// ------------------------------------------------------------------
	int MapHelper::getDirection(Vec2& p1, Vec2& p2)
	{
		int dx[] = {1, 1, 0, -1, -1, -1,  0,  1};
		int dy[] = {0, 1, 1,  1,  0, -1, -1, -1};

		for (int i = 0; i < 8; ++i) {
			if (p1.getX() + dx[i] == p2.getX() && p1.getY() + dy[i] == p2.getY()) {
				return i;
			}
		}
		return 0;
	}
	
	// ------------------------------------------------------------------
	NAPI float MapHelper::distance(const Vec2& p1, const Vec2& p2)
	{
		float dx = p1.getX() - p2.getX();
		float dy = p2.getY() - p2.getY();

		return sqrt(dx * dx + dy * dy);
	}

};