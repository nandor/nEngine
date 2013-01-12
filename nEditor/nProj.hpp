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

	/**
		Create a new project
		@param dir			Location
	*/
	nProj(const std::string& dir);

	/**
		Open an existing project
		@param str			Location of the project file
		@param dir			Directory of the project
	*/
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

	std::string getProjName()
	{
		return mProjName;
	}

	std::vector<std::pair<std::string, std::string> > getResourceGroups();

private:

	boost::property_tree::ptree mData;
	std::string mProjFile;
	std::string mName;
	std::string mDir;
	std::string mInitScript;
	std::string mProjName;
};

#endif /*NPROJ_HPP*/