/**
	@file ChatBox.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef CHATBOX_HPP
#define CHATBOX_HPP

#include "types.hpp"
#include "ChatAction.hpp"
#include "ChatMessage.hpp"
#include "Chat.hpp"
#include "Singleton.hpp"
#include "GUI.hpp"

namespace nEngine {
	class ChatBox : public Singleton<ChatBox> {
	public:
		NAPI ChatBox();
		NAPI ~ChatBox();

		NAPI void setChatMessage(ChatMessage* msg);
		NAPI void hide();

	private:
		ChatMessage* myMessage;

		GUILabel* lbl;
		GUIPanel* panel;
	};
};

#endif /*CHATBOX_HPP*/
