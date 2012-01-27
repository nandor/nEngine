/**
	@file ResourceView.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEngine/nHeaders.hpp"
#include "nEngine/Resources.hpp"

#include "ResourceView.hpp"
#include "MainWindow.hpp"

// Icons
#include "Icons/error.xpm"
#include "Icons/file.xpm"
#include "Icons/font.xpm"
#include "Icons/map.xpm"
#include "Icons/object.xpm"
#include "Icons/package.xpm"
#include "Icons/script.xpm"
#include "Icons/particles.xpm"
#include "Icons/json.xpm"
#include "Icons/shader.xpm"
#include "Icons/sound.xpm"

enum ResourceIcon {
	RESOURCE_ICON_PACKAGE = nEngine::RESOURCE_NUM,
	RESOURCE_NUM_ICONS
};

BEGIN_EVENT_TABLE(ResourceView, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(wxID_ANY, ResourceView::OnItemSelect)
	EVT_TREE_ITEM_MENU(wxID_ANY, ResourceView::OnItemMenu)
	EVT_TREE_ITEM_ACTIVATED(wxID_ANY, ResourceView::OnItemActivate)
END_EVENT_TABLE();

// ------------------------------------------------------------------
class TreeItemData : public wxTreeItemData {
public:
	TreeItemData(std::string id, nEngine::ResourceType type)
		:mId(id),
		 mType(type)
	{

	}
	
	std::string getID() {
		return mId;
	}

	nEngine::ResourceType getType()
	{
		return mType;
	}
	
private:

	std::string mId;
	nEngine::ResourceType mType;
};

// ------------------------------------------------------------------
ResourceView::ResourceView(wxWindow* parent, wxSize& size, wxWindow* root)
	:wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, size, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT)
{
	mRoot = root;
	mIconSize = 16;
	mRootID = this->AddRoot(wxT("Root"));

	CreateIcons();
	Refresh();
}


// ------------------------------------------------------------------
ResourceView::~ResourceView()
{

}

// ------------------------------------------------------------------
void ResourceView::CreateIcons()
{
	wxImageList* images = new wxImageList(mIconSize, mIconSize, true);

	wxIcon icons[RESOURCE_NUM_ICONS];
	
	icons[nEngine::RESOURCE_NONE] = wxIcon(error_xpm);
	icons[nEngine::RESOURCE_FILE] = wxIcon(file_xpm);
	icons[nEngine::RESOURCE_IMAGE] = wxIcon(error_xpm);
	icons[nEngine::RESOURCE_FONT] = wxIcon(font_xpm);
	icons[nEngine::RESOURCE_SHADER] = wxIcon(shader_xpm);
	icons[nEngine::RESOURCE_CHAT] = wxIcon(error_xpm);
	icons[nEngine::RESOURCE_MAP] = wxIcon(map_xpm);
	icons[nEngine::RESOURCE_ARCHIVE] = wxIcon(error_xpm);
	icons[nEngine::RESOURCE_OBJECT] = wxIcon(object_xpm);
	icons[nEngine::RESOURCE_NPC] = wxIcon(error_xpm);
	icons[nEngine::RESOURCE_JSON] = wxIcon(json_xpm);
	icons[nEngine::RESOURCE_SOUND] = wxIcon(sound_xpm);
	icons[RESOURCE_ICON_PACKAGE] = wxIcon(package_xpm);

	for (unsigned i = 0; i < WXSIZEOF(icons); ++i) {
		images->Add(icons[i]);
	}
	
	this->AssignImageList(images);
}

// ------------------------------------------------------------------
void ResourceView::Refresh()
{
	this->DeleteChildren(mRootID);

	this->AppendItem(mRootID, "init", nEngine::RESOURCE_FILE, nEngine::RESOURCE_FILE, new TreeItemData("init", nEngine::RESOURCE_FILE));

	std::vector<std::string> resourceGroups = nEngine::Resources::inst().getResourceGroupNames();
	for (unsigned i = 0; i < resourceGroups.size(); ++i) {
		wxTreeItemId group = this->AppendItem(mRootID, resourceGroups[i], RESOURCE_ICON_PACKAGE, RESOURCE_ICON_PACKAGE);

		std::vector<std::pair<std::string, nEngine::ResourceType> >::iterator it;
		std::vector<std::pair<std::string, nEngine::ResourceType> > items = nEngine::Resources::inst().getGroup(resourceGroups[i])->getResources();
		
		for (it = items.begin(); it != items.end(); ++it) {
			wxTreeItemId itm = this->AppendItem(group, it->first, it->second, it->second, new TreeItemData(it->first, it->second));

			switch (it->second) {
			case nEngine::RESOURCE_SHADER: {
				nEngine::Shader* shader = nEngine::Resources::inst().get<nEngine::Shader> (it->first);
				std::vector<std::string> shaders = shader->getShaderNames();

				for (std::vector<std::string>::iterator it = shaders.begin(); it != shaders.end(); ++it) {
					this->AppendItem(itm, *it, nEngine::RESOURCE_FILE, nEngine::RESOURCE_FILE, new TreeItemData(*it, nEngine::RESOURCE_FILE));
				}

				break;
			}
			default:
				break;
			}
		}
	}

	wxTreeCtrl::Refresh();
}


// ------------------------------------------------------------------
void ResourceView::OnItemMenu(wxTreeEvent& evt)
{
	wxTreeItemId item = evt.GetItem();
	if (!item.IsOk() || GetItemData(item) == NULL) {
		return;
	}

	TreeItemData* data = (TreeItemData*)this->GetItemData(item);
	
	evt.Skip();
}

// ------------------------------------------------------------------
void ResourceView::OnItemSelect(wxTreeEvent& evt)
{
	wxTreeItemId item = evt.GetItem();
	if (!item.IsOk() || GetItemData(item) == NULL) {
		return;
	}

	TreeItemData* data = (TreeItemData*)this->GetItemData(item);
	
	MainWindow* wnd = (MainWindow*)mRoot;
	wnd->OnResourceSelected(std::pair<std::string, nEngine::ResourceType> (data->getID(), data->getType()));
}

// ------------------------------------------------------------------
void ResourceView::OnItemActivate(wxTreeEvent& evt)
{
	wxTreeItemId item = evt.GetItem();
	if (!item.IsOk() || GetItemData(item) == NULL) {
		return;
	}

	TreeItemData* data = (TreeItemData*)this->GetItemData(item);
	MainWindow* wnd = (MainWindow*)mRoot;

	if (data->getType() == nEngine::RESOURCE_FILE) {
		wnd->OpenFileEditor(data->getID());
	}
}