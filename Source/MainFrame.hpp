// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "ApplicationConfig.hpp"
#include "ModManager.hpp"
#include "StatusBarManipulator.hpp"

class MainFrame : public WxfbMainFrame
{
public:
	MainFrame(wxWindow* parent);

private:
	virtual void onClose(wxCloseEvent& event);
	virtual void onSize(wxSizeEvent& event);
	void onMove(wxMoveEvent& event);
	void onRefresh(wxCommandEvent& event);

	virtual void statusBarOnSize(wxSizeEvent& event);

	virtual void listCtrlOnKillFocus(wxFocusEvent& event);
	virtual void listCtrlOnListColBeginDrag(wxListEvent& event);
	virtual void listCtrlOnListColDragging(wxListEvent& event);
	virtual void listCtrlOnListItemActivated(wxListEvent& event);
	virtual void listCtrlOnListItemDeselected(wxListEvent& event);
	virtual void listCtrlOnListItemSelected(wxListEvent& event);
	virtual void listCtrlOnSize(wxSizeEvent& event);

	virtual void activateDeactivateButtonOnButtonClick(wxCommandEvent& event);
	virtual void increasePriorityButtonOnButtonClick(wxCommandEvent& event);
	virtual void decreasePriorityButtonOnButtonClick(wxCommandEvent& event);
	virtual void pauseResumeButtonOnButtonClick(wxCommandEvent& event);
	virtual void deleteButtonOnButtonClick(wxCommandEvent& event);
	virtual void openModsFolderButtonOnButtonClick(wxCommandEvent& event);
	virtual void selectGameFolderButtonOnButtonClick(wxCommandEvent& event);
	virtual void openModURLButtonOnButtonClick(wxCommandEvent& event);
	virtual void openReadmeButtonOnButtonClick(wxCommandEvent& event);
	virtual void selectModArchivesButtonOnButtonClick(wxCommandEvent& event);
	virtual void aboutButtonOnButtonClick(wxCommandEvent& event);
	virtual void applyButtonOnButtonClick(wxCommandEvent& event);
	virtual void applyAndStartGameButtonOnButtonClick(wxCommandEvent& event);
	virtual void buttonOnEnterWindow(wxMouseEvent& event);

	void onListRefresh(wxCommandEvent& event);
	void onInterfaceUpdate(wxCommandEvent& event);
	void onFileSystemWatcherEvent(wxFileSystemWatcherEvent& event);
	void listCtrlOnKeyDown(wxKeyEvent& event);

	void connectEventFunctions();
	void initialize();
	void interfaceInitialize();
	void interfaceUpdate();
	void scheduleRefresh();
	void scheduleListRefresh();
	void scheduleInterfaceUpdate();

	void fileSystemWatcherInitialize();
	bool fileSystemWatcherProcessEvent(wxFileSystemWatcherEvent& event);

	void setTooltip(int windowId);

	void listUpdateElements();
	void listUpdateSelected();
	void editButtonsUpdate();

	void statusBarUpdate();

	void listSetItemCount(int itemCount);
	void listUpdateElement(int index);
	void listRefreshColumnsWidth();
	void listSetColumnsWidth();

	void buttonClick(wxButton* button);
	void buttonClickStart(wxCommandEvent& event);
	void buttonClickFinish(wxCommandEvent& event);

	void columnsLoadOrder();
	void columnsSaveOrder();
	int getColumnIndex(const std::string& name);
	std::string getColumnName(int index);

	void loadInitialModArchives();
	void extractModArchives(const wxArrayString& modArchives, bool showProgress);

	static const int ID_DUMMY_0_;
	static const int ID_INDEX_;
	static const int ID_NAME_;
	static const int ID_STATUS_;
	static const int ID_TYPE_;
	static const int ID_DUMMY_1_;
	static const std::vector<int> DEFAULT_ORDER_;

	const ApplicationConfig defaultConfig_;
	ApplicationConfig config_;
	ModManager modManager_;

	int queuedInterfaceUpdateEvents_;
	bool shouldSetColumnsWidth_;
	bool shouldStartGame_;

	bool messageDialogOpen_;

	wxSize size_;
	wxPoint position_;
	bool isMaximized_;

	std::unique_ptr<wxFileSystemWatcher> fileSystemWatcher_;
	bool fswShouldReinitialize_;
	int fswEventTypes_;
	size_t fswBasePathLength_;
	bool fswHasSubpaths_;

	wxImageList imageList_;
	int columnsIconWidthCorrection_;
	int columnsTotalWidth_;
	int columnsBaseNameWidth_;
	bool columnsCanResize_;

	wxMemoryDC auxDC_;
	std::vector<int> auxNameWidths_;
	int auxLastNameWidth_;

	StatusBarManipulator statusBarManipulator_;
};