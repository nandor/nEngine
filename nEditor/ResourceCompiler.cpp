/**\
	@file ResourceCompiler.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEngine/nHeaders.hpp"
#include "ResourceCompiler.hpp"
using namespace boost::property_tree;

DEFINE_EVENT_TYPE(wxEVT_COMPILE)

// ------------------------------------------------------------------
ResourceCompiler::ResourceCompiler(wxWindow* wnd, nProj* proj)
	:wxThread(wxTHREAD_DETACHED),
	 mData(proj->getData()),
	 mOutDir(proj->getDir()),
	 mNumCompiled(0)
{
	mRoot = wnd;

	if (Create() == wxTHREAD_NO_ERROR) {
		wxThread::Run();
    }
}
	
// ------------------------------------------------------------------
ResourceCompiler::~ResourceCompiler()
{

}

// ------------------------------------------------------------------
wxThread::ExitCode ResourceCompiler::Entry()
{

	try {
		// Count the number of resources to compile
		mResourceCount = countResources();
		if (mResourceCount <= 0) {
			sendMessage(100, "Done: Compiled 0 resources!");
			return NULL;
		}
	
		// Start the compile process
		if (compile()) {
			sendMessage(100, "Done: Compiled " + boost::lexical_cast<std::string> (mResourceCount) + " resources!");
		}
	} catch (...) {
		sendMessage(100, "Compile failed!");
	}
	
	return NULL;
}

// ------------------------------------------------------------------
bool ResourceCompiler::compile()
{
	nEngine::File::setenv("projdir", mOutDir);
	BOOST_FOREACH(ptree::value_type& packages, mData.get_child("resources")) {
		if (!compilePackage(packages.first, packages.second)) {
			return false;
		}
	}	

	return true;
}

// ------------------------------------------------------------------
bool ResourceCompiler::compilePackage(const std::string& name, boost::property_tree::ptree& packData)
{
	mPackageName = name;
	mPackage = zipOpen((mOutDir + "/build/" + name + ".zip").c_str(), APPEND_STATUS_CREATE);
	mHeader.clear();

	mHeader.put_child("lua", ptree());
	mHeader.put_child("map", ptree());

	BOOST_FOREACH(ptree::value_type& resNode, packData) {	
		std::string type(resNode.second.get<std::string> ("type"));
		sendMessage((mNumCompiled++) / mResourceCount * 100.0f, "Compiled: " + resNode.first);
	
		if (type == "LUA") {
			if (!compileLua(resNode.second.get<std::string> ("file"))) {
				return false;
			}
			continue;
		}

		if (type == "MAP") {
			if (!compileMap(resNode.first, resNode.second)) {
				return false;
			}
			continue;
		}
	}

	// Add the header
	if (!compileHeader()) {
		return false;
	}

	zipClose(mPackage, "Created with nEditor");
	return true;
}

// ------------------------------------------------------------------
bool ResourceCompiler::compileLua(const std::string& input)
{
	std::string fileName(input);
	nEngine::File* f = nEngine::Resources::inst().require<nEngine::File> (fileName);
		
	// Add the file to the archive
	if (!writeFile(nEngine::File::strip(fileName), f->getData(), f->getSize())) {
		return false;
	}

	// Add the file to the header
	mHeader.get_child("lua").put_value("%" + mPackageName + "%/" + nEngine::File::strip(fileName));
	return true;
}

// ------------------------------------------------------------------
bool ResourceCompiler::compileMap(const std::string& id, boost::property_tree::ptree& data)
{
	std::string mapFileName(nEngine::File::strip(data.get<std::string> ("mapData")));
	nEngine::File* mapData = nEngine::Resources::inst().require<nEngine::File>(data.get<std::string> ("mapData"));

	ptree mapHeader, fields;
	
	mapHeader.put("size", data.get<std::string>("size"));
	mapHeader.put("namespace", data.get<std::string> ("namespace"));
	mapHeader.put("mapData", mapFileName);


	BOOST_FOREACH(ptree::value_type& v, data.get_child("fields")) {
		ptree field;
		std::string imageName(v.second.get<std::string> ("image"));

		nEngine::File* f = nEngine::Resources::inst().require<nEngine::File> (imageName);
		
		writeFile(nEngine::File::strip(imageName), f->getData(), f->getSize());

		field.put("name", v.second.get<std::string> ("name"));
		field.put("image", "%" + mPackageName + "%/" + nEngine::File::strip(imageName));
		fields.put_child(v.first, field);
	}

	mapHeader.put_child("fields", fields);
	mHeader.put_child("map." + id, mapHeader);

	return writeFile(mapFileName.c_str(), mapData->getData(), mapData->getSize());
}
// ------------------------------------------------------------------
bool ResourceCompiler::compileHeader()
{
	std::stringstream ss;
	write_json(ss, mHeader);
	std::string headerData = ss.str();

	return writeFile("header", headerData.c_str(), headerData.length());
}

// ------------------------------------------------------------------
bool ResourceCompiler::writeFile(const std::string fileName, const void* data, unsigned length)
{
	if (zipOpenNewFileInZip(mPackage, fileName.c_str(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != ZIP_OK ||
		zipWriteInFileInZip(mPackage, data, length) != ZIP_OK ||
		zipCloseFileInZip(mPackage) != ZIP_OK) 
	{
		return false;
	}

	return true;
}

// ------------------------------------------------------------------
void ResourceCompiler::sendMessage(int progress, const std::string& msg)
{
	wxCommandEvent evt(wxEVT_COMPILE, wxID_ANY);

	evt.SetInt(progress);
	evt.SetString(msg);	

	mRoot->AddPendingEvent(evt);
}

// ------------------------------------------------------------------
int ResourceCompiler::countResources()
{
	boost::optional<ptree&> groups = mData.get_child_optional("resources");
	
	if (!groups.is_initialized()) {
		return 0;
	}

	int count = 0;
	BOOST_FOREACH(ptree::value_type& groupNode, groups.get()) {
		BOOST_FOREACH(ptree::value_type& resNode, groupNode.second) {
			++count;
		}
	}

	return count;
}