/**
	@file ChatBox.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef CHATBOX_HPP
#define CHATBOX_HPP

#include "types.hpp"
#include "Singleton.hpp"

namespace nEngine {
	// Forward declarations
	class ChatMessage;
	class GUILabel;
	class GUIPanel;

	/**
		Display a chatbox
	*/
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
