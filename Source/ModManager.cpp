#include <Precompiled.hpp>

#include "Application.hpp"
#include "Message.hpp"
#include "Utils.hpp"

#include "ModManager.hpp"

ModManager::ModManager(ApplicationConfig& config)
	: config_(config)
	, shouldReloadModsFolders_(false)
	, shouldSortInactives_(false)
	, warningCount_(0)
	, activeOrPausedCount_(0)
	, activeGamesysCount_(0)
	, logFilePath_(config_.application.executableFolderPath + '\\' + config_.application.logFile)
{
	setIsInitialized(false);
}

void ModManager::initialize(bool saveToFile)
{
	if (getIsInitialized())
		return;

	if (wxFileExists(logFilePath_))
		wxRemoveFile(logFilePath_);

	std::vector<Mod>().swap(mods_);
	std::vector<int>().swap(allSelected_);

	std::string gameFolderPath = config_.game.folderPath;
	Utils::normalizePath(gameFolderPath);

	config_.game.folderPath = "";

	if (!wxDirExists(gameFolderPath))
	{
		gameFolderPath = config_.application.executableFolderPath;
		Utils::normalizePath(gameFolderPath);
	}

	std::string exePath = gameFolderPath + '\\' + config_.game.executableFile;
	if (!wxFileExists(exePath))
	{
		std::string alternativeExe = getAlternativeFile(gameFolderPath, config_.game.additionalExecutableFiles);
		if (alternativeExe.empty())
			return;

		exePath = gameFolderPath + '\\' + alternativeExe;
		config_.game.executableFile = alternativeExe;
	}

	std::string installConfigFile = gameFolderPath + '\\' + config_.game.installConfigFile;
	if (!wxFileExists(installConfigFile))
	{
		std::string alternativeInstallConfig = getAlternativeFile(gameFolderPath, config_.game.additionalInstallConfigFiles);
		if (alternativeInstallConfig.empty())
			return;

		config_.game.installConfigFile = alternativeInstallConfig;
	}

	gameVersion_ = "unknown";
	Utils::getVersionInfo(exePath, gameVersion_);

	modsFolderPath_ = gameFolderPath + '\\' + config_.game.modsFolder;
	Utils::normalizePath(modsFolderPath_);

	if (!wxDirExists(modsFolderPath_))
	{
		if (!wxMkdir(modsFolderPath_))
		{
			std::string extendedErrorMessage =
				makeErrorMessage(Message::ErrorMessageType::ERROR_CREATE_DIRECTORY, modsFolderPath_);

			showMessage(Message::MessageType::ERROR_OK, "Couldn't create mods folder.", extendedErrorMessage);

			return;
		}
	}

	if (Utils::getPathLength(modsFolderPath_) > config_.game.maxFullPathLength)
	{
		std::string extendedErrorMessage = makeErrorMessage(Message::ErrorMessageType::ERROR_ACCESS_DIRECTORY,
			modsFolderPath_);
		showMessage(Message::MessageType::ERROR_OK,
			"Mod folder path must not exceed " + std::to_string(config_.game.maxFullPathLength) +
			" characters.", extendedErrorMessage);

		return;
	}

	archivesFolderPath_ = gameFolderPath + '\\' + config_.game.archivesFolder;
	Utils::normalizePath(archivesFolderPath_);

	if (!wxDirExists(archivesFolderPath_))
	{
		if (!wxMkdir(archivesFolderPath_))
		{
			std::string extendedErrorMessage =
				makeErrorMessage(Message::ErrorMessageType::ERROR_CREATE_DIRECTORY, archivesFolderPath_);

			showMessage(Message::MessageType::ERROR_OK, "Couldn't create mod archives folder.", extendedErrorMessage);

			return;
		}
	}

	config_.game.folderPath = gameFolderPath;
	setIsInitialized(true);

	loadModsFolders();
	loadModsConfig(saveToFile);
	computeMaxActive();

	shouldSortInactives_ = true;
}

void ModManager::initialize(const std::string& gamePath, bool saveToFile, bool forceRefresh)
{
	if (getIsInitialized())
	{
		if (!forceRefresh && Utils::stringIsEqualNoCase(config_.game.folderPath, gamePath))
			return;

		shutDown(saveToFile);
	}

	config_.game.folderPath = gamePath;
	initialize(saveToFile);
}

void ModManager::shutDown(bool saveToFile)
{
	if (!getIsInitialized())
		return;

	refreshModListAndSaveToFile(saveToFile);

	std::vector<Mod>().swap(mods_);

	countStats();

	setIsInitialized(false);
}

void ModManager::refreshModListAndSaveToFile(bool saveToFile)
{
	scheduleSetModTypeAll();
	update();

	// Prepare mod_path.
	constructAndUpdateModPath(saveToFile);

	// Prepare movie_path.
	constructMoviePath(saveToFile);

	// Save mods setup.
	if (saveToFile)
	{
		std::string errorMessage;

		bool isModConfigSaved = modConfig_.saveUpdating();
		if (!isModConfigSaved)
		{
			errorMessage = modConfig_.getErrorMessage();
		}

		bool isInstallConfigSaved = installConfig_.saveUpdating();
		if (!isInstallConfigSaved)
		{
			if (!errorMessage.empty())
				errorMessage += "\n";

			errorMessage += installConfig_.getErrorMessage();
		}

		if (isModConfigSaved && isInstallConfigSaved)
			createLogFile();
		else
			showMessage(Message::MessageType::ERROR_OK, "Couldn't save mods setup.", errorMessage);
	}

	removeAllSelected();
}

