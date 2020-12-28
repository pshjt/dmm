#include <Precompiled.hpp>

#include "CustomEvent.hpp"
#include "Utils.hpp"

#include "SelectGameFolderDialog.hpp"

SelectGameFolderDialog::SelectGameFolderDialog(wxWindow* parent, const ApplicationConfig& applicationConfig)
	: WxfbSelectGameFolderDialog(parent)
	, config_(applicationConfig)
	, queuedRefreshEvents_(0)
{
	Connect(wxEVT_MOVE, wxMoveEventHandler(SelectGameFolderDialog::onMove));
	Connect(REFRESH_EVENT, wxCommandEventHandler(SelectGameFolderDialog::onRefresh));

	{
		float scale = 0.75f;
		wxSize maxSize = parent->GetSize();

		int maxWidth = static_cast<int>(static_cast<float>(maxSize.GetWidth()) * scale);
		int maxHeight = static_cast<int>(static_cast<float>(maxSize.GetHeight()) * scale);

		Utils::adjustSizeDown(this, maxWidth, maxHeight);
	}

	panel_->SetDoubleBuffered(true);

	if (wxDirExists(config_.game.folderPath))
		pathTextCtrl_->SetLabel(config_.game.folderPath);
	else
		pathTextCtrl_->SetLabel(config_.application.executableFolderPath);

	genericDirCtrl_->SetFocus();
}

const std::string& SelectGameFolderDialog::getGameFolderPath() const
{
	return gameFolderPath_;
}

const std::string SelectGameFolderDialog::getAlternativeExecutable(std::string gameFolderPath) const
{
	std::string alternativeExePath;

	for (auto& alternativeExe : config_.game.additionalExecutableFiles)
	{
		alternativeExePath = gameFolderPath + '\\' + alternativeExe;
		if (wxFileName::FileExists(alternativeExePath))
		{
			return Utils::getFilenameCaseInsensitive(wxFileName(alternativeExePath));
		}
	}

	return std::string();
}

void SelectGameFolderDialog::onSize(wxSizeEvent& event)
{
	scheduleRefresh();

	event.Skip();
}

void SelectGameFolderDialog::onMove(wxMoveEvent& event)
{
	scheduleRefresh();

	event.Skip();
}

void SelectGameFolderDialog::onRefresh(wxCommandEvent& WXUNUSED(event))
{
	--queuedRefreshEvents_;

	if (queuedRefreshEvents_ > 0)
		return;

	queuedRefreshEvents_ = 0;

	Refresh();
	Update();
	panel_->Update();
}

void SelectGameFolderDialog::pathTextCtrlOnText(wxCommandEvent& event)
{
	std::string path(pathTextCtrl_->GetLineText(0));

	genericDirCtrl_->SetEvtHandlerEnabled(false);
	genericDirCtrl_->SetPath(path);
	genericDirCtrl_->SetEvtHandlerEnabled(true);

	if (wxFileName(path).IsAbsolute() && wxDirExists(path))
		gameFolderPath_ = genericDirCtrl_->GetPath();
	else
		gameFolderPath_ = "";

	std::string info;
	bool isExeFound = false;

	if (!gameFolderPath_.empty())
	{
		Utils::normalizePath(gameFolderPath_);

		std::string exePath = gameFolderPath_;
		exePath += "\\";
		exePath += config_.game.executableFile;

		if (!wxFileExists(exePath))
		{
			std::string alternativeExe = getAlternativeExecutable(gameFolderPath_);
			if (!alternativeExe.empty())
				exePath = gameFolderPath_ + '\\' + alternativeExe;
		}

		if (wxFileExists(exePath))
		{
			isExeFound = true;
			std::string name, version;
			Utils::getVersionInfo(exePath, name, version);
		}
	}

	infoStaticText_->SetLabel(info);
	okButton_->Enable(isExeFound);

	event.Skip();
}

void SelectGameFolderDialog::genericDirCtrlOnTreeSelChanged(wxTreeEvent& event)
{
	pathTextCtrl_->SetLabel(genericDirCtrl_->GetPath());

	event.Skip();
}

void SelectGameFolderDialog::scheduleRefresh()
{
	++queuedRefreshEvents_;

	wxCommandEvent* event = new wxCommandEvent(REFRESH_EVENT, GetId());
	GetEventHandler()->QueueEvent(event);
}