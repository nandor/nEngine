/**
	@file Font.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef FONT_HPP
#define FONT_HPP

#include "Lua.hpp"
#include "Resource.hpp"
#include "Resources.hpp"

namespace nEngine {
    class Font : public Resource {
    public:
        /**
            Create a new font from an opened library
            @param _height       Height of the font
        */
        Font(const std::string& _id, unsigned _height);

        /**
            Destructor
        */
        ~Font();

        /**
            Get the list base
            @return             OpenGL display list base
        */
        unsigned getListBase() {
			return mListBase;
		}


        /**
            Get the height of the font
            @return             Font height
        */
        unsigned getHeight() {
			return mHeight;
		}

		/**
			Get the width of the text
			@param str			Text to measure
			@return				Width of the text
		*/
		unsigned getTextWidth(const std::string& str);

		/**	
			Return the width of a character
			@param ch			Character
			@return				Width of the character
		*/
		unsigned getCharWidth(char ch);

		/**
			Get the memory usage
			@return				Memory usage
		*/
		int getMemoryUsage();
    private:

        /**
            Create a display list for a certain character
            @param ch           Character
            @return             Success
        */
        bool makeDisplayList(char ch);

    private:

        /// Pointer to an array containing texture ids
        unsigned *mTextures;

        /// Height of the font
        unsigned mHeight;

        /// OpenGL list pointer
        unsigned mListBase;

		/// Width of the characters
		unsigned *mCharWidth;
    public:
        static const ResourceType sType = RESOURCE_FONT;

        static bool initLibrary();
        static bool closeLibrary();

        static bool loadFace(const std::string& name);
		static bool unloadFace();
		static bool luaRegister(lua_State* L);
    private:

        static FT_Library sLibrary;
        static FT_Face    sFace;
        static bool       sHasFace;
    };
};

#endif /*FONT_HPP*/
