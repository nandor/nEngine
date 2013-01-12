/**
	@file nChatEditor.hp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef CHATSCRIPT_HPP
#define CHATSCRIPT_HPP

#include "Resource.hpp"
#include "Resources.hpp"
#include "ChatMessage.hpp"
#include "Error.hpp"

namespace nEngine {

    /**
        Chat script storage and manipulation
    */
    class Chat : public Resource {
    public:
		/**
			Create a new chat
			@param id		ID of the chat
		*/
        Chat(const std::string& id);
		
		/**
			Destroy a chat
		*/
        ~Chat();

		/**
			Draw the chat box
		*/
		void drawChatBox();

		/**
			Get a message
			@param id		ID of the message
			@return			Message
		*/
        ChatMessage& getMessage(const std::string& id);

		/**
			Return all the message ids
			@return			vector containing ids
		*/
		std::vector<std::string> getMessages();

    private:

		/// Iterator to the message map
        typedef std::map<std::string, ChatMessage>::iterator tMessageIter;

		/// Map containing messages
        std::map<std::string, ChatMessage> mMessages;

	public:
		/// Type of the resource
		const static ResourceType sType = RESOURCE_CHAT;

		/// Register the 'Chat' lua object
		static void luaRegister(lua_State* L);
    };
};

#endif /* CHATSCRIPT_HPP */
