/**
	@file GUISizer.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUISizer.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	
	// ------------------------------------------------------------------
	GUISizer::GUISizer()
		:mVertAlign(GUI_ALIGN_NONE),
		 mHorzAlign(GUI_ALIGN_NONE),
		 mPos(0, 0),
		 mSize(0, 0),
		 mComputedPos(0, 0),
		 mComputedSize(0, 0),
		 mWidthType(GUI_SIZE_PIXEL),
		 mHeightType(GUI_SIZE_PIXEL)
	{

	}
	
	
	// ------------------------------------------------------------------
	GUISizer::~GUISizer()
	{

	}
	
	
	// ------------------------------------------------------------------
	void GUISizer::applyTo(GUIElement* elem)
	{
		if (!elem || !elem->getParentNode()) {
			throw Error ("GUISizer", "An element with a parent is required!");
		}

		GUIElement* parent = elem->getParentNode();

		Vec2 parentSize = parent->getSize();
		
		mComputedSize = mSize;
		if (mWidthType == GUI_SIZE_PERCENT) {
			mComputedSize.setX(parentSize.getX() * mSize.getX() / 100.0f);
		}

		if (mHeightType == GUI_SIZE_PERCENT) {
			mComputedSize.setY(parentSize.getY() * mSize.getY() / 100.0f);
		}


		mComputedPos = mPos;
		switch (mHorzAlign) {
		case GUI_ALIGN_LEFT:
			mComputedPos.setX(0);
			break;
		case GUI_ALIGN_RIGHT:
			mComputedPos.setX(parentSize.getX() - mComputedSize.getX());
			break;
		case GUI_ALIGN_CENTER:
			mComputedPos.setX((parentSize.getX() - mComputedSize.getX()) / 2);
			break;
		default:
			break;
		}

		switch (mVertAlign) {
		case GUI_ALIGN_TOP:
			mComputedPos.setY(0);
			break;
		case GUI_ALIGN_BOTTOM:
			mComputedPos.setY(parentSize.getY() - mComputedSize.getY());
			break;
		case GUI_ALIGN_CENTER:
			mComputedPos.setY((parentSize.getY() - mComputedSize.getY()) / 2);
			break;
		default:
			break;
		}

		mComputedPos += parent->getPosition();

	}
};