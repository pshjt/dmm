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
	virtual void onSize(wxSizeEvent& event);
	void onMove(wxMoveEvent& event);
	void onRefresh(wxCommandEvent& event);

	virtual void pathTextCtrlOnText(wxCommandEvent& event);
	virtual void genericDirCtrlOnTreeSelChanged(wxTreeEvent& event);

	void scheduleRefresh();

	const ApplicationConfig& config_;
	std::string gameFolderPath_;

	int queuedRefreshEvents_;
};