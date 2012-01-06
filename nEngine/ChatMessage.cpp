/**
	@file ChatAction.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "ChatMessage.hpp"

namespace nEngine {

	// Chat message
	ChatMessage::ChatMessage()
	{
		mType = CHAT_PLAYER;
	}

	
	// ------------------------------------------------------------------
	ChatMessage::~ChatMessage()
	{

	}

	
	// ------------------------------------------------------------------
	ChatAction& ChatMessage::getAction(unsigned idx)
	{
		if (mActions.size() == 0) {
			throw Error("ChatMessage", getID(), "Invalid action: '" + boost::lexical_cast<std::string>(idx) + "'");
		}

		return idx < mActions.size() ? mActions[idx] : mActions[0];
	}
	

	// ------------------------------------------------------------------
	void ChatMessage::addAction(const std::string& message, ChatActionType type, const std::string& action)
	{
		ChatAction act;
		act.setText(message);
		act.setType(type);

		mActions.push_back(act);
	}


	// ------------------------------------------------------------------
	void ChatMessage::removeAction(unsigned id)
	{
		if (0 <= id && id < mActions.size()) {
			mActions.erase(mActions.begin() + id);
		}
	}


	// ------------------------------------------------------------------
	int ChatMessage::getNumActions()
	{
		return mActions.size();
	}
};