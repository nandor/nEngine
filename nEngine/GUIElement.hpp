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
#include "GUISizer.hpp"
#include "Util.hpp"

namespace nEngine {
	/**
		Base class for the GUI
	*/
	class GUIElement {
	public:
		/**
			Constructor
			@param id		ID of the element
		*/
		GUIElement (const std::string& id);

		/**
			Destructor
		*/
		virtual ~GUIElement ();

		/**
			Get the id of the element
			@return			ID of the element
		*/
		std::string getID()
		{
			return mID;
		}

		/**
			Get the font name
			@return			Name of the font
		*/
		std::string getFont()
		{
			return mFontName;
		}

		/**`
			Set the font name
			@param name		New font name
		*/
		void setFont(const std::string& name)
		{
			mFontName = name;
		}

		/**
			Draw the element and its children
		*/
		virtual void draw();

		/**
			Draw callback
		*/
		virtual void onDraw();

		/**
			Handle the events for this element
			@param evt		GUIEvent holding event data
		*/
		virtual bool handleEvent (GUIEvent& evt);
		
		/**
			Set the background color
			@param color		Color of the background
		*/
		void setBackgroundColor(const Color& color)
		{
			mBackgroundColor = color;
		}

		/**
			Set the text color
			@param color		Color of the text
		*/
		void setTextColor(const Color& color)
		{
			mFontColor = color;
		}
		
		/**
			Add a new child element
			@param elem		Pointer to the element
		*/
		void add (GUIElement* elem);

		/**
			Get the parent node
			@return			Pointer to the parent node
		*/
		GUIElement* getParentNode()
		{
			return mParent;
		}
		
		/**
			Set the sizer for the element
			@param size		Pointer to a sizer
		*/
		void setSizer(GUISizer* sizer);

		/**
			Get the sizer of the element
			@return			Pointer to the sizer
		*/
		GUISizer* getSizer()
		{
			return mSizer;
		}

		/**
			Get the size of an element
			@return			Vec2 containing size
		*/
		Vec2 getSize()
		{
			return mSize;
		}

		/**
			Get the computed position of an element
			@return			Vec2 containing position
		*/
		Vec2 getPosition()
		{
			return mPos;
		}

		/**
			Connect an event handler
			@param evt		Event type
			@param func		Event handler
		*/
		void connect(GUIEventType type, boost::function<void(GUIEvent&)> func);

		/**
			Check if the element is visible?
			@return			True if it's visible
		*/
		bool isVisible()
		{
			return mIsVisible;
		}

		/**
			Hide the element
		*/
		void hide()
		{
			mIsVisible = false;
		}

		/**
			Show the element
		*/
		void show()
		{
			mIsVisible = true;
		}
	protected:
		/**
			Check if the mouse is over an element
			@param pos		Mouse position
		*/		
		bool isUnderMouse(Vec2 pos);

		/**
			Fire an event!
			@param evt		An event
		*/
		void fireEvent(GUIEvent& evt);

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

		/// The parent node
		GUIElement* mParent;

		/// The sizer
		GUISizer* mSizer;

		/// Map containing children
		std::map<std::string, GUIElement*> mChildren;

		/// Iterator to the children
		typedef std::map<std::string, GUIElement*>::iterator tChildIter;
		
		/// Map contatining events
		std::vector<boost::function<void(GUIEvent&)> > mEvents[GUI_NUM_EVENTS];

		/// Iterator to the events
		typedef std::vector<boost::function<void(GUIEvent&)> >::iterator tEventIter;
	};
};
#endif /*GUIELEMENT_HPP*/