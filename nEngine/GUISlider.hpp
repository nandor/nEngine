/**
	@file GUIButton.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUISLIDER_HPP
#define GUISLIDER_HPP

#include "Singleton.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	class GUISlider : public GUIElement  {
	public:
		/**
			Create a new slider
			@param id			ID of the slider
		*/
		NAPI GUISlider(const std::string& id);
		
		/**
			Destroy the slider
		*/
		NAPI ~GUISlider();

		/**
			Draw the slider
		*/
		NAPI void onDraw();

		/**
			Return the value of the slider
			@return			A value from 0 to 100
		*/
		NAPI unsigned getValue()
		{
			return mValue;
		}

		/**
			Set the value of the slider
			@param value		A number from 0 to 100
		*/
		NAPI void setValue(unsigned value)
		{
			mValue = value;
		}

	private:

		void onMouseDown(GUIEvent& evt);
		void onMouseMove(GUIEvent& evt);
		void onMouseUp(GUIEvent& evt);

	private:

		/// Value of the slader
		unsigned mValue;

		/// Color of the slider
		Color mSliderColor;

		/// Is the slider down
		bool mSliderDown;
	};
};

#endif /*GUISLIDER_HPP*/