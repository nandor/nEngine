/**
	@file GUIStyledText.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUISTYLEDTEXT_HPP
#define GUISTYLEDTEXT_HPP

#include "Singleton.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	class GUIStyledText : public GUIElement {
	public:
		
		NAPI GUIStyledText(const std::string& id);
		NAPI ~GUIStyledText();

		NAPI void setContent(std::string content);

	private:

		NAPI void onDraw();
		
		enum ContentType {
			CONTENT_IMAGE,
			CONTENT_TEXT
		};

		struct TextNode {
			GUIMetric mWidth, mHeight;
			std::string mFont;
			std::string mData;
			ContentType mType;
		};

	private:

		typedef std::vector<TextNode>::iterator tChunkIter;
		std::vector<TextNode> mChunks;
		unsigned mLineSpacing;
	};

	bool luaRegisterGUIStyledText(lua_State* L);
};
#endif /*GUISTYLEDTEXT_HPP*/