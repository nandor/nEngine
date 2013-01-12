/**
	@file ChatAction.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef CHATACTION_HPP
#define CHATACTION_HPP

#include "nHeaders.hpp"
#include "ChatAction.hpp"

namespace nEngine {
	/**
		Chat action types
	*/
    enum ChatActionType {
        CHAT_ACTION_SCRIPT,
        CHAT_ACTION_NEXT
    };
	

    class ChatMessage;
    class Chat;

    /**
        Chat action
    */
    class ChatAction {
	public:
        ChatAction();
		~ChatAction();

		void setChatMessage(ChatMessage* message)
		{
			mMessage = message;
		}

		void setScript(Chat* script)
		{
			mScript = script;
		}
		
		void setType(ChatActionType type)
		{
			mType = type;
		}

		ChatActionType getType()
		{
			return mType;
		}

		void setText(const std::string& text)
		{
			mText = text;
		}

		std::string getText()
		{
			return mText;
		}
	private:
		/// Type of the action
        ChatActionType mType;

		/// Lua function ref
		int luaFunctionRef;

		/// ID of the next chat
        int mNextChat;

		/// Name of the lua function
        std::string luaFunctionName;

		/// Text of the action
        std::string mText;

		/// Message it is attached to
        ChatMessage* mMessage;

		/// Script it is attached to
        Chat* mScript;
    };
};

#endif /* CHATACTION_HPP */