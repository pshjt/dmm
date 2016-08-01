// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "AboutDialog.hpp"
#include "ApplicationInfo.hpp"
#include "CustomEvent.hpp"
#include "Message.hpp"
#include "SelectGameFolderDialog.hpp"
#include "ShellRun.hpp"
#include "Utils.hpp"

#include "MainFrame.hpp"

const int MainFrame::ID_DUMMY_0_ = 0;
const int MainFrame::ID_INDEX_ = 1;
const int MainFrame::ID_NAME_ = 2;
const int MainFrame::ID_STATUS_ = 3;
const int MainFrame::ID_TYPE_ = 4;
const int MainFrame::ID_DUMMY_1_ = 5;

const std::vector<int> MainFrame::DEFAULT_ORDER_ =
{{
	ID_INDEX_,
	ID_NAME_,
	ID_STATUS_,
	ID_TYPE_,
	ID_DUMMY_1_,
	ID_DUMMY_0_
}};

MainFrame::MainFrame(wxWindow* parent)
	: WxfbMainFrame(parent)
	, modManager_(config_)
	, queuedInterfaceUpdateEvents_(0)
	, shouldSetColumnsWidth_(false)
	, shouldStartGame_(false)
	, messageDialogOpen_(false)
	, columnsTotalWidth_(-1)
	, columnsBaseNameWidth_(-1)
	, columnsCanResize_(false)
	, auxLastNameWidth_(-1)
	, statusBarManipulator_(statusBar_, config_)
{
	connectEventFunctions();

	config_.assignFile(config_.application.executableFolderPath + '\\' + config_.application.configFile);

	if (!config_.load())
		Message::showMessage(Message::MessageType::ERROR_OK, "Couldn't load application settings.", config_.getErrorMessage());

	{
		if (!Utils::checkVersion("1.11", config_.application.version))
			config_.game.baseModPath = defaultConfig_.game.baseModPath;
	}

	initialize();
	interfaceInitialize();
	interfaceUpdate();
	Show();

	modManager_.initialize();
	loadInitialModArchives();
	scheduleInterfaceUpdate();
}

void MainFrame::onClose(wxCloseEvent& event)
{
	Utils::pointToString(size_.GetWidth(), size_.GetHeight(), config_.window.size);
	Utils::pointToString(position_.x, position_.y, config_.window.position);
	config_.window.isMaximized = isMaximized_;
	columnsSaveOrder();

	config_.application.version = ApplicationInfo::versionMajor;

	if (!ApplicationInfo::versionMinor.empty())
	{
		config_.application.version += ".";
		config_.application.version += ApplicationInfo::versionMinor;
	}

	if (!config_.saveOverwriting("; Configuration File - " + ApplicationInfo::full))
		Message::showMessage(Message::MessageType::ERROR_OK, "Couldn't save application settings.", config_.getErrorMessage());

	modManager_.shutDown();

	if (shouldStartGame_)
	{
		ShellRun shellRun;
		std::string gameExePath = config_.game.folderPath + '\\' + config_.game.currentExecutableFile;

		if (!shellRun.open(gameExePath, config_.game.folderPath))
			Message::showMessage(Message::MessageType::ERROR_OK, "Couldn't run:\n" + gameExePath, shellRun.getErrorMessage());
	}

	event.Skip();
}

void MainFrame::onSize(wxSizeEvent& event)
{
	isMaximized_ = IsMaximized();

	if (!isMaximized_)
		size_ = GetSize();

	scheduleListRefresh();
	scheduleRefresh();

	event.Skip();
}

void MainFrame::onMove(wxMoveEvent& event)
{
	isMaximized_ = IsMaximized();

	if (!isMaximized_)
		position_ = GetPosition();

	statusBar_->Refresh();
	statusBar_->Update();

	event.Skip();
}

void MainFrame::onRefresh(wxCommandEvent& WXUNUSED(event))
{
	listCtrl_->Thaw();

	Refresh();
	Update();
}

void MainFrame::statusBarOnSize(wxSizeEvent& event)
{
	statusBarManipulator_.adjustSize();

	statusBar_->Refresh();
	statusBar_->Update();

	event.Skip();
}

void MainFrame::listCtrlOnKillFocus(wxFocusEvent& WXUNUSED(event))
{
}

void MainFrame::listCtrlOnListColBeginDrag(wxListEvent& event)
{
	event.Veto();
}

void MainFrame::listCtrlOnListColDragging(wxListEvent& event)
{
	if (columnsCanResize_)
	{
		event.Skip();

		return;
	}

	event.Veto();
}

void MainFrame::listCtrlOnListItemActivated(wxListEvent& event)
{
	buttonClick(activateDeactivateButton_);

	event.Skip();
}

void MainFrame::listCtrlOnListItemDeselected(wxListEvent& event)
{
	modManager_.setSelected(-1);

	scheduleInterfaceUpdate();
	event.Skip();
}

void MainFrame::listCtrlOnListItemSelected(wxListEvent& event)
{
	modManager_.setSelected(event.GetIndex());

	scheduleInterfaceUpdate();
	event.Skip();
}

void MainFrame::listCtrlOnSize(wxSizeEvent& event)
{
	scheduleListRefresh();

	event.Skip();
}

void MainFrame::activateDeactivateButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	modManager_.activateDeactivate();

	buttonClickFinish(event);
}

void MainFrame::increasePriorityButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	modManager_.increasePriority();

	buttonClickFinish(event);
}

void MainFrame::decreasePriorityButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	modManager_.decreasePriority();

	buttonClickFinish(event);
}

void MainFrame::pauseResumeButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	modManager_.pauseResume();

	buttonClickFinish(event);
}

