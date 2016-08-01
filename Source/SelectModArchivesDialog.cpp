// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "CustomEvent.hpp"
#include "Utils.hpp"

#include "SelectModArchivesDialog.hpp"

SelectModArchivesDialog::SelectModArchivesDialog(wxWindow* parent, const ApplicationConfig& applicationConfig)
	: WxfbSelectModArchivesDialog(parent)
	, config_(applicationConfig)
	, queuedRefreshEvents_(0)
{
	Connect(wxEVT_MOVE, wxMoveEventHandler(SelectModArchivesDialog::onMove));
	Connect(REFRESH_EVENT, wxCommandEventHandler(SelectModArchivesDialog::onRefresh));


	std::wstring libpath = Utils::toWstring(config_.application.executableFolderPath);

	lib7z.Load(libpath);

	{
		float scale = 0.75f;
		wxSize maxSize = parent->GetSize();

		int maxWidth = static_cast<int>(static_cast<float>(maxSize.GetWidth()) * scale);
		int maxHeight = static_cast<int>(static_cast<float>(maxSize.GetHeight()) * scale);

		Utils::adjustSizeDown(this, maxWidth, maxHeight);
	}

	panel_->SetDoubleBuffered(true);

	genericFileCtrl_->SetEvtHandlerEnabled(false);
	if (wxDirExists(config_.game.folderPath))
		genericFileCtrl_->SetDirectory(config_.game.folderPath + "\\..");
	else
		genericFileCtrl_->SetDirectory(config_.application.executableFolderPath + "\\..");

	wxString wdCardArchives;
	wxString wdCards;
	std::string wdCardTemp;
	for (auto& archiveType : config_.application.modArchiveExtensions)
	{
		wdCardTemp = archiveType;
		Utils::stringToUpperCase(wdCardTemp);
		wdCardArchives = wdCardArchives + wdCardTemp + ", ";
		
		wdCards = wdCards + "*." + archiveType + ";";
	}
	wdCardArchives.RemoveLast();
	wdCardArchives.RemoveLast();
	wdCards.RemoveLast();

	genericFileCtrl_->SetWildcard(wdCardArchives + " files|" + wdCards);
	genericFileCtrl_->SetEvtHandlerEnabled(true);

	genericFileCtrl_->SetFocus();

}

const std::vector<std::string>& SelectModArchivesDialog::getArchiveFilenames() const
{
	return archiveFilenames_;
}

void SelectModArchivesDialog::onSize(wxSizeEvent& event)
{
	scheduleRefresh();

	event.Skip();
}

void SelectModArchivesDialog::onMove(wxMoveEvent& event)
{
	scheduleRefresh();

	event.Skip();
}

void SelectModArchivesDialog::onRefresh(wxCommandEvent& WXUNUSED(event))
{
	--queuedRefreshEvents_;

	if (queuedRefreshEvents_ > 0)
		return;

	queuedRefreshEvents_ = 0;

	Refresh();
	Update();
	panel_->Update();
}

void SelectModArchivesDialog::genericFileCtrlOnFileSelectionChanged(wxTreeEvent& event)
{
	
	std::string archiveDir(genericFileCtrl_->GetDirectory());
	bool selectionExists = false;

	if (wxFileName(archiveDir).IsAbsolute() && !Utils::stringIsEqualNoCase(archiveDir, config_.game.folderPath + "\\" + config_.game.modsFolder+ "\\"))
	{
		wxArrayString archiveNames;
		genericFileCtrl_->GetPaths(archiveNames);
		if (!archiveNames.IsEmpty())
		{

		}
	}






	okButton_->Enable(selectionExists);

	event.Skip();
}

void SelectModArchivesDialog::scheduleRefresh()
{
	++queuedRefreshEvents_;

	wxCommandEvent* event = new wxCommandEvent(REFRESH_EVENT, GetId());
	GetEventHandler()->QueueEvent(event);
}
