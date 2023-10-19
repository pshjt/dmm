///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <Precompiled.hpp>

#include "DarkModManager.h"

///////////////////////////////////////////////////////////////////////////

WxfbMainFrame::WxfbMainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,600 ), wxDefaultSize );

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

	increasePriorityButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_INCREASE_PRIORITY_BUTTON, wxT("▲ Increase priority"), wxDefaultPosition, wxDefaultSize, 0 );
	increasePriorityButton_->SetToolTip( wxT("Increase priority (Alt+Up)") );

	boxSizer11111->Add( increasePriorityButton_, 0, wxBOTTOM|wxEXPAND, 1 );

	decreasePriorityButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_DECREASE_PRIORITY_BUTTON, wxT("▼ Decrease priority"), wxDefaultPosition, wxDefaultSize, 0 );
	decreasePriorityButton_->SetToolTip( wxT("Decrease priority (Alt+Down)") );

	boxSizer11111->Add( decreasePriorityButton_, 0, wxEXPAND|wxTOP|wxBOTTOM, 1 );

	activateDeactivateButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_ACTIVATE_DEACTIVATE_BUTTON, wxT("Activate / Deactivate"), wxDefaultPosition, wxDefaultSize, 0 );
	activateDeactivateButton_->SetToolTip( wxT("(De-)Activate mod (Double-click element / Enter)") );

	boxSizer11111->Add( activateDeactivateButton_, 0, wxBOTTOM|wxEXPAND|wxTOP, 1 );

	pauseResumeButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_PAUSE_RESUME_BUTTON, wxT("Pause / Resume"), wxDefaultPosition, wxDefaultSize, 0 );
	pauseResumeButton_->SetToolTip( wxT("Pause deactivates a mod while maintaining its order on the list  (Space)") );

	boxSizer11111->Add( pauseResumeButton_, 0, wxBOTTOM|wxEXPAND|wxTOP, 1 );

	deleteButton_ = new wxButton( boxSizer11111->GetStaticBox(), ID_DELETE_BUTTON, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	deleteButton_->SetToolTip( wxT("Delete this mod folder from your installation (Del)") );

	boxSizer11111->Add( deleteButton_, 0, wxEXPAND|wxTOP, 1 );


	boxSizer1111->Add( boxSizer11111, 0, wxEXPAND|wxALL, 2 );

	wxStaticBoxSizer* staticBoxSizer11113;
	staticBoxSizer11113 = new wxStaticBoxSizer( new wxStaticBox( scrolledWindow_, wxID_ANY, wxT("Info") ), wxVERTICAL );

	openReadmeButton_ = new wxButton( staticBoxSizer11113->GetStaticBox(), ID_OPEN_README_BUTTON, wxT("Open ReadMe file"), wxDefaultPosition, wxDefaultSize, 0 );
	staticBoxSizer11113->Add( openReadmeButton_, 0, wxBOTTOM|wxEXPAND, 1 );

	openModURLButton_ = new wxButton( staticBoxSizer11113->GetStaticBox(), ID_OPEN_MOD_URL_BUTTON, wxT("Lookup mod (WWW)"), wxDefaultPosition, wxDefaultSize, 0 );
	staticBoxSizer11113->Add( openModURLButton_, 0, wxBOTTOM|wxEXPAND|wxTOP, 1 );

	aboutButton_ = new wxButton( staticBoxSizer11113->GetStaticBox(), ID_ABOUT_BUTTON, wxT("About Mod Manager"), wxDefaultPosition, wxDefaultSize, 0 );
	staticBoxSizer11113->Add( aboutButton_, 0, wxEXPAND|wxTOP, 1 );


	boxSizer1111->Add( staticBoxSizer11113, 0, wxALL|wxEXPAND, 2 );

	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( scrolledWindow_, wxID_ANY, wxT("File") ), wxVERTICAL );

	selectModArchivesButton_ = new wxButton( sbSizer4->GetStaticBox(), ID_EXTRACT_MOD_BUTTON, wxT("Install mod archive(s)..."), wxDefaultPosition, wxDefaultSize, 0 );
	selectModArchivesButton_->SetToolTip( wxT("Browse for mod archives to install") );

	sbSizer4->Add( selectModArchivesButton_, 0, wxBOTTOM|wxEXPAND, 1 );

	manageProfilesButton_ = new wxButton( sbSizer4->GetStaticBox(), ID_MANAGE_PROFILES_BUTTON, wxT("Manage profiles..."), wxDefaultPosition, wxDefaultSize, 0 );
	manageProfilesButton_->SetToolTip( wxT("Open profile manager") );

	sbSizer4->Add( manageProfilesButton_, 0, wxEXPAND|wxTOP, 1 );

	openModsFolderButton_ = new wxButton( sbSizer4->GetStaticBox(), ID_OPEN_MODS_FOLDER_BUTTON, wxT("Open mods folder"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( openModsFolderButton_, 0, wxBOTTOM|wxEXPAND|wxTOP, 1 );

	selectGameFolderButton_ = new wxButton( sbSizer4->GetStaticBox(), ID_SELECT_GAME_FOLDER_BUTTON, wxT("Select game folder..."), wxDefaultPosition, wxDefaultSize, 0 );
	selectGameFolderButton_->SetToolTip( wxT("Browse for main game folder") );

	sbSizer4->Add( selectGameFolderButton_, 0, wxBOTTOM|wxEXPAND|wxTOP, 1 );


	boxSizer1111->Add( sbSizer4, 0, wxALL|wxEXPAND, 2 );

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( scrolledWindow_, wxID_ANY, wxT("Game") ), wxVERTICAL );

	applyButton_ = new wxButton( sbSizer5->GetStaticBox(), ID_APPLY_BUTTON, wxT("Apply changes"), wxDefaultPosition, wxDefaultSize, 0 );
	applyButton_->SetToolTip( wxT("Apply mod config (Ctrl+s)") );

	sbSizer5->Add( applyButton_, 0, wxBOTTOM|wxEXPAND, 1 );

	applyAndStartGameButton_ = new wxButton( sbSizer5->GetStaticBox(), ID_APPLY_AND_START_GAME_BUTTON, wxT("Launch game && exit"), wxDefaultPosition, wxDefaultSize, 0 );
	applyAndStartGameButton_->SetToolTip( wxT("Apply mod config and launch game with current setup") );

	sbSizer5->Add( applyAndStartGameButton_, 0, wxALL|wxBOTTOM|wxEXPAND|wxTOP, 1 );


	boxSizer1111->Add( sbSizer5, 1, wxALL|wxEXPAND, 2 );


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
	listCtrl_->Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( WxfbMainFrame::listCtrlOnItemEndDrag ), NULL, this );
	listCtrl_->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( WxfbMainFrame::listCtrlOnItemEndDrag ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler( WxfbMainFrame::listCtrlOnItemBeginDrag ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_COL_BEGIN_DRAG, wxListEventHandler( WxfbMainFrame::listCtrlOnListColBeginDrag ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_COL_DRAGGING, wxListEventHandler( WxfbMainFrame::listCtrlOnListColDragging ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( WxfbMainFrame::listCtrlOnListItemActivated ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( WxfbMainFrame::listCtrlOnListItemDeselected ), NULL, this );
	listCtrl_->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( WxfbMainFrame::listCtrlOnListItemSelected ), NULL, this );
	listCtrl_->Connect( wxEVT_MOTION, wxMouseEventHandler( WxfbMainFrame::listCtrlOnItemDrag ), NULL, this );
	listCtrl_->Connect( wxEVT_SIZE, wxSizeEventHandler( WxfbMainFrame::listCtrlOnSize ), NULL, this );
	increasePriorityButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::increasePriorityButtonOnButtonClick ), NULL, this );
	increasePriorityButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	decreasePriorityButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::decreasePriorityButtonOnButtonClick ), NULL, this );
	decreasePriorityButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	activateDeactivateButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::activateDeactivateButtonOnButtonClick ), NULL, this );
	activateDeactivateButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	pauseResumeButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::pauseResumeButtonOnButtonClick ), NULL, this );
	pauseResumeButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	deleteButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::deleteButtonOnButtonClick ), NULL, this );
	deleteButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	openReadmeButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::openReadmeButtonOnButtonClick ), NULL, this );
	openReadmeButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	openModURLButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::openModURLButtonOnButtonClick ), NULL, this );
	openModURLButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	aboutButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::aboutButtonOnButtonClick ), NULL, this );
	aboutButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	selectModArchivesButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::selectModArchivesButtonOnButtonClick ), NULL, this );
	selectModArchivesButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	manageProfilesButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::manageProfilesOnButtonClick ), NULL, this );
	manageProfilesButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	openModsFolderButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::openModsFolderButtonOnButtonClick ), NULL, this );
	openModsFolderButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
	selectGameFolderButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbMainFrame::selectGameFolderButtonOnButtonClick ), NULL, this );
	selectGameFolderButton_->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( WxfbMainFrame::buttonOnEnterWindow ), NULL, this );
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