void MainFrame::deleteButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	wxFileName path(modManager_.getModsFolderPath() + '\\' + modManager_.getMods()[modManager_.getSelected()].getName() + "\\");
	path.MakeAbsolute();
	wxArrayString dirs = path.GetDirs();
	if (path.IsDir() && path.IsDirWritable() && !(Utils::stringIsEqualNoCase(dirs.Last().ToStdString(), config_.game.modsFolder)))
	{
		wxMessageDialog deleteDialog_(this, wxString("Do you want do delete this mod? \n" + dirs.Last() + " \n\nThis will remove the mod folder from your SS2 installation."), wxString("Delete mod?"), wxYES_NO);

		if (deleteDialog_.ShowModal() == wxID_YES)
		{
			bool isSuccess = path.Rmdir(wxPATH_RMDIR_RECURSIVE);
			if (!isSuccess)
				Message::showMessage(Message::MessageType::ERROR_OK, "Could not delete protected folder.");

		}			
	}

	buttonClickFinish(event);
}

void MainFrame::openModsFolderButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	int selected = modManager_.getSelected();
	std::string modsFolderPath = modManager_.getModsFolderPath();
	std::string path;
	ShellRun shellRun;
	bool isSuccess = false;

	if (!(selected < 0))
	{
		path = modsFolderPath + '\\' + modManager_.getMods()[selected].getName();
		isSuccess = shellRun.openFolderAndSelectItem(path);
	}
	else
		isSuccess = false;


	if (!isSuccess)
	{
		path = modsFolderPath;
		isSuccess = shellRun.open(path);
	}

	if (!isSuccess)
		Message::showMessage(Message::MessageType::ERROR_OK, "Couldn't open:\n" + path, shellRun.getErrorMessage());

	buttonClickFinish(event);
}

void MainFrame::selectGameFolderButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	SelectGameFolderDialog selectGameFolder(this, config_);

	if (selectGameFolder.ShowModal() == wxID_OK)
		modManager_.initialize(selectGameFolder.getGameFolderPath());

	buttonClickFinish(event);
}

void MainFrame::openModURLButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	int selected = modManager_.getSelected();
	std::string modName = modManager_.getMods()[selected].getName();
	Utils::stringReplaceCharWithChar(modName, '-', '+');
	wxString lookupURL = config_.application.lookupURL;
	ShellRun shellRun;

    for (auto it = config_.application.modArchiveExtensions.begin(); it < config_.application.modArchiveExtensions.end(); ++it)
    {
        lookupURL = lookupURL + "%22" + modName + "." + *it + "%22+OR+";
    }
    lookupURL.erase(lookupURL.size() - 4, 4);
    lookupURL += "&btnI";

	wxLaunchDefaultBrowser(lookupURL);

	buttonClickFinish(event);
}

void MainFrame::openReadmeButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	int selected = modManager_.getSelected();
	std::string modsFolderPath = modManager_.getModsFolderPath();
	std::string path;
	ShellRun shellRun;
	bool isSuccess = false;

	path = modsFolderPath + '\\' + modManager_.getMods()[selected].getName() +'\\' + modManager_.getMods()[selected].getReadme();
	isSuccess = shellRun.open(path);

	if (!isSuccess)
		Message::showMessage(Message::MessageType::ERROR_OK, "Couldn't open:\n" + path, shellRun.getErrorMessage());

	buttonClickFinish(event);
}

void MainFrame::selectModArchivesButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	if (!modManager_.getCanExtractArchives())
	{
		buttonClickFinish(event);
		Message::showMessage(Message::MessageType::ERROR_OK, "Cannot open 7z.dll. \nArchive extraction not possible.");
	}

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
	wdCardArchives.Truncate(wdCardArchives.Length() - 2);
	wdCards.RemoveLast();

	std::string forbiddenFolder = config_.game.folderPath + "\\" + config_.game.modsFolder;
	wxArrayString modArchives;

	bool selectionFinished = false;
	int modalVal;

	while (!selectionFinished)
	{
		wxFileDialog genericFileDialog_(this, wxString("Choose mod archive(s) to install."), wxEmptyString, wxEmptyString, wdCardArchives + " files|" + wdCards, wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE);
		modalVal = genericFileDialog_.ShowModal();

		if (modalVal == wxID_CANCEL)
		{
			selectionFinished = true;
			break;
		}
		if (modalVal == wxID_OK)
		{
			if (!Utils::stringIsEqualNoCase(std::string(genericFileDialog_.GetDirectory()), forbiddenFolder))
			{
				selectionFinished = true;
				genericFileDialog_.GetPaths(modArchives);
				break;
			}
			else
			{
				Message::showMessage(Message::MessageType::ERROR_OK, "Do not choose archives from the mod folder. They are installed automatically. \n\nChoose different archives from different folder or click abort.");
			}
		}
	}


	if (!modArchives.IsEmpty())
		extractModArchives(modArchives, true);

	if (fswHasSubpaths_)
	{
		modManager_.scheduleModsFoldersReload();
		scheduleInterfaceUpdate();
	}

	buttonClickFinish(event);
}

void MainFrame::aboutButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	AboutDialog(this).ShowModal();

	buttonClickFinish(event);
}

void MainFrame::exitAndStartGameButtonOnButtonClick(wxCommandEvent& event)
{
	buttonClickStart(event);

	shouldStartGame_ = true;
	Close();

	buttonClickFinish(event);
}

void MainFrame::buttonOnEnterWindow(wxMouseEvent& event)
{
	setTooltip(event.GetId());

	event.Skip();
}

