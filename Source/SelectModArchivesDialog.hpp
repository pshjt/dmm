// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "ApplicationConfig.hpp"

class SelectModArchivesDialog : public WxfbSelectModArchivesDialog
{
	public:
		SelectModArchivesDialog(wxWindow* parent, const ApplicationConfig& applicationConfig);

		const std::vector<std::string>& getArchiveFilenames() const;

		~SelectModArchivesDialog()
		{
			lib7z.Free();
		}

	private:
		virtual void onSize(wxSizeEvent& event);
		void onMove(wxMoveEvent& event);
		void onRefresh(wxCommandEvent& event);

		virtual void genericFileCtrlOnFileSelectionChanged(wxTreeEvent& event);

		void scheduleRefresh();

		std::vector<std::string> archiveFilenames_;
		const ApplicationConfig& config_;
		SevenZip::SevenZipLibrary lib7z;

		int queuedRefreshEvents_;
};