std::string ModManager::constructMoviePath(bool updateConfig)
{
	auto [moviePath, baseMoviePath] = constructMoviePath();

	if (updateConfig)
	{
		installConfig_.movie_path = moviePath;
		config_.game.baseMoviePath = baseMoviePath;
	}

	return moviePath;
}

std::string ModManager::constructAndUpdateModPath(bool updateConfig)
{
	auto [modsPathPrefix, modPath, modsPathSuffix] = constructModPath();

	if (updateConfig)
	{
		modConfig_.mod_path = modPath;
		config_.game.modsPathSuffix = modsPathSuffix;
		config_.game.modsPathPrefix = modsPathPrefix;
	}

	return modPath;
}

std::tuple<std::string, std::string> ModManager::constructMoviePath() const
{
	int lastActive = calculateLastActiveIndex();
	std::string moviePath = "";
	std::string individualMoviePath;
	for (int i = 0; i <= lastActive; ++i)
	{
		if (!mods_[i].getIsPaused() && mods_[i].getHasCutscene())
		{
			individualMoviePath = "";
			individualMoviePath += ".\\";
			individualMoviePath += config_.game.modsFolder;
			individualMoviePath += '\\';
			individualMoviePath += mods_[i].getName();
			individualMoviePath += '\\';
			individualMoviePath += mods_[i].getCutsceneFolder();

			if (!config_.game.dataOverrideSubfolder.empty())
			{
				// if the override subfolder is set, duplicate each mod with and without that folder, giving priority to the override.
				individualMoviePath = individualMoviePath + '\\' + config_.game.dataOverrideSubfolder + '+' + individualMoviePath;
			}

			moviePath += '+' + individualMoviePath;
		}
	}

	std::string baseMoviePath = config_.game.baseMoviePath;
	Utils::stringTrim(baseMoviePath);
	Utils::stringTrim(baseMoviePath, '+');
	moviePath += '+' + config_.game.baseMoviePath;
	Utils::stringTrim(moviePath, '+');

	return std::make_tuple(moviePath, baseMoviePath);
}

std::tuple<std::string, std::string, std::string> ModManager::constructModPath() const
{
	int lastActive = calculateLastActiveIndex();

	std::string modsPathPrefix = config_.game.modsPathPrefix;
	Utils::stringTrim(modsPathPrefix);
	Utils::stringTrim(modsPathPrefix, '+');

	std::string modPath = modsPathPrefix;
	std::string individualModPath;
	for (int i = 0; i <= lastActive; ++i)
	{
		individualModPath = "";
		if (mods_[i].getIsPaused())
			individualModPath += config_.game.pauseIndicator;

		individualModPath += ".\\";
		individualModPath += config_.game.modsFolder;
		individualModPath += '\\';
		individualModPath += mods_[i].getName();

		if (!config_.game.dataOverrideSubfolder.empty()) 
		{
			// if the override subfolder is set, duplicate each mod with and without that folder, giving priority to the override.
			individualModPath = individualModPath + '\\' + config_.game.dataOverrideSubfolder + '+' + individualModPath;
		}

		modPath += '+' + individualModPath;
	}

	std::string modsPathSuffix = std::string(config_.game.modsPathSuffix);
	Utils::stringTrim(modsPathSuffix);
	Utils::stringTrim(modsPathSuffix, '+');
	modPath += '+' + modsPathSuffix;
	Utils::stringTrim(modPath, '+');

	return std::make_tuple(modsPathPrefix, modPath, modsPathSuffix);
}

bool ModManager::getHasStateChanged()
{
	if (hasStateChanged_)
	{
		hasStateChanged_ = false;

		return true;
	}

	return false;
}

bool ModManager::getIsInitialized() const
{
	return isInitialized_;
}

void ModManager::scheduleModsFoldersReload()
{
	shouldReloadModsFolders_ = true;
}

void ModManager::update()
{
	if (!getIsInitialized())
		return;

	if (shouldReloadModsFolders_)
		loadModsFolders();

	if (shouldSortInactives_)
		sortInactives();

	selectByIndex();

	for (int i = 0; i < getModCount(); ++i)
	{
		mods_[i].setIndex(i);

		if (mods_[i].getShouldUpdateType())
		{
			setModType(mods_[i]);
			setModReadme(mods_[i]);
		}
	}

	countStats();
}

bool ModManager::addSelected(int selected)
{
	assert(selected >= 0);
	if (selected < 0)
		return false;

	size_t originalSize = allSelected_.size();
	removeSelected(selected);
	allSelected_.push_back(selected);
	return allSelected_.size() > originalSize;
}

const std::vector<int>& ModManager::getAllSelected() const
{
	return allSelected_;
}

const int ModManager::getSingleSelected() const
{
	assert(allSelected_.size() == 1);
	if (allSelected_.size() == 1)
		return allSelected_.back();
	else
		return -1;
}

bool ModManager::removeSelected(int selected)
{
	size_t originalSize = allSelected_.size();
	allSelected_.erase(std::remove(allSelected_.begin(), allSelected_.end(), selected), allSelected_.end());
	return allSelected_.size() < originalSize;
}

