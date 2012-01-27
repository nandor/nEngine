/**
	@file nProj.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef NPROJ_HPP
#define NPROJ_HPP

#include <fstream>

#include "FileEditor.hpp"


class nProj {
public:
	nProj(const std::string& str, const std::string& dir);
	~nProj();
	
	void save();

	std::string getName()
	{
		return mName;
	}

	std::string getDir()
	{
		return mDir;
	}

	boost::property_tree::ptree& getData()
	{
		return mData;
	}

	std::string getInitScript()
	{
		return mInitScript;
	}

	std::vector<std::pair<std::string, std::string> > getResourceGroups();

private:

	boost::property_tree::ptree mData;
	std::string mProjFile;
	std::string mName;
	std::string mDir;
	std::string mInitScript;
};

#endif /*NPROJ_HPP*/