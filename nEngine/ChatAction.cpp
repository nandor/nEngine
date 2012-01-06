/**
	@file ChatAction.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "ChatAction.hpp"

namespace nEngine {

	// ------------------------------------------------------------------
	ChatAction::ChatAction()
	{
		mType = CHAT_ACTION_NEXT;
		mNextChat = 0;
		luaFunctionName = std::string("none");
		mText = std::string("none");
	}


	// ------------------------------------------------------------------
	ChatAction::~ChatAction()
	{

	}
};