void ModManager::removeAllSelected()
{
	allSelected_.clear();
}

const std::string ModManager::getAlternativeFile(const std::string& gameFolderPath, const std::vector<std::string>& files) const
{
	std::string alternativeFile;

	for (auto& file : files)
	{
		alternativeFile = gameFolderPath + '\\' + file;

		if (wxFileExists(alternativeFile))
			return Utils::getFilenameCaseInsensitive(wxFileName(alternativeFile));
	}

	return std::string();
}

const std::string& ModManager::getModsFolderPath() const
{
	return modsFolderPath_;
}

const std::string& ModManager::getArchivesFolderPath() const
{
	return archivesFolderPath_;
}

const bool ModManager::hasModWithName(const std::string& modName) const
{
	return std::find_if(mods_.begin(), mods_.end(), PredicateIsModNameEqualNoCase(modName)) != mods_.end();
}

std::vector<Mod>& ModManager::getMods()
{
	return mods_;
}

bool ModManager::canActivateDeactivate() const
{
	return !allSelected_.empty();
}

bool ModManager::canIncreasePriorities()
{
	// can increase priority unless a) nothing is selected, b) nothing is active, c) only top mods are selected
	if (allSelected_.empty())
		return false;

	if (std::all_of(allSelected_.begin(), allSelected_.end(), [this](int selected) {return !mods_[selected].getIsActive();}))
		return false;

	// top check: if active selection contains just sequential indices (or one index) and starts with 0, then increasing priority does nothing
	std::sort(allSelected_.begin(), allSelected_.end());
	std::vector<int> allActiveSorted;
	std::copy_if(allSelected_.begin(), allSelected_.end(), std::back_inserter(allActiveSorted), [this](int selected) {return mods_[selected].getIsActive(); });
	int numElements = static_cast<int>(allActiveSorted.size());
	if (allActiveSorted[0] == 0 && (allActiveSorted[numElements - 1] - allActiveSorted[0]) == (numElements - 1))
		return false;

	return true;
}

bool ModManager::canDecreasePriorities()
{
	// can increase priority unless a) nothing is selected, b) nothing is active, c) mod is last mod d) mod is last active mod
	if (allSelected_.empty())
		return false;

	if (std::all_of(allSelected_.begin(), allSelected_.end(), [this](int selected) {return !mods_[selected].getIsActive(); }))
		return false;


	// bottom check: if active selection contains just sequential indices (or just one index) and last element is last active element, decreasing priority does nothing
	std::sort(allSelected_.begin(), allSelected_.end());
	std::vector<int> allActiveSorted;
	std::copy_if(allSelected_.begin(), allSelected_.end(), std::back_inserter(allActiveSorted), [this](int selected) {return mods_[selected].getIsActive(); });
	int numElements = static_cast<int>(allActiveSorted.size());
	if ((allActiveSorted[numElements - 1] == calculateLastActiveIndex()) && (allActiveSorted[numElements - 1] - allActiveSorted[0]) == (numElements - 1))
		return false;

	return true;
}

bool ModManager::canPauseResume() const
{
	// can pause/unpause unless a) nothing is selected, b) nothing is active
	if (allSelected_.empty())
		return false;

	if (std::all_of(allSelected_.begin(), allSelected_.end(), [this](int selected) {return !mods_[selected].getIsActive(); }))
		return false;

	return true;
}

bool ModManager::canOpenReadme() const
{
	if (allSelected_.empty())
		return false;

	if (allSelected_.size() > 1)
		return false;

	if (!mods_[getSingleSelected()].getHasReadme())
		return false;

	return true;
}

bool ModManager::needsToApply()
{
	if (!getIsInitialized())
		return false;

	std::string modPath = constructAndUpdateModPath(false);
	std::string moviePath = constructMoviePath(false);
	if (modPath.compare(modConfig_.mod_path) == 0 && moviePath.compare(installConfig_.movie_path) == 0)
		return false;
	return true;
}

void ModManager::activateDeactivate()
{
	if (!canActivateDeactivate())
		return;


	for (int selectedIndex : allSelected_)
	{
		size_t modIndex = std::distance(mods_.begin(), std::find_if(mods_.begin(), mods_.end(), [&](Mod const& mod) {return mod.getIndex() == selectedIndex; }));

		if (modIndex >= mods_.size())
			continue;

		if (mods_[modIndex].getIsPaused())
		{
			mods_[modIndex].setIsPaused(false);
			continue;
		}

		if (mods_[modIndex].getIsActive())
			deactivate(modIndex);
		else
			activate(modIndex);
	}

	shouldSortInactives_ = true;
}

void ModManager::increasePriorities()
{
	if (!canIncreasePriorities())
		return;

	// as we modify the mods and the content of allSelected_, iterate through a sorted copy
	std::vector<int> allSelectedSorted(allSelected_.size());
	std::partial_sort_copy(std::begin(allSelected_), std::end(allSelected_), std::begin(allSelectedSorted), std::end(allSelectedSorted));

	int topIndex = 0;
	for (int indexModToShift : allSelectedSorted)
	{
		if (!mods_[indexModToShift].getIsActive())
			continue;
		if (indexModToShift == topIndex)
		{
			// if the mod is already at the top of the list, shifting up is not possible
			// repeat this behaviour until the first mods has been found which can actually be shifted up
			++topIndex;
			continue;
		}

		std::swap(mods_[indexModToShift], mods_[indexModToShift - 1]);
		mods_[indexModToShift].setIndex(indexModToShift);
		mods_[indexModToShift - 1].setIndex(indexModToShift - 1);

		removeSelected(indexModToShift);
		addSelected(indexModToShift - 1);
	}
}

