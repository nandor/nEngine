/**
	@file GUIScriptReader.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2012 Licker Nandor
*/

#ifndef GUISCRIPTREADER_HPP
#define GUISCRIPTREADER_HPP
#pragma once

#include "Vec2.hpp"
#include "DataSource.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	/**
		Reads a gui script, creates elements and puts them
		into dest
	*/
	class GUIScriptReader {
	public:
		GUIScriptReader(DataSource* source, GUIElement* dest);
		~GUIScriptReader();

		void read();

	private:

		void parseList(boost::property_tree::ptree& list, GUIElement* dest);

		GUIElement* parseNode(const std::string& id, boost::property_tree::ptree& node);

	private:

		DataSource* mDataSource;
		GUIElement* mDest;
	};
};


#endif /*GUISCRIPTREADER_HPP*/