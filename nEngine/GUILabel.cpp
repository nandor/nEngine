/**
	@file GUILabel.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUILabel.hpp"

namespace nEngine {

	// ------------------------------------------------------------------
	GUILabel::GUILabel(const std::string& id)
		:GUIElement(id)
	{

	}

	
	// ------------------------------------------------------------------
	GUILabel::~GUILabel()
	{

	}
	

	// ------------------------------------------------------------------
	void GUILabel::onDraw()
	{
		mFontColor.glUse();
		Font* ft = Resources::inst().get<Font> (mFontName);

		std::vector<std::string> lines;
		boost::split(lines, mText, boost::is_any_of("\n"));
		
		unsigned lineHeight = ft->getHeight() + 4, linePos = 0;

		typedef std::vector<std::string>::iterator tIter;

		for (tIter it = lines.begin(); it != lines.end(); ++it) {
			std::vector<std::string> words;
			boost::split(words, *it, boost::is_any_of(" "));

			unsigned tlen = 0;

			linePos += lineHeight;

			for (tIter jt = words.begin(); jt != words.end(); ++jt) {
				unsigned wlen = ft->getTextWidth(*jt);
				if (tlen + wlen > mSize.getX()) {
					tlen = 0, linePos += lineHeight;
				}

				glPrint(ft, tlen, linePos, *jt + " ");
				tlen += wlen + ft->getCharWidth(' ');
			}

			if (linePos + lineHeight > mSize.getY()) {
				break;
			}
		}
	}
};
