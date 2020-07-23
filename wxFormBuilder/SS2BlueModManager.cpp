///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <Precompiled.hpp>

#include "SS2BlueModManager.h"

///////////////////////////////////////////////////////////////////////////

WxfbMainFrame::WxfbMainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	statusBar_ = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	wxBoxSizer* boxSizer1;
	boxSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* boxSizer111;
	boxSizer111 = new wxBoxSizer( wxVERTICAL );

	listCtrl_ = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );
	boxSizer111->Add( listCtrl_, 1, wxEXPAND|wxALL, 2 );


	bSizer11->Add( boxSizer111, 1, wxEXPAND, 0 );

	scrolledWindow_ = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	scrolledWindow_->SetScrollRate( 8, 8 );
	wxBoxSizer* boxSizer1111;
	boxSizer1111 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* boxSizer11111;
	boxSizer11111 = new wxStaticBoxSizer( new wxStaticBox( scrolledWindow_, wxID_ANY, wxT("Edit") ), wxVERTICAL );

	activateDeactivateButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_ACTIVATE_DEACTIVATE_BUTTON, wxT("Activate / Deactivate"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer11111->Add( activateDeactivateButton_, 0, wxEXPAND|wxBOTTOM, 1 );

	increasePriorityButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_INCREASE_PRIORITY_BUTTON, wxT("Increase priority"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer11111->Add( increasePriorityButton_, 0, wxEXPAND|wxTOP|wxBOTTOM, 1 );

	decreasePriorityButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_DECREASE_PRIORITY_BUTTON, wxT("Decrease priority"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer11111->Add( decreasePriorityButton_, 0, wxEXPAND|wxTOP|wxBOTTOM, 1 );

	pauseResumeButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_PAUSE_RESUME_BUTTON, wxT("Pause / Resume"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer11111->Add( pauseResumeButton_, 0, wxBOTTOM|wxEXPAND|wxTOP, 1 );

	deleteButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_DELETE_BUTTON, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer11111->Add( deleteButton_, 0, wxEXPAND, 1 );


	boxSizer1111->Add( boxSizer11111, 0, wxEXPAND|wxALL, 2 );

	wxStaticBoxSizer* staticBoxSizer11112;
	staticBoxSizer11112 = new wxStaticBoxSizer( new wxStaticBox( scrolledWindow_, wxID_ANY, wxT("Tools") ), wxVERTICAL );

	openModsFolderButton_ = new wxButton( staticBoxSizer11112->GetStaticBox(), ID_OPEN_MODS_FOLDER_BUTTON, wxT("Open mods folder"), wxDefaultPosition, wxDefaultSize, 0 );
	staticBoxSizer11112->Add( openModsFolderButton_, 0, wxEXPAND|wxBOTTOM, 1 );

	selectGameFolderButton_ = new wxButton( staticBoxSizer11112->GetStaticBox(), ID_SELECT_GAME_FOLDER_BUTTON, wxT("Select game folder..."), wxDefaultPosition, wxDefaultSize, 0 );
	staticBoxSizer11112->Add( selectGameFolderButton_, 0, wxBOTTOM|wxEXPAND|wxTOP, 1 );


	boxSizer1111->Add( staticBoxSizer11112, 0, wxEXPAND|wxALL, 2 );

	wxStaticBoxSizer* staticBoxSizer11113;
	staticBoxSizer11113 = new wxStaticBoxSizer( new wxStaticBox( scrolledWindow_, wxID_ANY, wxT("Info") ), wxVERTICAL );

	openModURLButton_ = new wxButton( staticBoxSizer11113->GetStaticBox(), ID_OPEN_MOD_URL_BUTTON, wxT("Lookup mod (WWW)"), wxDefaultPosition, wxDefaultSize, 0 );
	staticBoxSizer11113->Add( openModURLButton_, 0, wxEXPAND|wxBOTTOM, 1 );

	openReadmeButton_ = new wxButton( staticBoxSizer11113->GetStaticBox(), ID_OPEN_README_BUTTON, wxT("Open ReadMe file"), wxDefaultPosition, wxDefaultSize, 0 );
	staticBoxSizer11113->Add( openReadmeButton_, 0, wxEXPAND, 1 );


	boxSizer1111->Add( staticBoxSizer11113, 0, wxALL|wxEXPAND, 2 );

	wxBoxSizer* boxSizer11114;
	boxSizer11114 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer01;
	bSizer01 = new wxBoxSizer( wxVERTICAL );

	selectModArchivesButton_ = new wxButton( scrolledWindow_, ID_EXTRACT_MOD_BUTTON, wxT("Install mod archive(s)..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer01->Add( selectModArchivesButton_, 0, wxALL|wxEXPAND, 1 );


	boxSizer11114->Add( bSizer01, 1, wxALIGN_BOTTOM|wxALL, 1 );


	boxSizer1111->Add( boxSizer11114, 1, wxALIGN_BOTTOM|wxALL|wxEXPAND, 0 );

	wxBoxSizer* boxSizer11115;
	boxSizer11115 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer02;
	bSizer02 = new wxBoxSizer( wxVERTICAL );

	aboutButton_ = new wxButton( scrolledWindow_, ID_ABOUT_BUTTON, wxT("About..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer02->Add( aboutButton_, 0, wxALIGN_BOTTOM|wxALL|wxEXPAND, 1 );


	bSizer02->Add( 0, 5, 1, wxEXPAND, 5 );

	applyButton_ = new wxButton( scrolledWindow_, ID_APPLY_BUTTON, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer02->Add( applyButton_, 0, wxALL|wxBOTTOM|wxEXPAND, 1 );

	applyAndStartGameButton_ = new wxButton( scrolledWindow_, ID_APPLY_AND_START_GAME_BUTTON, wxT("Apply and start game"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer02->Add( applyAndStartGameButton_, 0, wxALL|wxBOTTOM|wxEXPAND|wxTOP, 1 );


	boxSizer11115->Add( bSizer02, 1, wxALIGN_BOTTOM|wxALL, 1 );


	boxSizer1111->Add( boxSizer11115, 1, wxALIGN_BOTTOM|wxALL|wxEXPAND, 0 );


	scrolledWindow_->SetSizer( boxSizer1111 );
	scrolledWindow_->Layout();
	boxSizer1111->Fit( scrolledWindow_ );
	bSizer11->Add( scrolledWindow_, 0, wxEXPAND|wxFIXED_MINSIZE, 0 );


	boxSizer1->Add( bSizer11, 1, wxALL|wxEXPAND, 2 );


	this->SetSizer( boxSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( WxfbMainFrame::onClose ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( WxfbMainFrame::onSize ) );
	statusBar_->Connect( wxEVT_SIZE, wxSizeEventHandler( WxfbMainFrame::statusBarOnSize ), NULL, this );
	listCtrl_->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( WxfbMainFrame::listCtrlOnKillFocus ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_COL_BEGIN_DRAG, wxListEventHandler( WxfbMainFrame::listCtrlOnListColBeginDrag ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_COL_DRAGGING, wxListEventHandler( WxfbMainFrame::listCtrlOnListColDragging ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( WxfbMainFrame::listCtrlOnListItemActivated ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( WxfbMainFrame::listCtrlOnListItemDeselected ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( WxfbMainFrame::listCtrlOnListItemSelected ), NULL, this );
	listCtrl_->Connect( wxEVT_SIZE, wxSizeEventHandler( WxfbMainFrame::listCtrlOnSize ), NULL, this );
	activateDeactivateButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::activateDeactivateButtonOnButtonClick ), NULL, this );
	activateDeactivateButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	increasePriorityButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::increasePriorityButtonOnButtonClick ), NULL, this );
	increasePriorityButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	decreasePriorityButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::decreasePriorityButtonOnButtonClick ), NULL, this );
	decreasePriorityButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	pauseResumeButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::pauseResumeButtonOnButtonClick ), NULL, this );
	pauseResumeButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	deleteButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::deleteButtonOnButtonClick ), NULL, this );
	deleteButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	openModsFolderButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::openModsFolderButtonOnButtonClick ), NULL, this );
	openModsFolderButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	selectGameFolderButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::selectGameFolderButtonOnButtonClick ), NULL, this );
	selectGameFolderButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	openModURLButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::openModURLButtonOnButtonClick ), NULL, this );
	openModURLButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	openReadmeButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::openReadmeButtonOnButtonClick ), NULL, this );
	openReadmeButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	selectModArchivesButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::selectModArchivesButtonOnButtonClick ), NULL, this );
	selectModArchivesButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	aboutButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::aboutButtonOnButtonClick ), NULL, this );
	aboutButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	applyButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::applyButtonOnButtonClick ), NULL, this );
	applyButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	applyAndStartGameButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::applyAndStartGameButtonOnButtonClick ), NULL, this );
	applyAndStartGameButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
}

