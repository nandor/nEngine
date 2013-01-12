/**
	@file ResourceCompiler.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef RESOURCECOMPILER_HPP
#define RESOURCECOMPILER_HPP

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/progdlg.h>

#include "nProj.hpp"
#include "nEngine/File.hpp"
#include "minizip/zip.h"

DECLARE_EVENT_TYPE(wxEVT_COMPILE, -1)

class ResourceCompiler : public wxThread {
public:
	/**
		Creates a new compiler thread
		@param wnd			Pointer to parent window
		@param proj			Pointer to project
	*/
	ResourceCompiler(wxWindow* wnd, nProj* proj);

	/**
		Clear the compiler
	*/
	~ResourceCompiler();
	
private:

	wxThread::ExitCode Entry();
	void sendMessage(int progress, const std::string& msg);

private:

	int countResources();

	bool compile();
	bool compilePackage(const std::string& name, boost::property_tree::ptree& packData);
	bool compileHeader();

	bool compileLua(const std::string& input, const std::string& output);
	bool compileMap(const std::string& id, boost::property_tree::ptree& data);

	bool writeFile(const std::string fileName, const void* data, unsigned length);
	bool writeInitScript();

private:

	int mResourceCount;
	int mNumCompiled;
	wxWindow* mRoot;
	
	boost::property_tree::ptree& mData;
	std::string mOutDir;

	/// Package dependent
	std::string mPackageName;
	zipFile mPackage;
	boost::property_tree::ptree mHeader;
};

#endif /*RESOURCECOMPILER_HPP*/