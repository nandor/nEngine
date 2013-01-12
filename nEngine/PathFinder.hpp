/**
	@file PathFinder.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include "types.hpp"
#include "Map.hpp"

namespace nEngine {
	class PathFinder {
	public:
		PathFinder(Map* map, Vec2& source);
		PathFinder(Map* map, Vec2& source, Vec2& target);
		~PathFinder();

		/**
			Za force of Lee
			@return			Vector containing the path
		*/
		std::vector<Vec2> getPath();

		/**
			Get a random neighbour
			@return			Direction to the neighbour
		*/
		Vec2 getRandomNeighbour();
	private:

		/**
			Check if the point is on the checked zone
		*/
		bool PathFinder::onLimitZone (int cx, int cy);

		int xIndex(int idx);
		int yIndex(int idx);
		int rxIndex(int idx);
		int ryIndex(int idx);
	private:
		int mLimit;
		int **mDist;

		int mSx, mSy, mFx, mFy;

		Map* mMap;
		std::list<std::pair<int, int> > mList;
		Vec2 **mPrev;

		typedef std::list<std::pair<int, int> >::iterator tListIter;

		enum {
			FIND_PATH,
			FIND_NEIGHBOUR
		} mType;
	};
};

#endif /*PATHFINDER_HPP*/