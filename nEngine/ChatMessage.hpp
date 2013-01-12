/**
	@file ChatMessage.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef CHATMESSAGE_HPP
#define CHATMESSAGE_HPP

#include "types.hpp"
#include "ChatAction.hpp"
#include "Error.hpp"

namespace nEngine {
	/**
		Chat message types
	*/
    enum ChatMessageType {
        CHAT_PLAYER,
        CHAT_NPC,
        CHAT_MESSAGE,
        numChatMessageType
    };
	
    /**
        A single message in a chat
    */
    class ChatMessage {
    public:
		/**
			Create a new chat message
		*/
        ChatMessage ();

		/**
			Destroys the chat message
		*/
        ~ChatMessage();

        /**
            Get the message
            @return         Message
        */
        std::string getMessage() 
		{
            return mMessage;
        }

        /**
            Set the new message
            @param message New message
        */
        void setMessage(const std::string& message) 
		{
			mMessage = message;
        }

        /**
            Return the id of the message
            @return         ID
        */
        std::string getID() {
            return mID;
        }

        /**
            Set the id of the message
            @param id      New id
        */
        void setID(const std::string& id) {
            mID = id;
        }

        /**
            Get the type of the message
            @return         Type of the message
        */
        ChatMessageType getType() {
            return mType;
        }

        /**
            Set the type of the message
            @param _type    New type
        */
        void setType(ChatMessageType type) {
            mType = type;
        }

        /**
            Return the npc id
            @return         NPC id
        */
        std::string getNPCID() {
            return mNpcID;
        }

        /**
            Set a new NPC id
            @return npc  New npc id
        */
        void setNPCID(const std::string& npc) {
            mNpcID = npc;
        }

		/**
			Create a new action
			@param tmessage			Message of the action
			@param type				Type of the action
			@param action			Action string
		*/
        void addAction(const std::string& message, ChatActionType type, const std::string& action);

		/**
			Remove an action
			@param id				ID of the action
		*/
        void removeAction(unsigned id);

		/**
			Get an action
			@param id				ID of the action
			@return					Reference to the action
		*/
        ChatAction& getAction(unsigned id);

		/**
			Get the number of actions
			@return					Number of actions
		*/
        int getNumActions();
    private:
        /** Message displayed in the chatbox */
        std::string mMessage;

        /** ID of the message */
        std::string mID;

        /** ID of the NPC */
        std::string mNpcID;

        /** Type of the message */
        ChatMessageType mType;

        /** Possible actions */
        std::vector<ChatAction> mActions;
		
		// Forward declaration
        class Chat;

        /** Parent script */
        Chat* script;
    };
};

#endif /*CHATMESSAGE_HPP*/