void ModManager::decreasePriorities()
{
	if (!canDecreasePriorities())
		return;

	// as we modify the mods and the content of allSelected_, iterate through a sorted copy
	std::vector<int> allSelectedSorted(allSelected_.size());
	std::partial_sort_copy(std::begin(allSelected_), std::end(allSelected_), std::begin(allSelectedSorted), std::end(allSelectedSorted));

	int bottomIndex = calculateLastActiveIndex();
	for (int idx = allSelectedSorted.size() - 1; idx >= 0; --idx)
	{
		int indexModToShift = allSelectedSorted[idx];

		if (!mods_[indexModToShift].getIsActive())
			continue;
		if (indexModToShift == bottomIndex)
		{
			// if the mod is already at the bottom of the list (= is the last active one), shifting down is not possible
			// repeat this behaviour until the first mods has been found which can actually be shifted down
			--bottomIndex;
			continue;
		}

		std::swap(mods_[indexModToShift], mods_[indexModToShift + 1]);
		mods_[indexModToShift + 1].setIndex(indexModToShift + 1);
		mods_[indexModToShift].setIndex(indexModToShift);

		removeSelected(indexModToShift);
		addSelected(indexModToShift + 1);
	}
}

int ModManager::movePriority(int sourceIndex, int targetIndex)
{
	if (sourceIndex < 0 || targetIndex < 0 ||
		sourceIndex == targetIndex ||
		sourceIndex >= signed(mods_.size()) ||
		!mods_[sourceIndex].getIsActive()) // only active mods can be moved
		return -1;

	if (targetIndex >= signed(mods_.size()) || !mods_[targetIndex].getIsActive())
	{
		targetIndex = calculateLastActiveIndex();
	}

	if (sourceIndex > targetIndex)
		std::rotate(mods_.rend() - sourceIndex - 1, mods_.rend() - sourceIndex, mods_.rend() - targetIndex);
	else
		std::rotate(mods_.begin() + sourceIndex, mods_.begin() + sourceIndex + 1, mods_.begin() + targetIndex + 1);

	return targetIndex;
}

void ModManager::pauseResume()
{
	if (!canPauseResume())
		return;

	for (int selected : allSelected_)
	{
		if (!mods_[selected].getIsActive())
			continue;

		mods_[selected].setIsPaused(!mods_[selected].getIsPaused());
	}
}

int ModManager::getModCount() const
{
	return static_cast<int>(mods_.size());
}

int ModManager::getWarningCount() const
{
	return warningCount_;
}

int ModManager::getActiveOrPausedCount() const
{
	return activeOrPausedCount_;
}

int ModManager::getActiveGamesysCount() const
{
	return activeGamesysCount_;
}

bool ModManager::directoryAdd(const std::string& name)
{
	if (hasModWithName(name))
		return false;

	Mod mod;
	setModName(mod, name);
	mods_.push_back(mod);

	shouldSortInactives_ = true;

	return true;
}

bool ModManager::directoryRemove(const std::string& name)
{
	auto foundMod = std::find_if(mods_.begin(), mods_.end(), PredicateIsModNameEqualNoCase(name));

	if (foundMod == mods_.end())
		return false;

	mods_.erase(foundMod);

	return true;
}

bool ModManager::directoryRename(const std::string& name, const std::string& newName)
{
	auto foundMod = std::find_if(mods_.begin(), mods_.end(), PredicateIsModNameEqualNoCase(name));

	if (foundMod == mods_.end())
		return false;

	setModName(*foundMod, newName);

	if (!foundMod->getIsActive())
		shouldSortInactives_ = true;

	return true;
}

bool ModManager::directoryUpdate(const std::string& name)
{
	auto foundMod = std::find_if(mods_.begin(), mods_.end(), PredicateIsModNameEqualNoCase(name));

	if (foundMod == mods_.end())
		return false;

	foundMod->setShouldUpdateType(true);

	return true;
}

void ModManager::setIsInitialized(bool isInitialized)
{
	isInitialized_ = isInitialized;
	hasStateChanged_ = true;
}

void ModManager::loadModsFolders()
{
	shouldReloadModsFolders_ = false;

	wxDir dir;
	wxString name;

	if (!dir.Open(modsFolderPath_))
	{
		std::vector<Mod>().swap(mods_);

		std::string extendedErrorMessage =
			makeErrorMessage(Message::ErrorMessageType::ERROR_ACCESS_DIRECTORY, modsFolderPath_);

		showMessage(Message::MessageType::ERROR_OK, "Couldn't load mods folder list.", extendedErrorMessage);

		return;
	}

	bool isFound = dir.GetFirst(&name, wxEmptyString, wxDIR_DIRS | wxDIR_HIDDEN);
	std::vector<std::string> modsFolders;

	while (isFound)
	{
		modsFolders.emplace_back(name);

		isFound = dir.GetNext(&name);
	}

	for (size_t i = 0; i < mods_.size();)
	{
		auto foundMod = std::find(modsFolders.begin(), modsFolders.end(), mods_[i].getName());

		if (foundMod != modsFolders.end())
		{
			mods_[i].setShouldUpdateType(true);
			++i;

			std::swap(*foundMod, modsFolders.back());
			modsFolders.pop_back();
		}
		else
		{
			mods_.erase(mods_.begin() + i);
		}
	}

	if (!modsFolders.empty())
	{
		for (auto& modFolder : modsFolders)
		{
			if (!modFolder.empty())
			{
				Mod mod;
				setModName(mod, modFolder);
				mods_.push_back(mod);
			}
		}

		shouldSortInactives_ = true;
	}
}

