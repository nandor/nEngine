/**
	@file GUISizer.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUISIZER_HPP
#define GUISIZER_HPP

#include "types.hpp"
#include "Color.hpp"
#include "Vec2.hpp"

namespace nEngine {
	enum GUIAlign {
		GUI_ALIGN_TOP,
		GUI_ALIGN_BOTTOM,
		GUI_ALIGN_CENTER,
		GUI_ALIGN_LEFT,
		GUI_ALIGN_RIGHT,
		GUI_ALIGN_NONE
	};

	enum GUISize {
		GUI_SIZE_PERCENT,
		GUI_SIZE_PIXEL,
	};
	
	// Forward declaration
	class GUIElement;

	class GUISizer {
	public:
		GUISizer();
		~GUISizer();

		void setAlignment(GUIAlign horz, GUIAlign vert)
		{
			mHorzAlign = horz;
			mVertAlign = vert;
		}

		void setSize(Vec2 size, GUISize horzSize = GUI_SIZE_PIXEL, GUISize vertSize = GUI_SIZE_PIXEL)
		{
			mSize = size;
			mWidthType = horzSize;
			mHeightType = vertSize;
		}

		void applyTo(GUIElement* elem);
		
		/**
			Set the position of the element
			@param pos		Position
		*/
		void setPosition(Vec2 pos)
		{
			mPos = pos;
		}

		/**
			Get the computed size
			@return			Size of the element
		*/
		Vec2 getSize()
		{
			return mComputedSize;
		}

		/**
			Get the computed position
			@return			Computed position
		*/
		Vec2 getPosition()
		{
			return mComputedPos;
		}
	private:
		/// Vertical alignment
		GUIAlign mVertAlign;
		
		/// Horizontal alignment
		GUIAlign mHorzAlign;

		/// Position
		Vec2 mPos;

		/// Computed position
		Vec2 mComputedPos;

		// Size
		Vec2 mSize;

		/// Computed size
		Vec2 mComputedSize;

		/// Vertical size type
		GUISize mWidthType;

		/// Horizontal size type
		GUISize mHeightType;
	};
};

#endif /*GUISIZER_HPP*/