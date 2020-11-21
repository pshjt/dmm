// Copyright 2013-2020 Marvin
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

// why?
#include "ModManager.hpp"
#include "ProfileConfig.hpp"

class ApplicationConfig;
class ModManager;

class ManageProfilesDialog : public WxfbManageProfilesDialog
{
public:
	ManageProfilesDialog(wxWindow* parent, ApplicationConfig& applicationConfig, const ModManager& modManager);
	void apply();

private:
	void onRefresh(wxCommandEvent& event);
	void importProfileButtonOnButtonClick(wxCommandEvent& event) override;
	void exportProfileButtonOnButtonClick(wxCommandEvent& event) override;
	void scheduleRefresh();

	const ApplicationConfig& config_;
	const ModManager& modManager_;

	ModManager importModManager_;
	ProfileConfig profileConfig_;
	int queuedRefreshEvents_;
};