// Copyright 2013-2020 Marvin
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

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
}

void ManageProfilesDialog::importProfileButtonOnButtonClick(wxCommandEvent& event)
{
	wxFileDialog chooseProfileFileLoadDialog(this, _("Load profile from file..."), config_.application.executableFolderPath, "", "SS2BMM profiles (*.ss2bmm)|*.ss2bmm|All files (*.*)|*.", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (chooseProfileFileLoadDialog.ShowModal() == wxID_CANCEL)
		return;
	wxString chosenPath = chooseProfileFileLoadDialog.GetPath();
	profileConfig_.mod_path = "";
	profileConfig_.assignFile(chosenPath.ToStdString());

	if (!profileConfig_.load())
		showMessage(Message::MessageType::ERROR_OK, "Couldn't load mods setup.", profileConfig_.getErrorMessage());

	// load into import mod manager, preview
}

void ManageProfilesDialog::exportProfileButtonOnButtonClick(wxCommandEvent& event)
{
	auto [modPath, baseModPath] = modManager_.constructModPath();

	wxFileDialog chooseProfileFileSaveDialog(this, _("Save profile to file..."), config_.application.executableFolderPath, "", "SS2BMM profiles (*.ss2bmm)|*.ss2bmm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (chooseProfileFileSaveDialog.ShowModal() == wxID_CANCEL)
		return;
	wxString chosenPath = chooseProfileFileSaveDialog.GetPath();

	profileConfig_.mod_path = modPath;
	profileConfig_.assignFile(chosenPath.ToStdString());
	profileConfig_.saveOverwriting("");

	showMessage(Message::MessageType::INFORMATION_OK, "New profile saved to: " + chosenPath.ToStdString());

	scheduleRefresh();
	return;
}

void ManageProfilesDialog::onRefresh(wxCommandEvent& WXUNUSED(event))
{
	--queuedRefreshEvents_;

	if (queuedRefreshEvents_ > 0)
		return;

	queuedRefreshEvents_ = 0;

	Refresh();
	Update();
	profileListBox_->Update();
}

void ManageProfilesDialog::apply() {
	// apply selected profile
	return;
}

void ManageProfilesDialog::scheduleRefresh()
{
	++queuedRefreshEvents_;

	wxCommandEvent* event = new wxCommandEvent(REFRESH_EVENT, GetId());
	GetEventHandler()->QueueEvent(event);
}