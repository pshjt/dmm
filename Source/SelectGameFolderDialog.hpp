// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "ApplicationConfig.hpp"

class SelectGameFolderDialog : public WxfbSelectGameFolderDialog
{
public:
	SelectGameFolderDialog(wxWindow* parent, const ApplicationConfig& applicationConfig);

	const std::string& getGameFolderPath() const;
	const std::string getAlternativeExecutable(std::string) const;

private:
	void onSize(wxSizeEvent& event) override;
	void onMove(wxMoveEvent& event);
	void onRefresh(wxCommandEvent& event);

	void pathTextCtrlOnText(wxCommandEvent& event) override;
	void genericDirCtrlOnTreeSelChanged(wxTreeEvent& event) override;

	void scheduleRefresh();

	const ApplicationConfig& config_;
	std::string gameFolderPath_;

	int queuedRefreshEvents_;
};
