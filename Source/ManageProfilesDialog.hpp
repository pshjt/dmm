// Copyright 2013-2020 Marvin
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "ApplicationConfig.hpp"
#include "ProfileConfig.hpp"

class ManageProfilesDialog : public WxfbManageProfilesDialog
{
public:
	ManageProfilesDialog(wxWindow* parent, const ApplicationConfig& applicationConfig);
	void apply();

private:
	void onRefresh(wxCommandEvent& event);
	void importProfileButtonOnButtonClick(wxCommandEvent& event) override;
	void exportProfileButtonOnButtonClick(wxCommandEvent& event) override;

	void scheduleRefresh();
	const ApplicationConfig& config_;

	int queuedRefreshEvents_;
};