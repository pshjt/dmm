// Copyright 2013-2020 Marvin
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "CustomEvent.hpp"
#include "Utils.hpp"

#include "ManageProfilesFrame.hpp"

ManageProfilesFrame::ManageProfilesFrame(wxWindow* parent, const ApplicationConfig& applicationConfig)
	: WxfbManageProfilesFrame(parent)
{
	//Connect(wxEVT_MOVE, wxMoveEventHandler(SelectGameFolderDialog::onMove));
	//Connect(REFRESH_EVENT, wxCommandEventHandler(SelectGameFolderDialog::onRefresh));
}