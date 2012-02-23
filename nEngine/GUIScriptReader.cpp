/**
	@file GUIScriptReader.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2012 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Vec2.hpp"
#include "GUIButton.hpp"
#include "GUILabel.hpp"
#include "GUICheckbox.hpp"
#include "GUIComboBox.hpp"
#include "GUIPanel.hpp"
#include "GUISlider.hpp"
#include "GUIStyledText.hpp"
#include "GUIScriptReader.hpp"

using namespace boost::property_tree;

namespace nEngine {
	// regex
	std::regex gSizeRegex("[0-9]+(%|px)");

	// ------------------------------------------------------------------
	GUIScriptReader::GUIScriptReader(DataSource* source, GUIElement* dest)
	{
		if (mDest == NULL) {
			throw Error("GUIScriptReader", "Invalid target!");
		}

		if (mDataSource == NULL) {
			throw Error("GUIScriptReader", "Invalid data source!");
		}

		mDataSource = source;
		mDest = dest;
	}

	// ------------------------------------------------------------------
	GUIScriptReader::~GUIScriptReader()
	{

	}
	
	// ------------------------------------------------------------------
	void GUIScriptReader::read()
	{
		try {
			parseList(mDataSource->getRoot(), mDest);
		} catch (boost::property_tree::ptree_error& err) {
			throw Error("GUIScriptReader", mDataSource->getID(), err.what());
		}
	}
	
	// ------------------------------------------------------------------
	void GUIScriptReader::parseList(ptree& node, GUIElement* dest)
	{
		BOOST_FOREACH(ptree::value_type& v, node) {
			GUIElement* newElem = parseNode(v.first, v.second);
			if (newElem != NULL) {
				dest->add(newElem);
			}
		}
	}
	
	// ------------------------------------------------------------------
	GUIElement* GUIScriptReader::parseNode(const std::string& id, ptree& node)
	{
		std::string type(node.get<std::string>("type"));

		GUIElement* elem = NULL;

		if (type == "button") {
			GUIButton* button = new GUIButton(id);
			button->setCaption(node.get("caption", id));

			elem = button;
		} else if (type == "label") {
			GUILabel* label = new GUILabel(id);
			label->setText(node.get("text", ""));

			elem = label;
		} else if (type == "checkbox") {		
			GUICheckbox* chkbox = new GUICheckbox(id);
			chkbox->setCaption(node.get("caption", id));

			elem = chkbox;
		} else if (type == "panel") {
			elem = new GUIPanel(id);
		} else if (type == "slider") {
			GUISlider* sld = new GUISlider(id);
			sld->setValue(node.get("value", 0));
			elem = sld;
		} else if (type == "combo") {
			GUIComboBox* box = new GUIComboBox(id);

			elem = box;
		} else if (type == "styledText") {
			elem = new GUIStyledText(id);
		} else {
			throw Error("GUIScriptReader", mDataSource->getID(),  "Invalid type: " + type);
		}

		if (elem == NULL) {
			return NULL;
		}

		elem->setFont(node.get("font", "mono8"));
			
		elem->setSize(
			GUIMetric(node.get("size.width", "0px")),
			GUIMetric(node.get("size.height", "0px"))
		);

		elem->setPosition(
			GUIMetric(node.get("pos.x", "0px")),
			GUIMetric(node.get("pos.y", "0px"))
		);

		elem->setAlignment(
			getAlignment(node.get("align.horz", "none")),
			getAlignment(node.get("align.vert", "none"))
		);		

		elem->setTextColor(Color(
			node.get("textColor.r", 0.0f),
			node.get("textColor.g", 0.0f),
			node.get("textColor.b", 0.0f),
			node.get("textColor.a", 1.0f)
		));

		elem->setBackgroundColor(Color(
			node.get("backgroundColor.r", 0.0f),
			node.get("backgroundColor.g", 0.0f),
			node.get("backgroundColor.b", 0.0f),
			node.get("backgroundColor.a", 1.0f)
		));

		elem->setVisibility(node.get("visibility", true));

		boost::optional<ptree&> children = node.get_child_optional("children");
		if (children.is_initialized()) {
			parseList(children.get(), elem);
		}

		return elem;
	}
};