bool ModManager::loadModsConfig(bool saveModList, const std::string& modPathOverride)
{
	std::string token, remainder;

	std::vector<std::string> baseModPaths;
	remainder = config_.game.modsPathPrefix + '+' + config_.game.modsPathSuffix;

	while (Utils::tokenize(token, remainder, remainder, '+'))
	{
		Utils::normalizePath(token);

		if (std::find_if(baseModPaths.cbegin(), baseModPaths.cend(), Utils::StringPredicateIsEqualNoCase(token))
			== baseModPaths.cend())
		{
			baseModPaths.push_back(token);
		}
	}

	modConfig_.mod_path = "";

	modConfig_.assignFile(config_.game.folderPath + '\\' + config_.game.modsConfigFile);
	installConfig_.assignFile(config_.game.folderPath + '\\' + config_.game.installConfigFile);

	if (!modConfig_.load())
		showMessage(Message::MessageType::ERROR_OK, "Couldn't load mods setup.", modConfig_.getErrorMessage());

	remainder = modConfig_.mod_path;
	if (!modPathOverride.empty()) {
		remainder = std::string(modPathOverride);
	}

	struct ActiveMod
	{
		std::string name;
		bool isPaused;
	};

	std::vector<ActiveMod> activeMods;
	std::vector<std::string> foreignPaths;

	while (Utils::tokenize(token, remainder, remainder, '+'))
	{
		if (!config_.game.dataOverrideSubfolder.empty() && token.find(config_.game.dataOverrideSubfolder) != std::string::npos)
		{
			// If the mod name contains dataOverrideSubfolder, then we expect the same mod to be represented both by this token and the original token without the folder, so skip it and fetch the original token next.
			// This also prevents the manager from warning the user about removed mods.
			continue;
		}
			
		ActiveMod activeMod;

		if (Utils::stringIsEqualNoCase(token.substr(0, config_.game.pauseIndicator.length()),
			config_.game.pauseIndicator))
		{
			token.erase(0, config_.game.pauseIndicator.length());
			activeMod.isPaused = true;
		}
		else
		{
			activeMod.isPaused = false;
		}

		Utils::normalizePath(token);
		std::string t, r(token);
		Utils::tokenize(t, r, r, '\\');

		if (Utils::stringIsEqualNoCase(t, config_.game.modsFolder))
		{
			activeMod.name = r;
			activeMods.push_back(activeMod);
		}
		else if (std::find_if(baseModPaths.cbegin(), baseModPaths.cend(), Utils::StringPredicateIsEqualNoCase(token))
			== baseModPaths.cend())
		{
			foreignPaths.push_back(token);
		}
	}

	if (foreignPaths.size() > 0)
	{
		std::string msg;

		msg += "Following mod paths will be removed from the search order.";
		msg += " Mods inside these folders will no longer be loaded:\n";

		for (auto& foreignPath : foreignPaths)
			msg += "\n" + foreignPath;

		showMessage(Message::MessageType::INFORMATION_OK, msg);
	}

	size_t lastInactive = 0;

	for (auto it = activeMods.begin(); it < activeMods.end(); ++it)
	{
		auto f = std::find_if(mods_.begin(), mods_.end(), PredicateIsModNameEqualNoCase(it->name));

		if (f != mods_.end() && !f->getIsActive())
		{
			f->setIsActive(true);
			f->setIsPaused(it->isPaused);
			std::iter_swap(f, mods_.begin() + lastInactive);

			++lastInactive;
		}
		else if (!modPathOverride.empty()) {
			Mod overrideMod;
			overrideMod.setIsActive(!it->isPaused);
			overrideMod.setIsPaused(it->isPaused);
			overrideMod.setName(it->name);
			overrideMod.setIndex(mods_.size());
			mods_.push_back(overrideMod);
		}
	}

	refreshModListAndSaveToFile(saveModList);

	return activeMods.size() == lastInactive;
}

void ModManager::loadNewModPath(const std::string& modPath)
{
	modConfig_.mod_path = modPath;
	if (!getIsInitialized() || !modConfig_.saveUpdating())
	{
		showMessage(Message::MessageType::ERROR_OK, "Could not load new mod path.",
			makeErrorMessage(Message::ErrorMessageType::ERROR_WRITE_FILE, config_.game.modsConfigFile));
		return;
	}

	initialize(config_.game.folderPath, false, true);
}

