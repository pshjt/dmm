///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/scrolwin.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/dirctrl.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/dialog.h>
#include <wx/listbox.h>

///////////////////////////////////////////////////////////////////////////

#define ID_INCREASE_PRIORITY_BUTTON 1000
#define ID_DECREASE_PRIORITY_BUTTON 1001
#define ID_ACTIVATE_DEACTIVATE_BUTTON 1002
#define ID_PAUSE_RESUME_BUTTON 1003
#define ID_DELETE_BUTTON 1004
#define ID_OPEN_README_BUTTON 1005
#define ID_OPEN_MOD_URL_BUTTON 1006
#define ID_ABOUT_BUTTON 1007
#define ID_EXTRACT_MOD_BUTTON 1008
#define ID_OPEN_MODS_FOLDER_BUTTON 1009
#define ID_SELECT_GAME_FOLDER_BUTTON 1010
#define ID_MANAGE_PROFILES_BUTTON 1011
#define ID_APPLY_BUTTON 1012
#define ID_APPLY_AND_START_GAME_BUTTON 1013

///////////////////////////////////////////////////////////////////////////////
/// Class WxfbMainFrame
///////////////////////////////////////////////////////////////////////////////
class WxfbMainFrame : public wxFrame
{
	private:

	protected:
		wxStatusBar* statusBar_;
		wxListCtrl* listCtrl_;
		wxScrolledWindow* scrolledWindow_;
		wxButton* increasePriorityButton_;
		wxButton* decreasePriorityButton_;
		wxButton* activateDeactivateButton_;
		wxButton* pauseResumeButton_;
		wxButton* deleteButton_;
		wxButton* openReadmeButton_;
		wxButton* openModURLButton_;
		wxButton* aboutButton_;
		wxButton* selectModArchivesButton_;
		wxButton* openModsFolderButton_;
		wxButton* selectGameFolderButton_;
		wxButton* manageProfilesButton_;
		wxButton* applyButton_;
		wxButton* applyAndStartGameButton_;

		// Virtual event handlers, overide them in your derived class
		virtual void onClose( wxCloseEvent& event ) = 0;
		virtual void onSize( wxSizeEvent& event ) = 0;
		virtual void statusBarOnSize( wxSizeEvent& event ) = 0;
		virtual void listCtrlOnKillFocus( wxFocusEvent& event ) = 0;
		virtual void listCtrlOnListColBeginDrag( wxListEvent& event ) = 0;
		virtual void listCtrlOnListColDragging( wxListEvent& event ) = 0;
		virtual void listCtrlOnListItemActivated( wxListEvent& event ) = 0;
		virtual void listCtrlOnListItemDeselected( wxListEvent& event ) = 0;
		virtual void listCtrlOnListItemSelected( wxListEvent& event ) = 0;
		virtual void listCtrlOnSize( wxSizeEvent& event ) = 0;
		virtual void increasePriorityButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void buttonOnEnterWindow( wxMouseEvent& event ) = 0;
		virtual void decreasePriorityButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void activateDeactivateButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void pauseResumeButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void deleteButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void openReadmeButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void openModURLButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void aboutButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void selectModArchivesButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void openModsFolderButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void selectGameFolderButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void manageProfilesOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void applyButtonOnButtonClick( wxCommandEvent& event ) = 0;
		virtual void applyAndStartGameButtonOnButtonClick( wxCommandEvent& event ) = 0;


	public:

		WxfbMainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,800 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~WxfbMainFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class WxfbSelectGameFolderDialog
///////////////////////////////////////////////////////////////////////////////
class WxfbSelectGameFolderDialog : public wxDialog
{
	private:

	protected:
		wxPanel* panel_;
		wxTextCtrl* pathTextCtrl_;
		wxGenericDirCtrl* genericDirCtrl_;
		wxScrolledWindow* scrolledWindow_;
		wxStaticText* infoStaticText_;
		wxButton* okButton_;
		wxButton* cancelButton_;

		// Virtual event handlers, overide them in your derived class
		virtual void onSize( wxSizeEvent& event ) = 0;
		virtual void pathTextCtrlOnText( wxCommandEvent& event ) = 0;
		virtual void genericDirCtrlOnTreeSelChanged( wxTreeEvent& event ) = 0;


	public:

		WxfbSelectGameFolderDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Select game folder"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
		~WxfbSelectGameFolderDialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class WxfbManageProfilesDialog
///////////////////////////////////////////////////////////////////////////////
class WxfbManageProfilesDialog : public wxDialog
{
	private:

	protected:
		wxListBox* profileListBox_;
		wxScrolledWindow* m_scrolledWindow4;
		wxButton* createProfileButton_;
		wxButton* renameProfileButton_;
		wxButton* deleteProfileButton_;
		wxButton* useProfileButton_;

	public:

		WxfbManageProfilesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~WxfbManageProfilesDialog();

};

