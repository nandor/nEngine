/**
	@file MapHelper.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef MAPHELPER_HPP
#define MAPHELPER_HPP

#include "types.hpp"
#include "Vec2.hpp"
#include "Util.hpp"

namespace nEngine {
	namespace MapHelper {
		/**
			Check if two tiles are neighbours
			@param first		Coordinate of the first tile
			@param second		Coordinate of the second tile

			@return				True / False
		*/		
		NAPI bool neighbours(Vec2& first, Vec2& second);

		/**
			Get the direction from a tile to another
			@param first		Coordinate of the first tile
			@param second		Coordinate of the second tile

			@return				Direction
		*/
		NAPI int getDirection(Vec2& p1, Vec2& p2);

		namespace nDirection {
			enum NDIRECTION {
				nDirS,
				nDirSE,
				nDirE,
				nDirNE,
				nDirN,
				nDirNW,
				nDirW,
				nDirSW
			};
		};
	};
};

#endif MAPHELPER_HPP