// Copyright 2013-2020 Marvin
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "CustomEvent.hpp"
#include "Utils.hpp"

#include "ManageProfilesDialog.hpp"

ManageProfilesDialog::ManageProfilesDialog(wxWindow* parent, const ApplicationConfig& applicationConfig)
	: WxfbManageProfilesDialog(parent)
	, config_(applicationConfig)
	, queuedRefreshEvents_(0)
{
	Connect(REFRESH_EVENT, wxCommandEventHandler(ManageProfilesDialog::onRefresh));
}

void ManageProfilesDialog::importProfileButtonOnButtonClick(wxCommandEvent& event)
{
	return;
}

void ManageProfilesDialog::exportProfileButtonOnButtonClick(wxCommandEvent& event)
{
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