void MainFrame::onListRefresh(wxCommandEvent& WXUNUSED(event))
{
	listRefreshColumnsWidth();
}

void MainFrame::onInterfaceUpdate(wxCommandEvent& WXUNUSED(event))
{
	--queuedInterfaceUpdateEvents_;

	if (queuedInterfaceUpdateEvents_ > 0)
		return;

	queuedInterfaceUpdateEvents_ = 0;

	interfaceUpdate();
}

void MainFrame::onFileSystemWatcherEvent(wxFileSystemWatcherEvent& event)
{
	bool shouldUpdateInterface = fileSystemWatcherProcessEvent(event);

	if (fswHasSubpaths_)
	{
		shouldUpdateInterface = true;
		modManager_.scheduleModsFoldersReload();
	}

	if (shouldUpdateInterface || queuedInterfaceUpdateEvents_ > 0)
		scheduleInterfaceUpdate();
}

void MainFrame::listCtrlOnKeyDown(wxKeyEvent& event)
{
	wxButton* button = nullptr;

	if (!event.HasAnyModifiers())
	{
		switch (event.GetKeyCode())
		{
			case WXK_RETURN:
				button = activateDeactivateButton_;
				break;

			case WXK_SPACE:
				button = pauseResumeButton_;
				break;

			case WXK_F5:
				modManager_.scheduleModsFoldersReload();
				scheduleInterfaceUpdate();
				break;

			case WXK_DELETE:
				button = deleteButton_;
				break;
		}
	}
	else if (event.GetModifiers() == wxMOD_ALT)
	{
		switch (event.GetKeyCode())
		{
			case WXK_UP:
				button = increasePriorityButton_;
				break;

			case WXK_DOWN:
				button = decreasePriorityButton_;
				break;
		}
	}

	if (button)
		buttonClick(button);
	else
		event.Skip();
}

void MainFrame::connectEventFunctions()
{
	Connect(wxEVT_MOVE, wxMoveEventHandler(MainFrame::onMove));
	Connect(REFRESH_EVENT, wxCommandEventHandler(MainFrame::onRefresh));
	Connect(LIST_REFRESH_EVENT, wxCommandEventHandler(MainFrame::onListRefresh));
	Connect(INTERFACE_UPDATE_EVENT, wxCommandEventHandler(MainFrame::onInterfaceUpdate));
	Connect(wxEVT_FSWATCHER, wxFileSystemWatcherEventHandler(MainFrame::onFileSystemWatcherEvent));

	listCtrl_->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MainFrame::listCtrlOnKeyDown), nullptr, this);
}

void MainFrame::initialize()
{
	SetTitle(wxString(ApplicationInfo::full));

	std::array<wxSize, 4> iconSizes =
	{{
		// 16x16 and 32x32 are recommended to be present.
		wxSize(16, 16),
		wxSize(32, 32),

		// These sizes are loaded for good measure.
		wxSize(48, 48),
		wxSize(256, 256)
	}};

	wxIconBundle iconBundle;

	for (const auto& i : iconSizes)
	{
		wxIcon icon("MAINICON", wxBITMAP_TYPE_ICO_RESOURCE, i.GetWidth(), i.GetHeight());

		if (icon.IsOk())
			iconBundle.AddIcon(icon);
	}

	SetIcons(iconBundle);

	SetBackgroundColour(wxPanel().GetBackgroundColour());

	Utils::adjustSizeUp(this, 640);

	size_ = GetSize();
	position_ = GetPosition();

	{
		int x, y;
		if (Utils::stringToPoint(x, y, config_.window.size))
		{
			size_ = wxSize(x, y);
			SetSize(size_);
		}

		if (Utils::stringToPoint(x, y, config_.window.position))
		{
			position_ = wxPoint(x, y);
			SetPosition(position_);
		}
	}

	if (config_.window.isMaximized)
		Maximize();
}

void MainFrame::interfaceInitialize()
{
	if (wxApp::GetComCtl32Version() >= 470)
	{
		HWND listCtrlHwnd = static_cast<HWND>(listCtrl_->GetHWND());

		int listCtrlStyle = LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT;

		if (wxApp::GetComCtl32Version() >= 580)
			listCtrlStyle |= LVS_EX_LABELTIP;

		if (wxApp::GetComCtl32Version() >= 600)
		{
			#ifndef LVS_EX_DOUBLEBUFFER
				#define LVS_EX_DOUBLEBUFFER 0x00010000
			#endif

			listCtrlStyle |= LVS_EX_DOUBLEBUFFER;
		}

		::SendMessage(listCtrlHwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, listCtrlStyle);
	}

	wxSize imageListSize = wxArtProvider::GetSizeHint(wxART_FRAME_ICON);
	imageList_.Create(imageListSize.GetWidth(), imageListSize.GetHeight());
	imageList_.Add(wxArtProvider::GetBitmap(wxART_WARNING, wxART_FRAME_ICON, imageListSize));

	listCtrl_->SetImageList(&imageList_, wxIMAGE_LIST_SMALL);

	listCtrl_->InsertColumn(ID_DUMMY_0_, wxEmptyString);
	listCtrl_->InsertColumn(ID_INDEX_, "#", wxLIST_FORMAT_RIGHT);
	listCtrl_->InsertColumn(ID_NAME_, "Name");
	listCtrl_->InsertColumn(ID_STATUS_, "Status");
	listCtrl_->InsertColumn(ID_TYPE_, "Type");
	listCtrl_->InsertColumn(ID_DUMMY_1_, wxEmptyString);

	{
		columnsCanResize_ = true;

		listCtrl_->DeleteAllItems();

		wxListItem listItem;
		listItem.SetId(0);
		listItem.SetColumn(ID_STATUS_);
		listItem.SetText("Status");

		listCtrl_->InsertItem(0, wxEmptyString);

		listItem.SetImage(-1);
		listCtrl_->SetItem(listItem);
		listCtrl_->SetColumnWidth(ID_STATUS_, wxLIST_AUTOSIZE_USEHEADER);
		int columnWidth = listCtrl_->GetColumnWidth(ID_STATUS_);

		listItem.SetImage(0);
		listCtrl_->SetItem(listItem);
		listCtrl_->SetColumnWidth(ID_STATUS_, wxLIST_AUTOSIZE_USEHEADER);
		int columnWidthWithIcon = listCtrl_->GetColumnWidth(ID_STATUS_);

		listCtrl_->DeleteAllItems();

		if (columnWidthWithIcon > columnWidth)
			columnsIconWidthCorrection_ = 0;
		else
			columnsIconWidthCorrection_ = imageListSize.GetWidth();

		columnsCanResize_ = false;
	}

	columnsLoadOrder();

	shouldSetColumnsWidth_ = true;

	auxDC_.SetFont(listCtrl_->GetFont());

	if (config_.application.showTooltips)
	{
		wxToolTip::Enable(true);
		wxToolTip::SetDelay(500);
		wxToolTip::SetAutoPop(32767); // 32767 is maximum acceptable value.
		wxToolTip::SetReshow(500);
		wxToolTip::SetMaxWidth(activateDeactivateButton_->GetSize().GetWidth() * 2);
	}
}

