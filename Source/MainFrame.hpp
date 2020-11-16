// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "ApplicationConfig.hpp"
#include "ModManager.hpp"
#include "ModExtractor.hpp"
#include "StatusBarManipulator.hpp"

class MainFrame : public WxfbMainFrame
{
public:
	MainFrame(wxWindow* parent);

private:
	void onClose(wxCloseEvent& event) override;
	void onSize(wxSizeEvent& event) override;
	void onMove(wxMoveEvent& event);
	void onRefresh(wxCommandEvent& event);

	void statusBarOnSize(wxSizeEvent& event) override;

	void listCtrlOnKillFocus(wxFocusEvent& event) override;
	void listCtrlOnListColBeginDrag(wxListEvent& event) override;
	void listCtrlOnListColDragging(wxListEvent& event) override;
	void listCtrlOnListItemActivated(wxListEvent& event) override;
	void listCtrlOnListItemDeselected(wxListEvent& event) override;
	void listCtrlOnListItemSelected(wxListEvent& event) override;
	void listCtrlOnSize(wxSizeEvent& event) override;

	void activateDeactivateButtonOnButtonClick(wxCommandEvent& event) override;
	void increasePriorityButtonOnButtonClick(wxCommandEvent& event) override;
	void decreasePriorityButtonOnButtonClick(wxCommandEvent& event) override;
	void pauseResumeButtonOnButtonClick(wxCommandEvent& event) override;
	void deleteButtonOnButtonClick(wxCommandEvent& event) override;
	void openModsFolderButtonOnButtonClick(wxCommandEvent& event) override;
	void selectGameFolderButtonOnButtonClick(wxCommandEvent& event) override;
	void openModURLButtonOnButtonClick(wxCommandEvent& event) override;
	void openReadmeButtonOnButtonClick(wxCommandEvent& event) override;
	void selectModArchivesButtonOnButtonClick(wxCommandEvent& event) override;
	void aboutButtonOnButtonClick(wxCommandEvent& event) override;
	void applyButtonOnButtonClick(wxCommandEvent& event) override;
	void applyAndStartGameButtonOnButtonClick(wxCommandEvent& event) override;
	void buttonOnEnterWindow(wxMouseEvent& event) override;

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
	ModExtractor modExtractor_;

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
