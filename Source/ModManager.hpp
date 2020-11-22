// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "ApplicationConfig.hpp"
#include "ModConfig.hpp"
#include "InstallConfig.hpp"
#include "Mod.hpp"

class ModManager
{
public:
	ModManager(ApplicationConfig& config);
	// ~ModManager() {};

	void initialize(bool saveToFile = true);
	void initialize(const std::string& gamePath, bool saveToFile = true, bool forceRefresh = false);
	void shutDown(bool saveToFile = true);
	void refreshModListAndSaveToFile(bool saveToFile = true);
	bool loadModsConfig(bool saveModList = true, const std::string& modPathOverride = std::string());
	void loadNewModPath(const std::string& modPath);

	std::string constructMoviePath(bool updateConfig);
	std::string constructAndUpdateModPath(bool updateConfig);
	std::tuple<std::string, std::string> constructModPath() const;
	std::tuple<std::string, std::string> constructMoviePath() const;

	void setSelected(int selected);

	bool getHasStateChanged();
	bool getIsInitialized() const;
	void scheduleModsFoldersReload();
	void update();
	int getSelected() const;
	const std::string& getModsFolderPath() const;
	const std::string& getArchivesFolderPath() const;
	const std::string getAlternativeExecutable(std::string) const;
	std::vector<Mod>& getMods();
	void createModsTable(std::string& modsTable) const;
	int calculateLastActiveIndex() const;
	void resetStates();

	const bool hasModWithName(const std::string& modName) const;
	bool canActivateDeactivate() const;
	bool canIncreasePriority() const;
	bool canDecreasePriority() const;
	bool canPauseResume() const;
	bool canOpenReadme() const;
	bool needsToApply();

	void activateDeactivate();
	void increasePriority();
	void decreasePriority();
	// Moves the mod at sourceIndex to targetIndex. If the mod at targetIndex is inactive, moves to the position of the last active mod.
	// Returns: index the source was moved to
	int movePriority(int sourceIndex, int targetIndex);
	void swapPriorities(int idx1, int idx2);
	void pauseResume();

	int getModCount() const;
	int getWarningCount() const;
	int getActiveOrPausedCount() const;
	int getActiveGamesysCount() const;

	bool directoryAdd(const std::string& name);
	bool directoryRemove(const std::string& name);
	bool directoryRename(const std::string& name, const std::string& newName);
	bool directoryUpdate(const std::string& name);

private:
	class PredicateIsModNameEqualNoCase
	{
	public:
		PredicateIsModNameEqualNoCase(const std::string& str) : str_(str)
		{
		}

		bool operator ()(const Mod& mod)
		{
			return Utils::stringIsEqualNoCase(str_, mod.getName());
		}

	private:
		std::string str_;
	};

	// Non copyable.
	ModManager(const ModManager&); // Not to implement.
	ModManager& operator =(const ModManager&); // Not to implement.

	void setIsInitialized(bool isInitialized);

	void loadModsFolders();
	void computeMaxActive();
	void activate(int index);
	void deactivate(int index);
	void scheduleSetModTypeAll();
	void setModName(Mod& mod, const std::string& name) const;
	void setModType(Mod& mod);
	void setModReadme(Mod& mod) const;
	void checkModDirectory(Mod& mod);
	void sortInactives();
	void selectByIndex();
	void countStats();
	void collectMisFileToModMapping(Mod& mod, std::unordered_map<std::string, std::vector<Mod*>>& misFileToMods);
	void createLogFile();

	static const std::array<wxString, 16> dataDirectories_;
	static const std::array<wxString, 8> dataFiles_;

	ApplicationConfig& config_;
	ModConfig modConfig_;
	InstallConfig installConfig_;
	std::vector<Mod> mods_;

	bool isInitialized_;
	bool hasStateChanged_;

	std::string gameVersion_;
	int selected_;
	bool shouldReloadModsFolders_;
	bool shouldSortInactives_;

	int warningCount_;
	int activeOrPausedCount_;
	int activeGamesysCount_;

	const std::string logFilePath_;
	std::string modsFolderPath_;
	std::string archivesFolderPath_;
};