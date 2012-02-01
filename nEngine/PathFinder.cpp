/**
	@file PathFinder.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "PathFinder.hpp"
#include "Scene.hpp"

namespace nEngine {
	static const int dx[] = {0, -1, 0, 1, 1, 1, -1, -1};
	static const int dy[] = {1, 0, -1, 0, 1, -1, 1, -1};
	static const int mNumDir = 4;

	// ------------------------------------------------------------------
	PathFinder::PathFinder(Map* map, Vec2& source)
	{
		mSx = source.getX(), mSy = source.getY();
		mMap = map;
		mType = FIND_NEIGHBOUR;

		mPrev = NULL;
		mDist = NULL;
	}

	// ------------------------------------------------------------------
	PathFinder::PathFinder(Map* map, Vec2& source, Vec2& dest)
	{
		mMap = map;
		mLimit = 10;
		mType = FIND_PATH;

		mPrev = new Vec2* [(mLimit << 1) + 4];
		mDist = new int*[(mLimit << 1) + 4];

		for (int i = 0; i < (mLimit << 1) + 4; ++i) {
			mPrev[i] = new Vec2 [mLimit << 1];
			mDist[i] = new int[mLimit << 1];

			for (int j = 0; j < (mLimit << 1); ++j) {
				mDist[i][j] = INF;
			}
		}
		
		
		mSx = source.getX(), mSy = source.getY();
		mFx = dest.getX(), mFy = dest.getY();
	}
	
	// ------------------------------------------------------------------
	PathFinder::~PathFinder()
	{
		if (mPrev && mDist) {
			for (int i = 0; i < (mLimit << 1); ++i) {
				delete[] mPrev[i];
				delete[] mDist[i];
			}

			delete[] mPrev;
			delete[] mDist;
		}
	}

	// ------------------------------------------------------------------
	bool PathFinder::onLimitZone (int cx, int cy)
	{
		return  0 <= xIndex(cx) && xIndex(cx) < (mLimit << 1) &&
				0 <= yIndex(cy) && yIndex(cy) < (mLimit << 1) && 
				mMap->hasTile (cx, cy);
	}
	

	// ------------------------------------------------------------------
	int PathFinder::xIndex(int idx)
	{
		return idx - mSx + mLimit;
	}

	
	// ------------------------------------------------------------------
	int PathFinder::yIndex(int idx)
	{
		return idx - mSy + mLimit;
	}
	
	// ------------------------------------------------------------------
	int PathFinder::ryIndex(int idx)
	{
		return idx + mSy - mLimit;
	}
	
	// ------------------------------------------------------------------
	int PathFinder::rxIndex(int idx)
	{
		return idx + mSx - mLimit;
	}

	// ------------------------------------------------------------------
	std::vector<Vec2> PathFinder::getPath()
	{
		if (mType != FIND_PATH) {
			std::vector<Vec2> v;
			return v;
		}

		mList.push_back(std::make_pair(mSx, mSy));
		mDist[mLimit][mLimit] = 0;

		for (tListIter it = mList.begin(); it != mList.end(); ++it) {
			int x = it->first, y = it->second;
			for (int k = 0; k < mNumDir; ++k) {
				int nx = x + dx[k], ny = y + dy[k];
				
				if (onLimitZone(nx, ny) && onLimitZone(nx, ny)) {
					Tile* tile = mMap->getTile(nx, ny);
					if (!mMap->isBlocked(nx, ny) && !Scene::inst().getNodeAt(Vec2(nx, ny))) {
						if (mDist[xIndex(x)][yIndex(y)] + 1 < mDist[xIndex(nx)][yIndex(ny)]) {

							mDist[xIndex(nx)][yIndex(ny)] = mDist[xIndex(x)][yIndex(y)] + 1;
							mPrev[xIndex(nx)][yIndex(ny)] = Vec2(dx[k], dy[k]);

							mList.push_back(std::make_pair(nx, ny));
						}
					}
				}
			}
		}
    
		std::vector<Vec2> ret;
		int length = mDist[xIndex(mFx)][yIndex(mFy)];
		
		int px = mFx, py = mFy;
		if (mDist[xIndex(mFx)][yIndex(mFy)] == INF) {
			int min = INF;
			for (int i = 0; i <= (mLimit << 1); ++i) {
				for (int j = 0; j <= (mLimit << 1); ++j) {
					if (mMap->hasTile (rxIndex(i), ryIndex(j))) {
						if (!mMap->isBlocked (rxIndex(i), ryIndex(j)) && !Scene::inst().getNodeAt(Vec2(rxIndex(i), ryIndex(j)))) {
							int d = abs (mFx - rxIndex(i)) + abs (mFy - ryIndex(j));
							if (d < min || (d == min && mDist[i][j] < mDist[xIndex(px)][yIndex(py)])) {
								min = d;
								px = rxIndex(i), py = ryIndex(j);
							}
						}
					}
				}
			}
		}

		Vec2 pt(px, py);
		for (int i = 0; i < length; ++i) {
			ret.push_back (pt);

			Vec2 d = mPrev[xIndex(pt.getX())][yIndex(pt.getY())];

			if (d == Vec2(0, 0)) {
				ret.pop_back();
				break;
			}

			pt -= d;
		}

		return ret;
	}

	// ------------------------------------------------------------------
	Vec2 PathFinder::getRandomNeighbour()
	{
		std::vector<Vec2> vec;
		for (int i = 0; i < mNumDir; ++i) {
			Vec2 pos = Vec2(mSx + dx[i], mSy + dy[i]);
			if (!mMap->hasTile(pos.getX(), pos.getY())) {
				continue;
			}

			Tile* tile = mMap->getTile(pos);
			
			if (tile->isBlocked() || Scene::inst().getNodeAt(pos)) {
				continue;
			}

			vec.push_back(pos);
		}

		return vec[rand() % vec.size()];
	}
};