/**
	@file GUIPanel.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUIPANEL_HPP
#define GUIPANEL_HPP

#include "Singleton.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	class GUIPanel : public GUIElement {
	public:
		NAPI GUIPanel(const std::string& id);
		NAPI ~GUIPanel();

	protected:

		NAPI void onDraw();

	};
};
#endif /*GUIPANEL_HPP*/