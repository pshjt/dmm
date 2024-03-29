#include <Precompiled.hpp>

#include "CustomEvent.hpp"
#include "Message.hpp"
#include "ModManager.hpp"
#include "Utils.hpp"

#include "ManageProfilesDialog.hpp"

ManageProfilesDialog::ManageProfilesDialog(wxWindow* parent, ApplicationConfig& applicationConfig, const ModManager& modManager)
	: WxfbManageProfilesDialog(parent)
	, config_(applicationConfig)
	, queuedRefreshEvents_(0)
	, modManager_(modManager)
	, importModManager_(applicationConfig)
{
	Connect(REFRESH_EVENT, wxCommandEventHandler(ManageProfilesDialog::onRefresh));

	profileConfig_.mod_path = "";

	profileListBox_->ClearAll();
	profileListBox_->InsertColumn(0, "Imported profile will be shown here when loaded.", wxLIST_FORMAT_CENTER);

	this->SetEscapeId(wxID_CANCEL);

	scheduleRefresh();
}

const std::string& ManageProfilesDialog::getProfileModPath() const
{
	return profileConfig_.mod_path;
}

void ManageProfilesDialog::importProfileButtonOnButtonClick(wxCommandEvent& event)
{
	wxFileDialog chooseProfileFileLoadDialog(this, "Load profile from file...", config_.application.executableFolderPath, "", "DMM profiles (*.dmm)|*.dmm|All files (*.*)|*.", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (chooseProfileFileLoadDialog.ShowModal() == wxID_CANCEL)
		return;
	std::string chosenPath = chooseProfileFileLoadDialog.GetPath();
	profileConfig_.assignFile(chosenPath);

	if (!profileConfig_.load())
		showMessage(Message::MessageType::ERROR_OK, "Couldn't load profile.", profileConfig_.getErrorMessage());

	// initialize temporary import mod manager and print out expected import result
	importModManager_.initialize(config_.game.folderPath, false, true);
	importModManager_.resetStates();
	importModManager_.loadModsConfig(false, profileConfig_.mod_path);
	addListItems();

	scheduleRefresh();

	event.Skip();
}

void ManageProfilesDialog::exportProfileButtonOnButtonClick(wxCommandEvent& event)
{
	auto [modsPathPrefix, modPath, modsPathSuffix] = modManager_.constructModPath();

	wxFileDialog chooseProfileFileSaveDialog(this, "Save profile to file...", config_.application.executableFolderPath, "", "DMM profiles (*.dmm)|*.dmm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (chooseProfileFileSaveDialog.ShowModal() == wxID_CANCEL)
		return;
	std::string chosenPath = chooseProfileFileSaveDialog.GetPath();

	profileConfig_.mod_path = modPath;
	profileConfig_.assignFile(chosenPath);
	profileConfig_.saveOverwriting("");

	showMessage(Message::MessageType::INFORMATION_OK, "New profile saved to: " + chosenPath);

	profileConfig_.mod_path = "";
	scheduleRefresh();

	event.Skip();

	return;
}

void ManageProfilesDialog::addListItems()
{
	profileListBox_->Freeze();
	profileListBox_->ClearAll();
	profileListBox_->InsertColumn(0, "#", wxLIST_FORMAT_RIGHT);
	profileListBox_->InsertColumn(1, "Name");
	profileListBox_->InsertColumn(2, "Status");
	profileListBox_->InsertColumn(3, "Ready", wxLIST_FORMAT_CENTER);

	int itemCount = profileListBox_->GetItemCount() - 1;
	for (auto& mod : importModManager_.getMods())
	{
		bool isActive = mod.getIsActive();
		bool isPaused = mod.getIsPaused();
		bool canBeLoaded = modManager_.hasModWithName(mod.getName());
		if (!(isActive || isPaused) && canBeLoaded)
		{
			// inactive (but activatable) mods need not be shown, they will always be loaded in inactive state no matter what
			continue;
		}

		wxListItem listItem;
		listItem.SetId(++itemCount);
		listItem.SetColumn(0);
		listItem.SetText(wxString() << itemCount + 1);
		listItem.SetImage(-1);

		if (profileListBox_->InsertItem(listItem) == itemCount)
		{
			profileListBox_->SetItem(itemCount, 1, mod.getName());

			if (canBeLoaded)
			{
				profileListBox_->SetItem(itemCount, 2, mod.getIsPaused() ? "Paused" : "Active");
				profileListBox_->SetItem(itemCount, 3, L"\u2714");
			}
			else
			{
				profileListBox_->SetItem(itemCount, 2, "Not found");
				profileListBox_->SetItem(itemCount, 3, L"\u274C");
			}
		}
	}
	profileListBox_->Thaw();
}

void ManageProfilesDialog::onRefresh(wxCommandEvent& WXUNUSED(event))
{
	--queuedRefreshEvents_;

	if (queuedRefreshEvents_ > 0)
		return;

	Refresh();
	Update();
	profileListBox_->Update();

	// adjust column width in automated fashion
	for (int columnIndex = 0; columnIndex < profileListBox_->GetColumnCount(); columnIndex++)
	{
		profileListBox_->SetColumnWidth(columnIndex, wxLIST_AUTOSIZE_USEHEADER);
		int wh = profileListBox_->GetColumnWidth(columnIndex);
		profileListBox_->SetColumnWidth(columnIndex, wxLIST_AUTOSIZE);
		int wc = profileListBox_->GetColumnWidth(columnIndex);
		if (wh > wc)
			profileListBox_->SetColumnWidth(columnIndex, wxLIST_AUTOSIZE_USEHEADER);
	}

	bool profileLoaded = !getProfileModPath().empty();
	profileListBox_->Enable(profileLoaded);
	useProfileButton_->Enable(profileLoaded);
}

void ManageProfilesDialog::scheduleRefresh()
{
	++queuedRefreshEvents_;

	wxCommandEvent* event = new wxCommandEvent(REFRESH_EVENT, GetId());
	GetEventHandler()->QueueEvent(event);
}