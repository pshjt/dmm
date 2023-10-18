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
		bool createLogFile;
		bool showTooltips;
		bool showExtractTip;
		std::string version;
	} application;

	struct
	{
		int maxFolderLength;
		int maxFullPathLength;
		int maxPathCount;
		int maxActive;
		std::string executableFile; // *.exe filename, e.g. ss2.exe
		std::vector<std::string> additionalExecutableFiles;
		std::string modsConfigFile; // main file to write full mod path to
		std::string installConfigFile; // file to write base path info to
		std::vector<std::string> additionalInstallConfigFiles;
		std::string pauseIndicator; // char indicating paused mod present in path to be skipped during evaluation
		std::string modsPathPrefix; // prefix for full mod path
		std::string modsPathSuffix; // suffix for full mod path
		std::string baseMoviePath; // path to original movie folders
		std::vector<std::string> cutsceneFolders; // allowed folder names for (mod) cutscene folders
		std::string subtitleFolder; // allowed folder name for mod subtitle folder
		std::string folderPath; // folder path of executable file
		std::string modsFolder; // name of folder containing all mods folders
		std::string archivesFolder; // name of folder containing installed mod archives to move to
		std::string dataOverrideSubfolder; // if not empty, manager will recognize this subfolder as an override for each individual mod so that a mod may ship for multiple games. Example: dataOverrideSubfolder "Thief1" -> mod_A gets added with "...\mod_A\Thief1 + ...\mod_A\"
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

		application.configFile = "dmm.cfg";
		application.logFile = "dmm.log";
		application.lookupURL = "https://www.google.com/search?q=";
		application.modArchiveExtensions.assign({ "7z", "zip", "rar", "crf" });

		game.maxFolderLength = 30;
		game.maxFullPathLength = 255;
		game.maxPathCount = 63;
		game.maxActive = 0; // Computed while loading mods setup.
		game.additionalExecutableFiles.assign({ "shock2.exe", "ss2.exe", "thief.exe", "thief2.exe" });
		game.additionalInstallConfigFiles.assign({ "install.cfg", "darkinst.cfg" }); // install.cfg for SS2/Thief, darkinst.cfg for Thief2
		game.modsConfigFile = "cam_mod.ini";
		game.pauseIndicator = "*";
		game.cutsceneFolders.assign({ "cutscenes", "movies" }); // cutscenes for SS2, movies for Thief games
		game.subtitleFolder = "subtitles";

		configVariables();
	}

private:
	void configVariables() override
	{
		CVAR(application.createLogFile, true);
		CVAR(application.showTooltips, true);
		CVAR(application.showExtractTip, true);
		CVAR(application.version, "");

		CVAR(game.modsPathSuffix, ".\\patch_ext");
		CVAR(game.modsPathPrefix, "");
		CVAR(game.baseMoviePath, ".\\cutscenes+.\\Data\\cutscenes");
		CVAR(game.folderPath, "");
		CVAR(game.executableFile, "shock2.exe");
		CVAR(game.installConfigFile, "install.cfg");
		CVAR(game.modsFolder, "DMM");
		CVAR(game.archivesFolder, "DMMArchives");
		CVAR(game.dataOverrideSubfolder, "")

		CVAR(window.isMaximized, false);
		CVAR(window.listColumnOrder, "");
		CVAR(window.position, "");
		CVAR(window.size, "");
	}
};