WxfbMainFrame::~WxfbMainFrame()
{
}

WxfbSelectGameFolderDialog::WxfbSelectGameFolderDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	wxBoxSizer* boxSizer1;
	boxSizer1 = new wxBoxSizer( wxVERTICAL );

	panel_ = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* boxSizer11;
	boxSizer11 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* boxSizer111;
	boxSizer111 = new wxBoxSizer( wxVERTICAL );

	pathTextCtrl_ = new wxTextCtrl( panel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer111->Add( pathTextCtrl_, 0, wxALL|wxEXPAND, 4 );

	genericDirCtrl_ = new wxGenericDirCtrl( panel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_DIR_ONLY, wxEmptyString, 0 );

	genericDirCtrl_->ShowHidden( false );
	boxSizer111->Add( genericDirCtrl_, 1, wxEXPAND | wxALL, 4 );

	scrolledWindow_ = new wxScrolledWindow( panel_, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	scrolledWindow_->SetScrollRate( 8, 8 );
	wxBoxSizer* boxSizer1111;
	boxSizer1111 = new wxBoxSizer( wxVERTICAL );

	infoStaticText_ = new wxStaticText( scrolledWindow_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	infoStaticText_->Wrap( -1 );
	boxSizer1111->Add( infoStaticText_, 0, wxALL|wxEXPAND, 4 );

	wxBoxSizer* boxSizer11111;
	boxSizer11111 = new wxBoxSizer( wxHORIZONTAL );

	okButton_ = new wxButton( scrolledWindow_, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer11111->Add( okButton_, 0, wxTOP|wxRIGHT|wxLEFT, 4 );

	cancelButton_ = new wxButton( scrolledWindow_, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer11111->Add( cancelButton_, 0, wxTOP|wxRIGHT|wxLEFT, 4 );


	boxSizer1111->Add( boxSizer11111, 0, wxALIGN_RIGHT, 0 );


	scrolledWindow_->SetSizer( boxSizer1111 );
	scrolledWindow_->Layout();
	boxSizer1111->Fit( scrolledWindow_ );
	boxSizer111->Add( scrolledWindow_, 0, wxEXPAND|wxFIXED_MINSIZE, 0 );


	boxSizer11->Add( boxSizer111, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 4 );


	panel_->SetSizer( boxSizer11 );
	panel_->Layout();
	boxSizer11->Fit( panel_ );
	boxSizer1->Add( panel_, 1, wxEXPAND, 0 );


	boxSizer1->Add( 0, 0, 0, wxTOP, 16 );


	this->SetSizer( boxSizer1 );
	this->Layout();
	boxSizer1->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( WxfbSelectGameFolderDialog::onSize ) );
	pathTextCtrl_->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WxfbSelectGameFolderDialog::pathTextCtrlOnText ), NULL, this );
	genericDirCtrl_->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( WxfbSelectGameFolderDialog::genericDirCtrlOnTreeSelChanged ), NULL, this );
}

WxfbSelectGameFolderDialog::~WxfbSelectGameFolderDialog()
{
}
