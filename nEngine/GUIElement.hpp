/**
	@file GUIElement.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUIELEMENT_HPP
#define GUIELEMENT_HPP

#include "types.hpp"
#include "Color.hpp"
#include "Vec2.hpp"
#include "GUIEvent.hpp"
#include "Util.hpp"

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

	/**
		Base class for the GUI
	*/
	class GUIElement {
	public:
		/**
			Constructor
			@param id		ID of the element
		*/
		NAPI GUIElement (const std::string& id);

		/**
			Destructor
		*/
		NAPI virtual ~GUIElement ();

		/**
			Get the id of the element
			@return			ID of the element
		*/
		NAPI std::string getID()
		{
			return mID;
		}

		/**
			Get the font name
			@return			Name of the font
		*/
		NAPI std::string getFont()
		{
			return mFontName;
		}

		/**`
			Set the font name
			@param name		New font name
		*/
		NAPI void setFont(const std::string& name)
		{
			mFontName = name;
		}

		/**
			Draw the element and its children
		*/
		NAPI virtual void draw();

		/**
			Draw callback
		*/
		NAPI virtual void onDraw();

		/**
			Handle the events for this element
			@param evt		GUIEvent holding event data
		*/
		NAPI virtual bool handleEvent (GUIEvent& evt);
		
		/**
			Set the background color
			@param color		Color of the background
		*/
		NAPI void setBackgroundColor(const Color& color)
		{
			mBackgroundColor = color;
		}

		/**
			Set the text color
			@param color		Color of the text
		*/
		NAPI void setTextColor(const Color& color)
		{
			mFontColor = color;
		}
		
		/**
			Add a new child element
			@param elem		Pointer to the element
		*/
		NAPI void add (GUIElement* elem);

		/**
			Get the parent node
			@return			Pointer to the parent node
		*/
		NAPI GUIElement* getParentNode()
		{
			return mParent;
		}
		
		/**
			Get the size of an element
			@return			Vec2 containing size
		*/
		NAPI Vec2 getSize()
		{
			return mComputedSize;
		}

		/**
			Get the computed position of an element
			@return			Vec2 containing position
		*/
		NAPI Vec2 getPosition()
		{
			return mComputedPos;
		}

		/**
			Connect an event handler
			@param evt		Event type
			@param func		Event handler
		*/
		NAPI void connect(GUIEventType type, boost::function<void(GUIEvent&)> func);
		
		/**
			Check if the element is visible?
			@return			True if it's visible
		*/
		NAPI bool isVisible()
		{
			return mIsVisible;
		}

		/**
			Hide the element
		*/
		NAPI void hide()
		{
			mIsVisible = false;
		}

		/**
			Show the element
		*/
		NAPI void show()
		{
			mIsVisible = true;
		}

		NAPI void setAlignment(GUIAlign horz, GUIAlign vert)
		{
			mHorzAlign = horz;
			mVertAlign = vert;
		}

		NAPI void setSize(Vec2 size, GUISize horzSize = GUI_SIZE_PIXEL, GUISize vertSize = GUI_SIZE_PIXEL)
		{
			mSize = size;
			mWidthType = horzSize;
			mHeightType = vertSize;
		}

		NAPI void applyTo(GUIElement* elem);
		
		/**
			Set the position of the element
			@param pos		Position
		*/
		NAPI void setPosition(Vec2 pos)
		{
			mPos = pos;
		}

		NAPI void enable()
		{
			mEnabled = true;
		}

		NAPI void disable()
		{
			mEnabled = false;
		}

		NAPI void remove();

	protected:
		/**
			Check if the mouse is over an element
			@param pos		Mouse position
		*/		
		virtual bool isUnderMouse(Vec2 pos);

		/**
			Fire an event!
			@param evt		An event
		*/
		void fireEvent(GUIEvent& evt);

		/**
			Compute the size of the element
		*/
		void computeSize();

	protected:
		/// ID of the element
		std::string mID;

		/// Font
		std::string mFontName;
		
		/// Size of the element
		Vec2 mSize;

		/// Position of the element (relative to parent)
		Vec2 mPos;

		/// Background color
		Color mBackgroundColor;

		/// Font color
		Color mFontColor;

		/// Is the mouse over the element?
		bool mMouseOver;

		/// Is the mouse pressed?
		bool mMousePressed; 

		/// Is the element visible?
		bool mIsVisible;

		/// Is the elemnt active?
		bool mEnabled;

		/// The parent node
		GUIElement* mParent;
		
		/// Map containing children
		std::map<std::string, GUIElement*> mChildren;

		/// Iterator to the children
		typedef std::map<std::string, GUIElement*>::iterator tChildIter;
		
		/// Map contatining events
		std::vector<boost::function<void(GUIEvent&)> > mEvents[GUI_NUM_EVENTS];

		/// Iterator to the events
		typedef std::vector<boost::function<void(GUIEvent&)> >::iterator tEventIter;

		/// Vertical alignment
		GUIAlign mVertAlign;
		
		/// Horizontal alignment
		GUIAlign mHorzAlign;
		
		/// Computed position
		Vec2 mComputedPos;

		/// Computed size
		Vec2 mComputedSize;

		/// Vertical size type
		GUISize mWidthType;

		/// Horizontal size type
		GUISize mHeightType;
	};

	bool luaRegisterGUIElement(lua_State* L);
};
#endif /*GUIELEMENT_HPP*/