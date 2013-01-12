/**
	@file nProj.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEditor.hpp"
#include "nEngine/Resources.hpp"
#include "nEngine/Error.hpp"
#include "nEngine/Map.hpp"

#include "nProj.hpp"
using namespace boost::property_tree;
namespace fs = boost::filesystem3;

// ------------------------------------------------------------------
nProj::nProj(const std::string& dir)
{
	fs::path dirPath(dir);

	if (!fs::is_directory(dirPath)) {
		throw nEngine::Error("Invalid project location!");
	}

	mName = dirPath.leaf().string();
	mDir = dir;

	fs::create_directory(dirPath / "src");
	nEngine::File::setenv("path", dir + "/src");

	boost::property_tree::ptree config;
	config.put("name", mName);

	// Create source & build dir
	fs::create_directory(dirPath / "src/");
	fs::create_directory(dirPath / "build/");

	// Create an init script
	mInitScript = (dirPath / "src/init.lua").string();

	std::fstream fout(mInitScript, std::ios::out);

	fout << "--[[" << std::endl
		 << "\t" << mName << " init script " << std::endl
		 << "]]--" << std::endl;

	fout.close();

	nEngine::File* f = new nEngine::File("init", mInitScript);
	f->setEditable(true);
	nEngine::Resources::inst().addResource(f);
	config.put("init", mInitScript);

	// Write the project file
	boost::property_tree::write_json((dirPath / (mName + ".json")).string(), config);
}

// ------------------------------------------------------------------
nProj::nProj(const std::string& file, const std::string& dir)
{
	mName = file;
	mDir = dir;
	
	nEngine::File::setenv("path", dir + "/src");
	
	mProjFile = file;
	try {
		read_json(file, mData);
		
		mInitScript = mData.get<std::string> ("init");
		mProjName = mData.get<std::string> ("name");

		nEngine::Resources::inst().clearResources();

		nEngine::File* f = new nEngine::File("init", mInitScript);
		f->setEditable(true);
		nEngine::Resources::inst().addResource(f);

		boost::optional<ptree&> packs = mData.get_child_optional("packs");

		if (packs.is_initialized()) {
			// parse external resources
			BOOST_FOREACH(ptree::value_type& v, packs.get()) {
				nEngine::Resources::inst().loadGroup(v.second.get_value<std::string>());
			}
		}

		// parse new packages
		boost::optional<ptree&> resources = mData.get_child_optional("resources");

		if (resources.is_initialized()) {
			// each resource pack
			BOOST_FOREACH(ptree::value_type& pack, resources.get()) {
				std::string package(pack.first);

				nEngine::ResourceGroup* group = nEngine::Resources::inst().createGroup(package);
				group->setEditable(true);

				BOOST_FOREACH(ptree::value_type& res, pack.second) {
					std::string type(res.second.get("type", "NONE"));
					
					if (type == "LUA") {
						nEngine::Resources::inst().addResource(new nEngine::File(
							res.first, 
							res.second.get<std::string> ("file")
						), package);
						continue;
					}

					if (type == "MAP") {
						nEngine::Map* mp = new nEngine::Map(res.first);
						mp->loadData(res.second);
						nEngine::Resources::inst().addResource(mp, package);
						continue;
					}
				}
			}
		}
	} catch (boost::property_tree::ptree_error except) {
		throw nEngine::Error("nProj", file, "[json]" + std::string(except.what()));
	}
}

// ------------------------------------------------------------------
nProj::~nProj()
{

}

// ------------------------------------------------------------------
void nProj::save()
{
	write_json(mProjFile, mData);
}
