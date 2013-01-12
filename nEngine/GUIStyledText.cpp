/**
	@file GUIStyledText.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIStyledText.hpp"
#include "GUI.hpp"

namespace nEngine {	
	// -------------------------------------------------------------------------
	GUIStyledText::GUIStyledText(const std::string& id)
		:GUIElement(id),
		 mLineSpacing(5)
	{

	}
	
	
	// -------------------------------------------------------------------------
	GUIStyledText::~GUIStyledText()
	{

	}
	
	
	// -------------------------------------------------------------------------
	void GUIStyledText::onDraw()
	{		
		mBackgroundColor.glUse();

		Vec2 size = getScreenSize();

		glEnable(GL_SCISSOR_TEST);
		glScissor(mPos.getX(), size.getY() - mPos.getY() - mSize.getY(), mSize.getX(), mSize.getY());

		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(mSize.getX(), 0);
			glVertex2i(mSize.getX(), mSize.getY());
			glVertex2i(0, mSize.getY());
		glEnd();	

		typedef std::vector<std::string>::iterator tIter;

		mFontColor.glUse();
		unsigned linePos = 0;
		for (tChunkIter it = mChunks.begin(); it != mChunks.end(); ++it) {
			switch (it->mType) {
				case CONTENT_TEXT: {
					Font* ft = Resources::inst().get<Font> (it->mFont);
					unsigned tlen = 5, lineHeight;
					std::vector<std::string> words;

					if (!ft) {
						throw Error("GUIStyledText", getID(), 
							"Font does not exist: '" + it->mFont + "'");
					}

					lineHeight = ft->getHeight() + mLineSpacing;
					boost::split(words, it->mData, boost::is_any_of(" "));

					linePos += lineHeight;

					for (tIter jt = words.begin(); jt != words.end(); ++jt) {
						unsigned wlen = ft->getTextWidth(*jt);
						if (tlen + wlen > mSize.getX()) {
							tlen = 5, linePos += lineHeight;
						}

						glPrint(ft, tlen, linePos, *jt + " ");
						tlen += wlen + ft->getCharWidth(' ');
					}

					if (linePos + lineHeight > mSize.getY()) {
						break;
					}
					break;
				}
				case CONTENT_IMAGE: {
					unsigned mWidth = it->mWidth.getMetric(mSize.getX());
					unsigned mHeight = it->mHeight.getMetric(mSize.getY());

					Shader::useProgram("texture");
					Image* img = Resources::inst().require<Image> (it->mData);
					if (img == NULL) {
						throw Error("GUIStyledText", getID(), "Image does not exist: '" + it->mData + "'");
					}
					linePos += mLineSpacing;

					img->bind(0);
					glBegin(GL_QUADS);
						glTexCoord2f(0.0f, 0.0f); glVertex2i((mSize.getX() - mWidth) / 2, linePos);
						glTexCoord2f(1.0f, 0.0f); glVertex2i((mSize.getX() - mWidth) / 2 + mWidth, linePos);
						glTexCoord2f(1.0f, 1.0f); glVertex2i((mSize.getX() - mWidth) / 2 + mWidth, linePos + mHeight);
						glTexCoord2f(0.0f, 1.0f); glVertex2i((mSize.getX() - mWidth) / 2, linePos + mHeight);
					glEnd();
					Shader::popProgram();

					linePos += mHeight;
					break;
				}
			}
		}

		glDisable(GL_SCISSOR_TEST);
	}
	
	void GUIStyledText::setContent(std::string content)
	{
		typedef std::string::iterator iter;
		std::string currentFont = "mono8";
		unsigned i = 0;
		
		mChunks.clear();
		while (i < content.length()) {
			// we have a text node
			if (content[i] == '<') {
				unsigned j = content.find_first_of('>', i + 1);

				if (j == std::string::npos) {
					throw Error("GUIStyledtext", getID(), "Missing '>'");
				}

				TextNode node;
				node.mType = CONTENT_TEXT;
				node.mFont = currentFont;
				node.mData.insert(node.mData.begin(), 
					content.begin() + i + 1, content.begin() + j);
				mChunks.push_back(node);
				i = j;
			}

			if (content[i] == '{') {
				unsigned j = content.find_first_of('}', i + 1);

				if (j == std::string::npos) {
					throw Error("GUIStyledtext", getID(), "Missing '}'");
				}
				currentFont.clear();
				currentFont.insert(currentFont.begin(), 
					content.begin() + i + 1, content.begin() + j);
				i = j;
			}

			if (content[i] == '!') {
				unsigned firstComma = content.find_first_of(',', i + 1);
				unsigned secondComma = content.find_first_of(',', firstComma + 1);
				unsigned end = content.find_first_of('!', secondComma + 1);

				if (firstComma == std::string::npos || secondComma == std::string::npos ||
					end == std::string::npos)
				{
					throw Error("GUIStyledText", getID(), "Invalid image declaration");
				}

				TextNode node;
				node.mType = CONTENT_IMAGE;
				node.mWidth = GUIMetric(std::string(content.begin() + i + 1, content.begin() + firstComma));
				node.mHeight = GUIMetric(std::string(content.begin() + firstComma + 1, content.begin() + secondComma));
				node.mData = std::string(content.begin() + secondComma + 1, content.begin() + end);
				mChunks.push_back(node);
				i = end;
			}

			++i;
		}
	}
	 
	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIStyledText, new)
	{
		GUIStyledText* pnl = new GUIStyledText(luaL_checkstring(L, 1));
		luaInstance(L, GUIStyledText, pnl);
		return 1;
	}

	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIStyledText, __setter)
	{
		GUIStyledText* text = *(GUIStyledText**)luaGetInstance(L, 1, "GUIStyledText");
		std::string field(luaL_checkstring(L, 2));

		if (field == "content") {
			text->setContent(luaL_checkstring(L, 3));
			return 0;
		}

		lua_pushboolean(L, 0);
		return 1;
	}
	
	
	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIStyledText, get)
	{
		GUIStyledText* panel = (GUIStyledText*)GUI::inst().get(luaL_checkstring(L, 1));
		luaInstance(L, GUIStyledText, panel);
		return 1;
	}

	// -------------------------------------------------------------------------
	luaBeginMeta(GUIStyledText)
		luaMethod(GUIStyledText, __setter)
	luaEndMeta()

	luaBeginMethods(GUIStyledText)
		luaMethod(GUIStyledText, new)
		luaMethod(GUIStyledText, get)
	luaEndMethods()
	
	// -------------------------------------------------------------------------
	bool luaRegisterGUIStyledText(lua_State* L)
	{
		luaInheritClass(L, GUIStyledText, GUIElement);
		return true;
	}
};