WxfbManageProfilesDialog::WxfbManageProfilesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	profileListBox_ = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	profileListBox_->Enable( false );

	bSizer11->Add( profileListBox_, 1, wxALL|wxEXPAND, 2 );


	bSizer13->Add( bSizer11, 1, wxEXPAND, 0 );

	m_scrolledWindow4 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow4->SetScrollRate( 8, 8 );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow4, wxID_ANY, wxT("Import") ), wxVERTICAL );

	importProfileButton_ = new wxButton( sbSizer5->GetStaticBox(), wxID_ANY, wxT("Load file..."), wxDefaultPosition, wxDefaultSize, 0 );
	importProfileButton_->SetToolTip( wxT("Load previously exported mod configuration from file.") );

	sbSizer5->Add( importProfileButton_, 0, wxBOTTOM|wxEXPAND, 1 );

	useProfileButton_ = new wxButton( sbSizer5->GetStaticBox(), wxID_OK, wxT("Use"), wxDefaultPosition, wxDefaultSize, 0 );
	useProfileButton_->SetToolTip( wxT("Apply configuration This discards your current mod configuration.") );

	sbSizer5->Add( useProfileButton_, 0, wxEXPAND|wxTOP, 1 );


	bSizer12->Add( sbSizer5, 1, wxALL|wxEXPAND, 2 );

	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow4, wxID_ANY, wxT("Export") ), wxVERTICAL );

	exportProfileButton_ = new wxButton( sbSizer6->GetStaticBox(), wxID_ANY, wxT("Save profile..."), wxDefaultPosition, wxDefaultSize, 0 );
	exportProfileButton_->SetToolTip( wxT("Export your current mod configuration to its own file") );

	sbSizer6->Add( exportProfileButton_, 0, wxALL|wxEXPAND, 1 );


	bSizer12->Add( sbSizer6, 1, wxALL|wxEXPAND, 2 );


	m_scrolledWindow4->SetSizer( bSizer12 );
	m_scrolledWindow4->Layout();
	bSizer12->Fit( m_scrolledWindow4 );
	bSizer13->Add( m_scrolledWindow4, 0, wxEXPAND|wxFIXED_MINSIZE, 0 );


	bSizer10->Add( bSizer13, 1, wxALL|wxEXPAND, 2 );


	this->SetSizer( bSizer10 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	importProfileButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbManageProfilesDialog::importProfileButtonOnButtonClick ), NULL, this );
	exportProfileButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbManageProfilesDialog::exportProfileButtonOnButtonClick ), NULL, this );
}

WxfbManageProfilesDialog::~WxfbManageProfilesDialog()
{
}
