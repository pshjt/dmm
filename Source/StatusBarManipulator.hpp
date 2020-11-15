// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "ApplicationConfig.hpp"
#include "TransparentStaticText.hpp"

enum class StatusBarStatus
{
	GAME_FOLDER_NOT_SELECTED,
	NO_MODS_DETECTED,
	MOD_TYPE_UNRECOGNIZED,
	FOLDER_NAME_TOO_LONG,
	PLUS_SIGN_IN_FOLDER_NAME,
	MULTIPLE_GAMESYS_MODS_ACTIVE,
	DUPLICATE_FOLDERS,
	TOO_MANY_MODS_ACTIVE,
	ISSUES_DETECTED,
	OK,
	NONE
};

class StatusBarManipulator
{
public:
	StatusBarManipulator(wxStatusBar* statusBar, ApplicationConfig& config);
	// ~StatusBarManipulator() {}

	void setStatus(StatusBarStatus status);
	void adjustSize();

private:
	// Non copyable.
	StatusBarManipulator(const StatusBarManipulator&); // Not to implement.
	StatusBarManipulator& operator =(const StatusBarManipulator&); // Not to implement.

	wxStatusBar* statusBar_;
	StatusBarStatus status_;

	wxStaticBitmap* warningStaticBitmap_;
	TransparentStaticText* staticText_;

	int staticTextPositionX_;
	int staticTextWithBitmapPositionX_;
	int staticTextSizeGripPadding_;

	ApplicationConfig& config_;
};