void MainFrame::interfaceUpdate()
{
	if (modManager_.getHasStateChanged())
	{
		fileSystemWatcherInitialize();

		if (modManager_.getIsInitialized())
		{
			openModsFolderButton_->Enable();
			exitAndStartGameButton_->Enable();
			if (modManager_.getCanExtractArchives())
				selectModArchivesButton_->Enable();
		}
		else
		{
			openModsFolderButton_->Disable();
			exitAndStartGameButton_->Disable();
			selectModArchivesButton_->Disable();
		}
	}

	if (fswShouldReinitialize_)
		fileSystemWatcherInitialize();

	modManager_.update();

	listCtrl_->Freeze();

	listUpdateElements();
	listUpdateSelected();

	if (shouldSetColumnsWidth_)
		listSetColumnsWidth();

	listCtrl_->Thaw();

	editButtonsUpdate();
	statusBarUpdate();
}

void MainFrame::scheduleRefresh()
{
	listCtrl_->Freeze();

	wxCommandEvent* event = new wxCommandEvent(REFRESH_EVENT, GetId());
	GetEventHandler()->QueueEvent(event);
}

void MainFrame::scheduleListRefresh()
{
	wxCommandEvent* event = new wxCommandEvent(LIST_REFRESH_EVENT, GetId());
	GetEventHandler()->QueueEvent(event);
}

void MainFrame::scheduleInterfaceUpdate()
{
	++queuedInterfaceUpdateEvents_;

	wxCommandEvent* event = new wxCommandEvent(INTERFACE_UPDATE_EVENT, GetId());
	GetEventHandler()->QueueEvent(event);
}

void MainFrame::fileSystemWatcherInitialize()
{
	fswShouldReinitialize_ = false;

	fswEventTypes_ = wxFSW_EVENT_CREATE | wxFSW_EVENT_DELETE | wxFSW_EVENT_RENAME;
	fswEventTypes_ |= wxFSW_EVENT_WARNING | wxFSW_EVENT_ERROR;

	fswBasePathLength_ = 0;
	fswHasSubpaths_ = false;

	fileSystemWatcher_.reset(new wxFileSystemWatcher);
	fileSystemWatcher_->SetOwner(this);

	if (!modManager_.getIsInitialized())
		return;

	std::string volumeName(modManager_.getModsFolderPath(), 0, 1);
	volumeName += ":\\";

	const DWORD flagsNTFS = 0x700FF;
	DWORD fileSystemFlags = 0;

	BOOL gviResult = GetVolumeInformationA(
		volumeName.c_str(),
		nullptr,
		0,
		nullptr,
		nullptr,
		&fileSystemFlags,
		nullptr,
		0
	);

	if (gviResult != 0)
		fswHasSubpaths_ = ((fileSystemFlags & flagsNTFS) != flagsNTFS);

	if (fswHasSubpaths_)
	{
		fileSystemWatcher_->AddTree(wxFileName(modManager_.getModsFolderPath() + '\\'), fswEventTypes_);
	}
	else
	{
		fswEventTypes_ |= wxFSW_EVENT_MODIFY;

		fileSystemWatcher_->Add(wxFileName(modManager_.getModsFolderPath() + '\\'), fswEventTypes_);
	}

	wxArrayString watchedPaths;
	fileSystemWatcher_->GetWatchedPaths(&watchedPaths);

	if (watchedPaths.size() != 1)
		return;

	fswBasePathLength_ = watchedPaths[0].length();

	modManager_.scheduleModsFoldersReload();
}