void ModManager::computeMaxActive()
{
	std::string token;
	int nPath = 0;

	std::string uberModPath = modConfig_.uber_mod_path;
	Utils::stringTrim(uberModPath);
	Utils::stringTrim(uberModPath, '+');

	while (Utils::tokenize(token, uberModPath, uberModPath, '+'))
		++nPath;

	std::string additionalModPaths = config_.game.modsPathPrefix + '+' + config_.game.modsPathSuffix;
	Utils::stringTrim(additionalModPaths);
	Utils::stringTrim(additionalModPaths, '+');

	while (Utils::tokenize(token, additionalModPaths, additionalModPaths, '+'))
		++nPath;
	config_.game.maxActive = config_.game.maxPathCount - nPath;
	// in case of a valid override subfolder, each mods path is basically duplicated, halving the amount of mods that can be activated
	if (!config_.game.dataOverrideSubfolder.empty())
		config_.game.maxActive = config_.game.maxActive / 2;
}

void ModManager::activate(int index)
{
	if (mods_[index].getIsActive())
		return;

	Mod mod = mods_[index];
	mod.setIsActive(true);

	if (mod.getHasGamesys())
	{
		// move gamesys mods to last possible position
		int lastActive = calculateLastActiveIndex() + 1;
		std::rotate(mods_.begin() + lastActive, mods_.begin() + index, mods_.begin() + index + 1);
		mods_[lastActive] = mod;
	}
	else
	{
		std::move_backward(mods_.begin(), mods_.begin() + index, mods_.begin() + index + 1);
		mods_[0] = mod;
	}
}

void ModManager::deactivate(int index)
{
	if (!mods_[index].getIsActive())
		return;

	mods_[index].setIsActive(false);
	mods_.push_back(mods_[index]);
	mods_.erase(mods_.begin() + index);
}

void ModManager::scheduleSetModTypeAll()
{
	for (auto& mod : mods_)
		mod.setShouldUpdateType(true);
}

void ModManager::setModName(Mod& mod, const std::string& name) const
{
	mod.setName(name);
	std::string modPath(getModsFolderPath() + "\\" + name);
	if (static_cast<int>(name.length()) > config_.game.maxFolderLength || Utils::getPathLength(modPath) > config_.game.
		maxFullPathLength)
		mod.setIsNameTooLong(true);
	else
		mod.setIsNameTooLong(false);

	// Name must not contain mod path separator '+' or non US-ASCII characters, the latter of which lead to the mod being silently ignored by the game.
	if (name.find('+') != std::string::npos || !std::all_of(name.begin(), name.end(), [](char c) { return static_cast<unsigned char>(c) <= 127; }))
		mod.setIsInvalidCharInName(true);
	else
		mod.setIsInvalidCharInName(false);
}

void ModManager::setModType(Mod& mod)
{
	checkModDirectory(mod);

	std::string type;
	if (mod.getIsUnrecognized())
	{
		type = "Unrecognized";
	}
	else
	{
		if (mod.getHasGamesys())
		{
			type = "Gamesys";
		}
		else if (mod.getHasOther())
		{
			type = "Replacement/Other";
		}
		else if (mod.getHasCutscene())
		{
			type = "Cutscene";
		}
		else if (mod.getHasSubtitle())
		{
			type = "Subtitle";
		}

		if (mod.getHasCFG())
		{
			if (!type.empty())
				type += ", ";

			type += "CFG";
		}

		if (mod.getHasDML())
		{
			if (!type.empty())
				type += ", ";

			type += "DML";
		}

		if (mod.getHasScript())
		{
			if (!type.empty())
				type += ", ";

			type += "Script";
		}

		if (mod.getHasMis())
		{
			if (!type.empty())
				type += ", ";

			type += "Level";
		}
	}

	mod.setType(type);
	mod.setShouldUpdateType(false);
}

void ModManager::setModReadme(Mod& mod) const
{
	wxDir dir;
	if (!dir.Open(wxString(modsFolderPath_ + "\\" + mod.getName())))
		return;

	wxString foundName;
	std::vector<wxString> foundReadmes;

	bool isFileFound = dir.GetFirst(&foundName, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN);
	if (!isFileFound)
		return;

	int idxExt[2] = { -1, -1 };
	while (isFileFound)
	{
		foundName.MakeLower();

		if (foundName.Find("read") != wxNOT_FOUND)
		{
			foundReadmes.push_back(foundName);
			if (foundName.Find("pdf") != wxNOT_FOUND)
			{
				idxExt[0] = foundReadmes.size() - 1;
				idxExt[1] = 3;
			}
			else if (foundName.Find("txt") != wxNOT_FOUND && idxExt[1] < 3)
			{
				idxExt[0] = foundReadmes.size() - 1;
				idxExt[1] = 2;
			}
			else if (idxExt[1] < 2)
			{
				idxExt[0] = foundReadmes.size() - 1;
				idxExt[1] = 1;
			}
		}
		isFileFound = dir.GetNext(&foundName);
	}

	if (foundReadmes.empty())
		return;

	mod.setReadme(foundReadmes[idxExt[0]].ToStdString());
	mod.setHasReadme(true);
}

