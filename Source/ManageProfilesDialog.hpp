#pragma once

#include "ModManager.hpp"
#include "ProfileConfig.hpp"

class ApplicationConfig;

class ManageProfilesDialog : public WxfbManageProfilesDialog
{
public:
	ManageProfilesDialog(wxWindow* parent, ApplicationConfig& applicationConfig, const ModManager& modManager);
	const std::string& getProfileModPath() const;

private:
	void onRefresh(wxCommandEvent& event);
	void importProfileButtonOnButtonClick(wxCommandEvent& event) override;
	void exportProfileButtonOnButtonClick(wxCommandEvent& event) override;
	void addListItems();
	void scheduleRefresh();

	const ApplicationConfig& config_;
	const ModManager& modManager_;

	ModManager importModManager_;
	ProfileConfig profileConfig_;
	int queuedRefreshEvents_;
	wxImageList imageList_;
};