bool MainFrame::fileSystemWatcherProcessEvent(wxFileSystemWatcherEvent& event)
{
	if (!modManager_.getIsInitialized())
		return false;

	int changeType = event.GetChangeType();

	if ((changeType & fswEventTypes_) == 0)
		return false;

	if (changeType == wxFSW_EVENT_ERROR)
	{
		fswShouldReinitialize_ = true;

		return true;
	}

	if (changeType == wxFSW_EVENT_WARNING)
	{
		if (event.GetWarningType() == wxFSW_WARNING_OVERFLOW)
		{
			modManager_.scheduleModsFoldersReload();

			return true;
		}

		return false;
	}

	std::string path(event.GetPath().GetFullPath());
	std::string ext(event.GetPath().GetExt());

	if (path.length() <= fswBasePathLength_)
		return false;

	std::string modName;

	if (fswHasSubpaths_)
	{
		const char sep = '\\';

		size_t modNameBegin = fswBasePathLength_;
		size_t modNameEnd = path.find_first_of(sep, fswBasePathLength_);

		if (modNameEnd != std::string::npos)
			return false;

		modName.assign(path, modNameBegin, modNameEnd - modNameBegin);
	}
	else
	{
		modName = event.GetPath().GetFullName();
	}

	switch (changeType)
	{
		case wxFSW_EVENT_CREATE:
		{
			if (wxDirExists(path))
			{
				bool shouldUpdate = modManager_.directoryAdd(modName);

				return shouldUpdate;
			}

			return false;
		}
		case wxFSW_EVENT_DELETE:
		{
			bool shouldUpdate = modManager_.directoryRemove(modName);

			return shouldUpdate;
		}
		case wxFSW_EVENT_RENAME:
		{
			std::string newModName(event.GetNewPath().GetFullName());
			bool shouldUpdate = modManager_.directoryRename(modName, newModName);

			return shouldUpdate;
		}
		case wxFSW_EVENT_MODIFY:
		{
			//piggybacking modify event here to extract mod archives
			bool shouldUpdate = modManager_.directoryUpdate(modName);

			bool isArchive = (std::find(config_.application.modArchiveExtensions.begin(), config_.application.modArchiveExtensions.end(), ext) != config_.application.modArchiveExtensions.end());
			if (isArchive && wxFileName(path).IsFileReadable() && modManager_.getCanExtractArchives())
			{
				wxArrayString archive;
				archive.Add(wxString(path));
				extractModArchives(archive, true);

				wxRenameFile(path, modManager_.getArchivesFolderPath() + "\\" + modName, true);
				if (config_.application.showExtractTip && !messageDialogOpen_)
				{
					messageDialogOpen_ = true;

					wxString messageString = "All archives added to the " + config_.game.modsFolder + " folder are extracted/installed and then moved to the " + config_.game.archivesFolder + " folder. \nThe contents of " + config_.game.archivesFolder + " are not needed anymore and can be safely deleted.";
					wxRichMessageDialog messageDialogCheck_(this, messageString, wxEmptyString, wxOK); 
					messageDialogCheck_.ShowCheckBox(wxString("Don't show this message again."));
					messageDialogCheck_.ShowModal();
					config_.application.showExtractTip = !messageDialogCheck_.IsCheckBoxChecked();
					
					messageDialogOpen_ = false;
				}
			}

			return shouldUpdate;
		}
	}

	return false;
}

void MainFrame::setTooltip(int windowId)
{
	if (!config_.application.showTooltips)
		return;

	std::string tip, nbs;
	wxWindow* window = nullptr;

	switch (windowId)
	{
		case ID_ACTIVATE_DEACTIVATE_BUTTON :
			tip = "(Double-click list element or Enter)";
			window = activateDeactivateButton_;
			break;

		case ID_INCREASE_PRIORITY_BUTTON :
			tip = "(Alt+Up)";
			window = increasePriorityButton_;
			break;

		case ID_DECREASE_PRIORITY_BUTTON :
			tip = "(Alt+Down)";
			window = decreasePriorityButton_;
			break;

		case ID_PAUSE_RESUME_BUTTON :
			tip = "Pause deactivates a mod while maintaining its order on the list ";
			tip += "(Space)";
			window = pauseResumeButton_;
			break;

		case ID_DELETE_BUTTON:
			tip = "Delete this mod folder from your installation";
			tip += "(Del)";
			window = deleteButton_;
			break;

		case ID_OPEN_MODS_FOLDER_BUTTON :
			tip = "Open \"";
			nbs = config_.game.modsFolder;
			Utils::stringMakeNonBreakingSpaces(nbs);
			tip += nbs;
			tip += "\"";

			if (Utils::stringIsEqualNoCase("DMM", config_.game.modsFolder))
				tip += " (DataManagedMods)";

			tip += " folder in explorer";
			window = openModsFolderButton_;
			break;

		case ID_SELECT_GAME_FOLDER_BUTTON :
			tip = "Browse for the folder that contains ";
			nbs = "System Shock 2";
			Utils::stringMakeNonBreakingSpaces(nbs);
			tip += nbs;
			window = selectGameFolderButton_;
			break;

		case ID_OPEN_MOD_URL_BUTTON:
			tip = "Lookup ";
			nbs = modManager_.getMods()[modManager_.getSelected()].getName();
			Utils::stringMakeNonBreakingSpaces(nbs);
			tip += nbs;
			tip += " mod on Systemshock.org";
			window = openModURLButton_;
			break;

		case ID_OPEN_README_BUTTON:
			tip = "Open readme file for ";
			nbs = modManager_.getMods()[modManager_.getSelected()].getName();
			Utils::stringMakeNonBreakingSpaces(nbs);
			tip += nbs;
			tip += " mod";
			window = openReadmeButton_;
			break;

		case ID_EXTRACT_MOD_BUTTON:
			tip = "Browse for System Shock 2 mod archives to install";
			window = selectModArchivesButton_;
			break;

		case ID_ABOUT_BUTTON :
			tip = "About ";
			nbs = ApplicationInfo::name;
			Utils::stringMakeNonBreakingSpaces(nbs);
			tip += nbs;
			window = aboutButton_;
			break;

		case ID_EXIT_AND_START_GAME_BUTTON :
			tip = "Exit and start game with current setup";
			window = exitAndStartGameButton_;
			break;
	}

	if (window && !tip.empty())
		window->SetToolTip(wxString(tip));
}