void ModManager::checkModDirectory(Mod& mod)
{
	wxDir dir;
	if (!dir.Open(wxString(modsFolderPath_ + "\\" + mod.getName())))
		return;

	mod.setHasCFG(dir.HasFiles("*.cfg"));
	for (auto& cutsceneFolder : config_.game.cutsceneFolders)
	{
		if (dir.HasSubDirs(cutsceneFolder))
			mod.setCutsceneFolder(cutsceneFolder);
	}
	mod.setHasSubtitle(dir.HasSubDirs(config_.game.subtitleFolder));
	mod.setHasDML(dir.HasFiles("*.dml") || dir.HasSubDirs("dbmods"));
	mod.setHasScript(dir.HasSubDirs("scriptdata"));
	mod.setHasMis(dir.HasFiles("*.mis"));
	mod.setHasGamesys(dir.HasFiles("*.gam"));

	std::list<std::string> allSubDirs;
	wxString nextDirName;
	wxString currentDirName;
	bool isDirFound = dir.GetFirst(&nextDirName, wxEmptyString, wxDIR_DIRS | wxDIR_HIDDEN);
	while (isDirFound)
	{
		currentDirName = wxString(nextDirName).MakeLower();
		// go through all folders and collect for later case-sensitive check
		allSubDirs.push_back(currentDirName.ToStdString());

		// check existence of any data directory
		for (auto& dirName : dataDirectories_)
		{
			if (dirName.compare(currentDirName) == 0)
				mod.setHasOther(true);
		}

		isDirFound = dir.GetNext(&nextDirName);
	}

	// Wine-only check
	if (!allSubDirs.empty())
	{
		unsigned int dirNumberIncludingNonUnique = allSubDirs.size();
		allSubDirs.sort();
		allSubDirs.unique();
		if (dirNumberIncludingNonUnique != allSubDirs.size())
			mod.setHasDuplicateFolders(true);
	}

	if (mod.getHasOther())
		return;

	bool isFileFound = dir.GetFirst(&nextDirName, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN);
	while (isFileFound)
	{
		nextDirName.MakeLower();

		for (auto& fileName : dataFiles_)
		{
			if (wxMatchWild(fileName, nextDirName))
			{
				mod.setHasOther(true);

				return;
			}
		}

		isFileFound = dir.GetNext(&nextDirName);
	}

	mod.setHasOther(false);
}

void ModManager::sortInactives()
{
	shouldSortInactives_ = false;

	auto it = mods_.begin();

	while (it != mods_.end() && it->getIsActive())
		++it;

	if (it == mods_.end())
		return;

	std::sort(it, mods_.end());
}

void ModManager::selectByIndex()
{
	if (allSelected_.empty())
		return;

	std::vector<int> allSelectedTemp(allSelected_);
	removeAllSelected();

	// after order of mods_ has been changed and each mod index updated, reset allSelected_ to only contain the updated indices, minus any impossible indices (e.g. index >= modCount)
	for (int modIndex = 0; modIndex < getModCount(); ++modIndex)
	{
		int currentIndex = mods_[modIndex].getIndex();
		if (std::find(allSelectedTemp.begin(), allSelectedTemp.end(), currentIndex) != allSelectedTemp.end())
			addSelected(modIndex);
	}

}

void ModManager::countStats()
{
	warningCount_ = 0;
	activeOrPausedCount_ = 0;
	activeGamesysCount_ = 0;
	std::unordered_map<std::string, std::vector<Mod*>> misFileToMods;

	for (auto& mod : mods_)
	{
		mod.resetWarning();

		if (mod.getIsActive())
		{
			++activeOrPausedCount_;

			if (!mod.getIsPaused() && mod.getHasGamesys())
				++activeGamesysCount_;

			if (!mod.getIsPaused() && mod.getHasMis())
			{
				collectMisFileToModMapping(mod, misFileToMods);
			}
		}
	}

	for (auto& [misFile, mods] : misFileToMods)
	{
		if (mods.size() > 1) {
			for (auto mod_ptr : mods) {
				mod_ptr->setIsMultipleMis(true);
			}
		}
	}

	for (auto& mod : mods_)
	{
		// multiple gamesys
		if ((activeGamesysCount_ > 1) && mod.getHasGamesys() && mod.getIsActive())
			mod.setIsMultipleGamesys(true);

		if (mod.getHasWarning())
			++warningCount_;
	}
}

void ModManager::collectMisFileToModMapping(Mod& mod, std::unordered_map<std::string, std::vector<Mod*>>& misFileToMods)
{
	// get all *.mis files
	wxString modPath = wxString(modsFolderPath_ + "\\" + mod.getName());
	wxDir dir;
	if (!dir.Open(modPath))
	{
		return;
	}
	wxArrayString modMisFiles;
	dir.GetAllFiles(modPath, &modMisFiles, wxString("*.mis"), wxDIR_FILES);

	for (auto& misFile : modMisFiles)
	{
		// get filename
		std::string filename = misFile.ToStdString();
		filename.erase(0, filename.find_last_of('\\') + 1);

		misFileToMods[filename].push_back(&mod);
	}
}

