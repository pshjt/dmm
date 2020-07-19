// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "Utils.hpp"

#include "StatusBarManipulator.hpp"

StatusBarManipulator::StatusBarManipulator(wxStatusBar* statusBar, ApplicationConfig& config)
	: config_(config)
{
	const int marginLeft = 4;
	const int marginRight = 4;

	statusBar_ = statusBar;
	status_ = StatusBarStatus::NONE;

	int statusBarHeight = statusBar_->GetSize().GetHeight();
	int verticalBorder = statusBar_->GetBorders().GetHeight();

	///

	warningStaticBitmap_ = new wxStaticBitmap(statusBar_, wxID_ANY, wxArtProvider::GetBitmap(wxART_WARNING, wxART_FRAME_ICON,
		wxArtProvider::GetSizeHint(wxART_FRAME_ICON)));

	int bitmapWidth, bitmapHeight;
	warningStaticBitmap_->GetSize(&bitmapWidth, &bitmapHeight);

	int bitmapPositionY = (statusBarHeight - bitmapHeight) / 2 + verticalBorder;

	if (bitmapPositionY < verticalBorder)
		bitmapPositionY = verticalBorder;

	warningStaticBitmap_->SetPosition(wxPoint(marginLeft, bitmapPositionY));
	warningStaticBitmap_->Hide();

	///

	staticText_ = new TransparentStaticText(statusBar_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxST_ELLIPSIZE_END);

	statusBar_->SetDoubleBuffered(true);

	int textHeight = staticText_->GetSize().GetHeight();

	int textPositionY = (statusBarHeight - textHeight) / 2 + verticalBorder;

	if (textPositionY < verticalBorder)
		textPositionY = verticalBorder;

	staticText_->SetPosition(wxPoint(-1, textPositionY));

	///

	staticTextPositionX_ = marginLeft;
	staticTextWithBitmapPositionX_ = marginLeft + bitmapWidth + marginRight;
	staticTextSizeGripPadding_ = bitmapWidth;

	adjustSize();
}

void StatusBarManipulator::setStatus(StatusBarStatus status)
{
	if (status_ == status)
		return;

	status_ = status;
	bool isWarning = false;
	std::string statusText;

	std::string statusBase = "Status: ";

	switch (status)
	{
	case StatusBarStatus::GAME_FOLDER_NOT_SELECTED:
		statusText = "Game folder is not selected.";
		break;

	case StatusBarStatus::NO_MODS_DETECTED:
		statusText = "Press \"Install mod archives...\" button and choose the mods you want to use";
		break;

	case StatusBarStatus::MOD_TYPE_UNRECOGNIZED:
		statusText = "Unrecognized mod type. Make sure that mod is extracted and placed properly.";
		isWarning = true;
		break;

	case StatusBarStatus::FOLDER_NAME_TOO_LONG:
		statusText = "Mod's folder name is too long, rename to shorter. Maximum length is ";
		statusText += Utils::toString(config_.game.maxFolderLength);
		statusText += " characters for mod folder and ";
		statusText += Utils::toString(config_.game.maxFullPathLength);
		statusText += " for full path length including mod folder.";
		isWarning = true;
		break;

	case StatusBarStatus::PLUS_SIGN_IN_FOLDER_NAME:
		statusText = "Plus sign is reserved as a path separator. Remove \"+\" from mod's folder name.";
		isWarning = true;
		break;

	case StatusBarStatus::MULTIPLE_GAMESYS_MODS_ACTIVE:
		statusText = statusBase;
		statusText += "Multiple gamesys mods are active. Do not activate more than one at a time.";
		isWarning = true;
		break;

	case StatusBarStatus::TOO_MANY_MODS_ACTIVE:
		statusText = "Too many mods are active or paused. Do not exceed ";
		statusText += Utils::toString(config_.game.maxActive);
		statusText += " mods limit.";
		isWarning = true;
		break;

	case StatusBarStatus::ISSUES_DETECTED:
		statusText = statusBase;
		statusText += "Issues detected. Select mod with a warning for details.";
		isWarning = true;
		break;

	case StatusBarStatus::OK:
		statusText = statusBase;
		statusText += "OK";
		break;

	case StatusBarStatus::NONE:
		break;
	}

	int posX;

	if (isWarning)
	{
		warningStaticBitmap_->Show();
		posX = staticTextWithBitmapPositionX_;
	}
	else
	{
		warningStaticBitmap_->Hide();
		posX = staticTextPositionX_;
	}

	staticText_->SetPosition(wxPoint(posX, -1));
	staticText_->SetLabel(wxString(statusText));

	adjustSize();
	statusBar_->Refresh();
}

void StatusBarManipulator::adjustSize()
{
	int fieldWidth = statusBar_->GetSize().GetWidth() - staticTextSizeGripPadding_;
	int textWidth = fieldWidth - staticText_->GetPosition().x;

	if (textWidth < 1)
		textWidth = 1;

	staticText_->SetSize(wxSize(textWidth, -1));
}