void MainFrame::listUpdateElements()
{
	assert(listCtrl_->IsFrozen());

	listSetItemCount(modManager_.getModCount());
	auxNameWidths_.resize(modManager_.getModCount(), -1);

	bool listChanged = false;
	for (int i = 0; i < listCtrl_->GetItemCount(); ++i)
	{
		if (modManager_.getMods()[i].getIsChanged())
		{
			listUpdateElement(i);
			auxNameWidths_[i] = auxDC_.GetTextExtent(modManager_.getMods()[i].getName()).GetWidth();

			listChanged = true;
		}
	}

	if (listChanged)
		shouldSetColumnsWidth_ = true;
}

void MainFrame::listUpdateSelected()
{
	assert(listCtrl_->IsFrozen());

	const int selectedFlag = wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED;

	int selected = modManager_.getSelected();

	if (listCtrl_->GetItemCount() == 0)
		return;

	if (selected > listCtrl_->GetItemCount() - 1)
		return;

	if (selected < 0)
	{
		for (int i = 0; i < listCtrl_->GetItemCount(); ++i)
		{
			if (listCtrl_->GetItemState(i, selectedFlag) != 0)
				listCtrl_->SetItemState(i, 0, selectedFlag);
		}

		return;
	}

	if (listCtrl_->GetItemState(selected, selectedFlag) == selectedFlag)
		return;

	listCtrl_->SetItemState(selected, selectedFlag, selectedFlag);
	listCtrl_->EnsureVisible(selected);
}

void MainFrame::editButtonsUpdate()
{
	activateDeactivateButton_->Enable(modManager_.canActivateDeactivate());
	increasePriorityButton_->Enable(modManager_.canIncreasePriority());
	decreasePriorityButton_->Enable(modManager_.canDecreasePriority());
	pauseResumeButton_->Enable(modManager_.canPauseResume());
	deleteButton_->Enable(modManager_.canActivateDeactivate());
	openModURLButton_->Enable(modManager_.canActivateDeactivate());
	openReadmeButton_->Enable(modManager_.canOpenReadme());
}

void MainFrame::statusBarUpdate()
{
	int warningCount = modManager_.getWarningCount();
	int activeOrPausedCount = modManager_.getActiveOrPausedCount();
	int activeGamesysCount = modManager_.getActiveGamesysCount();

	if (!modManager_.getIsInitialized())
	{
		statusBarManipulator_.setStatus(StatusBarStatus::GAME_FOLDER_NOT_SELECTED);

		return;
	}
	if (modManager_.getModCount() < 1)
	{
		statusBarManipulator_.setStatus(StatusBarStatus::NO_MODS_DETECTED);

		return;
	}
	if (modManager_.getSelected() > -1)
	{
		Mod& mod = modManager_.getMods()[modManager_.getSelected()];

		if (mod.getIsUnrecognized())
		{
			statusBarManipulator_.setStatus(StatusBarStatus::MOD_TYPE_UNRECOGNIZED);

			return;
		}
		if (mod.getIsNameTooLong())
		{
			statusBarManipulator_.setStatus(StatusBarStatus::FOLDER_NAME_TOO_LONG);

			return;
		}
		if (mod.getIsPlusSignInName())
		{
			statusBarManipulator_.setStatus(StatusBarStatus::PLUS_SIGN_IN_FOLDER_NAME);

			return;
		}
	}
	if (activeGamesysCount > 1)
	{
		statusBarManipulator_.setStatus(StatusBarStatus::MULTIPLE_GAMESYS_MODS_ACTIVE);

		return;
	}
	if (activeOrPausedCount > config_.game.maxActive)
	{
		statusBarManipulator_.setStatus(StatusBarStatus::TOO_MANY_MODS_ACTIVE);

		return;
	}
	if (warningCount > 0)
	{
		statusBarManipulator_.setStatus(StatusBarStatus::ISSUES_DETECTED);

		return;
	}

	statusBarManipulator_.setStatus(StatusBarStatus::OK);
}

void MainFrame::listSetItemCount(int itemCount)
{
	assert(listCtrl_->IsFrozen());

	if (listCtrl_->GetItemCount() == itemCount)
		return;

	if (itemCount == 0)
	{
		listCtrl_->DeleteAllItems();
	}
	else if (listCtrl_->GetItemCount() < itemCount)
	{
		int lastItem = listCtrl_->GetItemCount() - 1;

		while (listCtrl_->GetItemCount() < itemCount)
		{
			listCtrl_->InsertItem(++lastItem, wxEmptyString);

			wxListItem listItem;
			listItem.SetId(lastItem);
			listItem.SetColumn(0);
			listItem.SetImage(-1);
			listCtrl_->SetItem(listItem);
		}
	}
	else if (listCtrl_->GetItemCount() > itemCount)
	{
		int tempSelected = modManager_.getSelected();

		int lastItem = listCtrl_->GetItemCount() - 1;

		while (listCtrl_->GetItemCount() > itemCount)
			listCtrl_->DeleteItem(lastItem--);

		modManager_.setSelected(tempSelected);
	}

	shouldSetColumnsWidth_ = true;
}

