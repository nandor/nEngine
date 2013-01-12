/**
	@file nChatEditor.hp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "Chat.hpp"
#include "File.hpp"
#include "ChatBox.hpp"
using namespace boost::property_tree;

namespace nEngine {	
	// ------------------------------------------------------------------
	Chat::Chat (const std::string& id)
		:Resource(id, RESOURCE_CHAT)
	{
		File* file =  Resources::inst().require<File> (id);

		try {
			std::stringstream ss((char*)file->getData());
			ptree cfg;
			read_json(ss, cfg);

			BOOST_FOREACH(ptree::value_type& v, cfg) {
				ptree data = v.second;

				ChatMessage cMsg;
				cMsg.setID(v.first);

				std::string type = data.get<std::string> ("type");

				if (type == "player") {
					cMsg.setType(CHAT_PLAYER);
					cMsg.setMessage(data.get("message", ""));
				} else if (type == "npc") {
					cMsg.setType(CHAT_NPC);
					cMsg.setNPCID(data.get<std::string> ("who"));
				} else {
					throw Error ("Chat", getID(), "Invalid type: '" + type + "' for '" + v.first + "'");
				}

				BOOST_FOREACH(ptree::value_type& act, data.get_child("actions")) {
					std::string type = act.second.get<std::string> ("type");
					if (type == "lua") {
						cMsg.addAction(
							act.second.get<std::string> ("message"), 
							CHAT_ACTION_SCRIPT, 
							act.second.get<std::string> ("action")
						);
					} else if (type == "next") {						
						cMsg.addAction(
							act.second.get<std::string> ("message"), 
							CHAT_ACTION_NEXT, 
							act.second.get<std::string> ("action")
						);
					} else {
						throw Error ("Chat", getID(), "Invalid action type: '" + type + "' for '" + v.first + "'");
					}
				}

				mMessages[v.first] = cMsg;
			}
		} catch (ptree_error err) {
			throw Error ("Chat", getID(), err.what()); 	
		}
	}

	
	// ------------------------------------------------------------------
	Chat::~Chat()
	{

	}

	
	// ------------------------------------------------------------------
	ChatMessage& Chat::getMessage(const std::string& id)
	{
		tMessageIter it = mMessages.find(id);
		return (it == mMessages.end()) ? (mMessages.begin()->second) : it->second;
	}

	
	// ------------------------------------------------------------------
	luaDeclareMethod(Chat, new) 
	{
		std::string id (luaL_checkstring(L, 1));

		luaInstance(L, Chat, Resources::inst().require<Chat>(id));
		return 1;
	}


	// ------------------------------------------------------------------
	luaDeclareMethod(Chat, showMessage)
	{
		Chat* c = *(Chat**)luaGetInstance(L, 1, "Chat");

		std::string id (luaL_checkstring(L, 2));
		ChatBox::inst().setChatMessage(&c->getMessage(id));
	
		return 0;
	}


	// ------------------------------------------------------------------
	luaBeginMeta(Chat)
		luaMethod(Chat, showMessage)
	luaEndMeta()
	

	// ------------------------------------------------------------------
	luaBeginMethods(Chat)
		luaMethod(Chat, new)
	luaEndMethods()


	// ------------------------------------------------------------------
	void Chat::luaRegister(lua_State* L)
	{
		luaClass(L, Chat);
	}
}