// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "Config.hpp"

class ApplicationConfig : public Config
{
public:
	struct
	{
		std::string executableFolderPath;
		std::string executableFile;
		std::string configFile;
		std::string logFile;
		std::string lookupURL;
		std::vector<std::string> modArchiveExtensions;
#pragma warning( push )
#pragma warning( disable : 26495 )
		bool checkGameVersion;
		bool createLogFile;
		bool showTooltips;
		bool showExtractTip;
#pragma warning( pop )
		std::string version;
	} application;

	struct
	{
		int maxFolderLength;
		int maxFullPathLength;
		int maxPathCount;
		int maxActive;
		std::string executableFile;
		std::string currentExecutableFile;
		std::vector<std::string> additionalExecutableFiles;
		std::string productName;
		std::string requiredVersion;
		std::string modsConfigFile;
		std::string installConfigFile;
		std::string pauseIndicator;
		std::string baseModPath;
		std::string baseMoviePath;
		std::string cutsceneFolder;
		std::string subtitleFolder;
		std::string folderPath;
		std::string modsFolder;
		std::string archivesFolder;
	} game;

	struct
	{
		bool isMaximized;
		std::string listColumnOrder;
		std::string position;
		std::string size;
	} window;

	ApplicationConfig()
	{
		wxString path, name, ext;
		wxFileName::SplitPath(wxStandardPaths::Get().GetExecutablePath(), &path, &name, &ext);

		if (ext != wxEmptyString)
			name += "." + ext;

		application.executableFolderPath = std::string(path);
		application.executableFile = std::string(name);

		application.configFile = "ss2bmm.cfg";
		application.logFile = "ss2bmm.log";
		application.lookupURL = "https://www.google.com/search?q=site:systemshock.org+";
		application.modArchiveExtensions.assign({ "7z", "zip", "rar", "ss2mod", "cfg" });

		game.maxFolderLength = 30;
		game.maxFullPathLength = 255;
		game.maxPathCount = 63;
		game.maxActive = 0; // Computed while loading mods setup.
		game.executableFile = "Shock2.exe";
		game.additionalExecutableFiles.assign({ "ss2.exe" }); // List of other accepted executables
		game.productName = "System Shock 2";
		game.requiredVersion = "2.4";
		game.modsConfigFile = "cam_mod.ini";
		game.installConfigFile = "install.cfg";
		game.pauseIndicator = "*";
		game.cutsceneFolder = "cutscenes";
		game.subtitleFolder = "subtitles";

		configVariables();
	}

private:
	virtual void configVariables()
	{
		CVAR(application.checkGameVersion, true);
		CVAR(application.createLogFile, true);
		CVAR(application.showTooltips, true);
		CVAR(application.showExtractTip, true);
		CVAR(application.version, "");

		CVAR(game.baseModPath, ".\\patch_ext");
		CVAR(game.baseMoviePath, ".\\cutscenes+.\\Data\\cutscenes");
		CVAR(game.folderPath, "");
		CVAR(game.modsFolder, "DMM");
		CVAR(game.archivesFolder, "DMMArchives");

		CVAR(window.isMaximized, false);
		CVAR(window.listColumnOrder, "");
		CVAR(window.position, "");
		CVAR(window.size, "");
	}
};