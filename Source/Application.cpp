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