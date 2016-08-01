// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "MainFrame.hpp"

#include "Application.hpp"

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
	#ifdef _DEBUG
		wxLogWindow* logWindow = new wxLogWindow(nullptr, "Logger");

		wxRect clientArea = wxDisplay().GetClientArea();
		logWindow->GetFrame()->SetPosition(clientArea.GetPosition());

		logWindow->Show();

		wxLogDebug(wxGetLibraryVersionInfo().ToString());
	#endif

	new MainFrame(nullptr);

	return true;
}