void MainFrame::listUpdateElement(int index)
{
	assert(listCtrl_->IsFrozen());

	static const wxColour backgroundActiveColor = wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX);
	static const wxColour backgroundInactiveColor = wxColour(backgroundActiveColor).MakeDisabled(222);

	Mod& mod(modManager_.getMods()[index]);

	if (mod.getIsActive() && !mod.getIsPaused())
		listCtrl_->SetItemBackgroundColour(index, backgroundActiveColor);
	else
		listCtrl_->SetItemBackgroundColour(index, backgroundInactiveColor);

	if (mod.getIsActive())
		listCtrl_->SetItem(index, ID_INDEX_, wxString() << index + 1);
	else
		listCtrl_->SetItem(index, ID_INDEX_, wxEmptyString);

	listCtrl_->SetItem(index, ID_NAME_, mod.getName());

	wxString status;
	if (mod.getIsPaused())
		status = "Paused";
	else if (mod.getIsActive())
		status = "Active";
	else
		status = "Inactive";

	wxListItem listItem;
	listItem.SetId(index);
	listItem.SetColumn(ID_STATUS_);
	listItem.SetText(status);

	if (mod.getHasWarning())
		listItem.SetImage(0);
	else
		listItem.SetImage(-1);

	listCtrl_->SetItem(listItem);

	listCtrl_->SetItem(index, ID_TYPE_, mod.getType());
}

void MainFrame::listRefreshColumnsWidth()
{
	static const int scrollHeight = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y);

	if (columnsTotalWidth_ < 0 || columnsBaseNameWidth_ < 0)
		return;

	wxSize listSize = listCtrl_->GetClientSize();

	if (listSize.GetHeight() <= scrollHeight)
		return;

	int nameWidth = columnsBaseNameWidth_;

	if (columnsTotalWidth_ < listSize.GetWidth())
		nameWidth += listSize.GetWidth() - columnsTotalWidth_;

	if (listCtrl_->GetColumnWidth(ID_NAME_) != nameWidth)
	{
		columnsCanResize_ = true;
		listCtrl_->SetColumnWidth(ID_NAME_, nameWidth);
		columnsCanResize_ = false;
	}
}

void MainFrame::listSetColumnsWidth()
{
	assert(listCtrl_->IsFrozen());

	shouldSetColumnsWidth_ = false;

	const int columnCount = listCtrl_->GetColumnCount();

	bool isDummiesOrderOK = (listCtrl_->GetColumnIndexFromOrder(columnCount - 1) == ID_DUMMY_0_)
		&& (listCtrl_->GetColumnIndexFromOrder(columnCount - 2) == ID_DUMMY_1_);

	if (!isDummiesOrderOK)
	{
		columnsSaveOrder();
		columnsLoadOrder();
	}

	columnsCanResize_ = true;

	listCtrl_->SetColumnWidth(ID_DUMMY_0_, 0);
	listCtrl_->SetColumnWidth(ID_INDEX_, wxLIST_AUTOSIZE_USEHEADER);
	listCtrl_->SetColumnWidth(ID_STATUS_, wxLIST_AUTOSIZE_USEHEADER);
	listCtrl_->SetColumnWidth(ID_TYPE_, wxLIST_AUTOSIZE_USEHEADER);
	listCtrl_->SetColumnWidth(ID_DUMMY_1_, 0);

	if ((columnsIconWidthCorrection_ > 0) && (modManager_.getWarningCount() > 0))
		listCtrl_->SetColumnWidth(ID_STATUS_, listCtrl_->GetColumnWidth(ID_STATUS_) + columnsIconWidthCorrection_);

	int auxNameWidth = 0;
	for (auto& i : auxNameWidths_)
		auxNameWidth = std::max(auxNameWidth, i);

	if (auxNameWidth != auxLastNameWidth_)
	{
		listCtrl_->SetColumnWidth(ID_NAME_, wxLIST_AUTOSIZE_USEHEADER);
		columnsBaseNameWidth_ = listCtrl_->GetColumnWidth(ID_NAME_);
		auxLastNameWidth_ = auxNameWidth;
	}

	columnsCanResize_ = false;

	columnsTotalWidth_ = 0;

	for (int i = 0; i < columnCount; ++i)
	{
		if (i == ID_NAME_)
			continue;

		columnsTotalWidth_ += listCtrl_->GetColumnWidth(i);
	}

	columnsTotalWidth_ += columnsBaseNameWidth_;

	listRefreshColumnsWidth();
}

void MainFrame::buttonClick(wxButton* button)
{
	if (button && button->IsEnabled())
	{
		wxCommandEvent* buttonEvent = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, button->GetId());
		buttonEvent->SetEventObject(static_cast<wxObject*>(button));
		button->GetEventHandler()->QueueEvent(buttonEvent);
	}
}

void MainFrame::buttonClickStart(wxCommandEvent& event)
{
	wxButton* button = static_cast<wxButton*>(event.GetEventObject());

	if (config_.application.showTooltips)
		button->UnsetToolTip();

	listCtrl_->SetFocus();
}

void MainFrame::buttonClickFinish(wxCommandEvent& event)
{
	scheduleInterfaceUpdate();
	event.Skip();
}

void MainFrame::columnsLoadOrder()
{
	const int columnCount = listCtrl_->GetColumnCount();
	const int lastOrder = columnCount - 3;

	wxArrayInt order(DEFAULT_ORDER_.cbegin(), DEFAULT_ORDER_.cend());
	assert(columnCount == static_cast<int>(order.size()));

	for (int i = 0; i <= lastOrder; ++i)
		order[i] = -1;

	std::string orderSequence = config_.window.listColumnOrder;
	std::string columnName;

	int i = 0;
	while (i <= lastOrder && Utils::tokenize(columnName, orderSequence, orderSequence))
	{
		int columnIndex = getColumnIndex(columnName);

		if (std::find(order.cbegin(), order.cend(), columnIndex) == order.cend())
			order[i] = columnIndex;

		++i;
	}

	bool isOrderOK = false;
	if (std::find(order.cbegin(), order.cend(), -1) == order.cend())
		isOrderOK = true;

	if (!isOrderOK)
		order.assign(DEFAULT_ORDER_.cbegin(), DEFAULT_ORDER_.cend());

	listCtrl_->SetColumnsOrder(order);

	assert(listCtrl_->GetColumnIndexFromOrder(columnCount - 1) == ID_DUMMY_0_);
	assert(listCtrl_->GetColumnIndexFromOrder(columnCount - 2) == ID_DUMMY_1_);
}