void ModManager::createLogFile()
{
	if (!config_.application.createLogFile)
		return;

	std::ofstream log(logFilePath_);

	if (log.fail())
	{
		showMessage(Message::MessageType::ERROR_OK, "Couldn't create log file.",
			makeErrorMessage(Message::ErrorMessageType::ERROR_WRITE_FILE, logFilePath_));

		return;
	}

	log << "Mods setup log - " << ApplicationInfo::full << std::endl;

	std::string dateTime;
	Utils::getCurrentDateTime(dateTime);

	log << "Created: " << dateTime << std::endl;
	log << std::endl;

	log << config_.game.executableFile << " version: " << gameVersion_ << std::endl;
	log << "Mods folder path: " << modsFolderPath_ << '\\' << std::endl;
	log << std::endl;

	std::string modsTable;
	createModsTable(modsTable);

	log << modsTable;
	log << std::endl;

	log << "mod_path";

	if (!modConfig_.mod_path.empty())
		log << " " << modConfig_.mod_path;
	log << std::endl << std::endl;

	log << "movie_path";

	if (!installConfig_.movie_path.empty())
		log << " " << installConfig_.movie_path;
	log << std::endl << std::endl;

	log << "Listing of mods folder content:" << std::endl;

	std::string modsDirectoryContent;
	Utils::listDirectoryContent(modsDirectoryContent, modsFolderPath_, 2);

	if (!modsDirectoryContent.empty())
	{
		log << std::endl;
		log << modsDirectoryContent;
	}

	if (log.fail())
	{
		showMessage(Message::MessageType::ERROR_OK, "Couldn't create log file.",
			makeErrorMessage(Message::ErrorMessageType::ERROR_WRITE_FILE, logFilePath_));
	}
}

void ModManager::createModsTable(std::string& modsTable) const
{
	struct ModEntry
	{
		std::string index;
		std::string name;
		std::string status;
		std::string type;
		std::string warning;
	};

	ModEntry header;
	header.index = "#";
	header.name = "Name";
	header.status = "Status";
	header.type = "Type";
	header.warning = "Warning";

	std::vector<ModEntry> modEntries;

	modEntries.push_back(header);
	modEntries.push_back(ModEntry());

	for (auto& mod : mods_)
	{
		ModEntry me;

		me.index = Utils::toString(mod.getIndex() + 1);
		me.name = mod.getName();

		if (mod.getIsPaused())
		{
			me.status = "Paused";
		}
		else if (mod.getIsActive())
		{
			me.status = "Active";
		}
		else
		{
			me.index = "";
			me.status = "Inactive";
		}

		me.type = mod.getType();

		me.warning = "";
		if (mod.getIsUnrecognized())
		{
			me.warning += " Unrecognized mod type.";
		}

		if (mod.getIsNameTooLong())
		{
			me.warning += " Folder name is too long.";
		}

		if (mod.getIsInvalidCharInName())
		{
			me.warning += " Name has plus or non-US-ASCII characters.";
		}

		if (mod.getIsMultipleGamesys())
		{
			me.warning += " Multiple gamesys mods are active.";
		}

		if (mod.getIsMultipleMis())
		{
			me.warning += " Multiple mods changing same *.mis file active.";
		}

		Utils::stringTrim(me.warning);

		modEntries.push_back(me);
	}

	size_t indexSize = 0;
	size_t nameSize = 0;
	size_t statusSize = 0;
	size_t typeSize = 0;
	size_t warningSize = 0;

	for (auto& me : modEntries)
	{
		indexSize = std::max(indexSize, me.index.length());
		nameSize = std::max(nameSize, me.name.length());
		statusSize = std::max(statusSize, me.status.length());
		typeSize = std::max(typeSize, me.type.length());
		warningSize = std::max(warningSize, me.warning.length());
	}

	char headerSeparator = '-';

	modEntries[1].index = std::string(indexSize, headerSeparator);
	modEntries[1].name = std::string(nameSize, headerSeparator);
	modEntries[1].status = std::string(statusSize, headerSeparator);
	modEntries[1].type = std::string(typeSize, headerSeparator);
	modEntries[1].warning = std::string(warningSize, headerSeparator);

	std::stringstream ss;

	if (activeOrPausedCount_ > config_.game.maxActive)
	{
		ss << "Warning: Too many mods are active or paused." << std::endl;
		ss << std::endl;
	}

	std::string columnSeparator = "  ";

	for (auto& entry : modEntries)
	{
		ss << std::right;
		ss << std::setw(indexSize) << entry.index << columnSeparator;
		ss << std::left;
		ss << std::setw(nameSize) << entry.name << columnSeparator;
		ss << std::setw(statusSize) << entry.status << columnSeparator;
		ss << std::setw(typeSize) << entry.type << columnSeparator;

		if (warningCount_ > 0)
			ss << std::setw(warningSize) << entry.warning;

		ss << std::endl;
	}

	modsTable.clear();

	std::string line;
	while (std::getline(ss, line))
	{
		Utils::stringTrimTrailing(line);
		modsTable += line + '\n';
	}
}

int ModManager::calculateLastActiveIndex() const
{
	int lastActiveIndex = -1;
	for (int i = 0; i < getModCount() && mods_[i].getIsActive(); ++i)
		lastActiveIndex = i;

	return lastActiveIndex;
}

void ModManager::resetStates()
{
	for (auto& mod : mods_)
	{
		mod.setIsActive(false);
	}
}

const std::array<wxString, 18> ModManager::dataDirectories_ =
{
	{
		"bitmap",
		"book",
		"editor",
		"fam",
		"fonts",
		"iface",
		"intrface",
		"mesh",
		"motions",
		"obj",
		"objicon",
		"pal",
		"shaders",
		"snd",
		"snd2",
		"song",
		"sq_scripts",
		"strings"
	}
};

const std::array<wxString, 8> ModManager::dataFiles_ =
{
	{
		"motiondb.bin",
		"editor.res",
		"metaui_r.res",
		"shkres.res",
		"skeldata.res",
		"texture.res",
		"*.nut"
		"*.osm"
	}
};