void MainFrame::columnsSaveOrder()
{
	wxArrayInt order = listCtrl_->GetColumnsOrder();

	std::string orderSequence;
	std::string columnName;

	for (int i = 0; i < listCtrl_->GetColumnCount(); ++i)
	{
		columnName = getColumnName(order[i]);

		if (!columnName.empty())
		{
			orderSequence += " ";
			orderSequence += columnName;
		}
	}

	Utils::stringTrim(orderSequence);

	config_.window.listColumnOrder = orderSequence;
}

int MainFrame::getColumnIndex(const std::string& name)
{
	for (int i = 0; i < listCtrl_->GetColumnCount(); ++i)
	{
		if (Utils::stringIsEqualNoCase(name, getColumnName(i)))
			return i;
	}

	return -1;
}

std::string MainFrame::getColumnName(int index)
{
	std::string name;

	switch (index)
	{
		case ID_INDEX_ :
			name = "index";
			break;

		case ID_NAME_ :
			name = "name";
			break;

		case ID_STATUS_ :
			name = "status";
			break;

		case ID_TYPE_ :
			name = "type";
			break;
	}

	return name;
}

void MainFrame::loadInitialModArchives()
{

	if (!modManager_.getCanExtractArchives())
		return;

	wxDir dir;
	wxString modsFolder = modManager_.getModsFolderPath();
	bool extractionTookPlace = false;

	for (auto& archiveType : config_.application.modArchiveExtensions)
	{
		wxArrayString modArchives;
		size_t isFound = dir.GetAllFiles(modsFolder, &modArchives, wxString("*." + archiveType), wxDIR_FILES);

		if (isFound > 0)
		{
			extractionTookPlace = true;
			extractModArchives(modArchives, true);
			for (auto& archiveToMove : modArchives)
				if (wxFileName(archiveToMove).IsFileWritable())
					wxRenameFile(archiveToMove, modManager_.getArchivesFolderPath() + "\\" + wxFileName(archiveToMove).GetFullName(), true);

		}
	}

	if (config_.application.showExtractTip && !messageDialogOpen_ && extractionTookPlace)
	{
		messageDialogOpen_ = true;

		wxString messageString = "All archives added to the " + config_.game.modsFolder + " folder are extracted/installed and then moved to the " + config_.game.archivesFolder + " folder. \nThe contents of " + config_.game.archivesFolder + " are not needed anymore and can be safely deleted.";
		wxRichMessageDialog messageDialogCheck_(this, messageString, wxEmptyString, wxOK);
		messageDialogCheck_.ShowCheckBox(wxString("Don't show this message again."));
		messageDialogCheck_.ShowModal();
		config_.application.showExtractTip = !messageDialogCheck_.IsCheckBoxChecked();

		messageDialogOpen_ = false;
	}

}

void MainFrame::extractModArchives(const wxArrayString &modArchives, bool showProgress)
{
	wxString pathDLL = config_.game.folderPath + "\\7z.dll";
	wxString pathModFolder = modManager_.getModsFolderPath();

	wxArrayString failedMods;
	wxString pathExtract;

	bool isSuccess;
	float progressCounter = 1;
	SevenZip::SevenZipLibrary lib_7zdll;
	bool loadDLL = lib_7zdll.Load(pathDLL.ToStdWstring());

	if (!loadDLL)
	{
		pathDLL = config_.application.executableFolderPath + "\\7z.dll";
		loadDLL = lib_7zdll.Load(pathDLL.ToStdWstring());
		if (!loadDLL)
		{
			Message::showMessage(Message::MessageType::ERROR_OK, "Couldn't load 7z.dll .\n");
			return;
		}
	}

	wxProgressDialog progressBar_(wxString("Extracting mod archives..."), wxEmptyString, 1000, this, wxPD_SMOOTH | wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_REMAINING_TIME);
	if (!showProgress)
	{
		progressBar_.Update(1000, wxEmptyString, NULL);
		progressBar_.Hide();
	}


	for (auto& archiveName : modArchives)
	{
		if (showProgress)
		{
			progressBar_.Update((int)(progressCounter / (float)(modArchives.Count()) * 1000 * 0.99), wxString("Extracting mod " + wxFileName(archiveName).GetName()), NULL);
			progressBar_.Show();
		}

		pathExtract = pathModFolder + "\\" + wxFileName(archiveName).GetName();
		SevenZip::SevenZipExtractor modExtractor(lib_7zdll, archiveName.ToStdWstring());

		if (modExtractor.DetectCompressionFormat())
		{
			isSuccess = modExtractor.ExtractArchive(pathExtract.ToStdWstring(), nullptr);
			if (!isSuccess)
				failedMods.Add(archiveName + "\n");
		}
		else
		{
			failedMods.Add(archiveName + "\n");
		}

		progressCounter++;
	}
	if (showProgress)
		progressBar_.Update(1000, wxString("Done."), NULL);


	if (!failedMods.IsEmpty())
	{
		std::string error_output;
		for (auto& failed : failedMods)
			error_output = error_output + failed;

		Message::showMessage(Message::MessageType::ERROR_OK, "Couldn't extract mod archives:\n", error_output);
	}


	lib_7zdll.Free();
}
