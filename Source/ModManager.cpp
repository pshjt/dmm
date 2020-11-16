// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "Application.hpp"
#include "Message.hpp"
#include "Utils.hpp"

#include "ModManager.hpp"

ModManager::ModManager(ApplicationConfig& config)
	: config_(config)
	  , selected_(-1)
	  , shouldReloadModsFolders_(false)
	  , shouldSortInactives_(false)
	  , warningCount_(0)
	  , activeOrPausedCount_(0)
	  , activeGamesysCount_(0)
	  , logFilePath_(config_.application.executableFolderPath + '\\' + config_.application.logFile)
{
	setIsInitialized(false);
}

void ModManager::initialize()
{
	if (getIsInitialized())
		return;

	if (wxFileExists(logFilePath_))
		wxRemoveFile(logFilePath_);

	std::vector<Mod>().swap(mods_);

	std::string gameFolderPath = config_.game.folderPath;
	Utils::normalizePath(gameFolderPath);

	config_.game.folderPath = "";

	if (!wxDirExists(gameFolderPath))
	{
		gameFolderPath = config_.application.executableFolderPath;
		Utils::normalizePath(gameFolderPath);
	}

	std::string exePath = gameFolderPath + '\\' + config_.game.executableFile;
	config_.game.currentExecutableFile = config_.game.executableFile;

	if (!wxFileExists(exePath))
	{
		std::string alternativeExe = getAlternativeExecutable(gameFolderPath);
		if (alternativeExe.empty())
			return;

		exePath = gameFolderPath + '\\' + alternativeExe;
		config_.game.currentExecutableFile = alternativeExe;
	}

	gameVersion_ = "unknown";

	if (config_.application.checkGameVersion)
	{
		std::string name;

		if (!Utils::getVersionInfo(exePath, name, gameVersion_) ||
			!Utils::stringIsEqualNoCase(config_.game.productName, name))
		{
			return;
		}

		bool isVersionOK = Utils::checkVersion(config_.game.requiredVersion, gameVersion_);

		if (!isVersionOK)
			return;
	}

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
	loadModsConfig();
	computeMaxActive();

	selected_ = -1;
	shouldSortInactives_ = true;
}

void ModManager::initialize(const std::string& gamePath)
{
	if (getIsInitialized())
	{
		if (Utils::stringIsEqualNoCase(config_.game.folderPath, gamePath))
			return;

		shutDown();
	}

	config_.game.folderPath = gamePath;
	initialize();
}

void ModManager::shutDown()
{
	if (!getIsInitialized())
		return;

	refreshModListAndSaveToFile();

	std::vector<Mod>().swap(mods_);

	countStats();

	setIsInitialized(false);
}

void ModManager::refreshModListAndSaveToFile()
{
	scheduleSetModTypeAll();
	update();

	int lastActive = calculateLastActiveIndex();

	// Prepare mod_path.
	constructModPath(lastActive, true);

	// Prepare movie_path.
	constructMoviePath(lastActive, true);

	// Save mods setup.
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

	selected_ = -1;
}

std::string ModManager::constructMoviePath(int lastActive, bool updateConfig)
{
	std::string moviePath = "";

	for (int i = 0; i <= lastActive; ++i)
	{
		if (!mods_[i].getIsPaused() && mods_[i].getHasCutscene())
		{
			moviePath += '+';
			moviePath += ".\\";
			moviePath += config_.game.modsFolder;
			moviePath += '\\';
			moviePath += mods_[i].getName();
			moviePath += '\\';
			moviePath += config_.game.cutsceneFolder;
		}
	}

	std::string baseMoviePath = config_.game.baseMoviePath;
	Utils::stringTrim(baseMoviePath);
	Utils::stringTrim(baseMoviePath, '+');
	moviePath += '+' + config_.game.baseMoviePath;
	Utils::stringTrim(moviePath, '+');

	if (updateConfig)
	{
		installConfig_.movie_path = moviePath;
		config_.game.baseMoviePath = baseMoviePath;
	}

	return moviePath;
}

std::string ModManager::constructModPath(int lastActive, bool updateConfig)
{
	std::string modPath = "";

	for (int i = 0; i <= lastActive; ++i)
	{
		modPath += '+';

		if (mods_[i].getIsPaused())
			modPath += config_.game.pauseIndicator;

		modPath += ".\\";
		modPath += config_.game.modsFolder;
		modPath += '\\';
		modPath += mods_[i].getName();
	}
	std::string baseModPath = std::string(config_.game.baseModPath);
	Utils::stringTrim(baseModPath);
	Utils::stringTrim(baseModPath, '+');
	modPath += '+' + baseModPath;
	Utils::stringTrim(modPath, '+');

	if (updateConfig)
	{
		modConfig_.mod_path = modPath;
		config_.game.baseModPath = baseModPath;
	}

	return modPath;
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

void ModManager::setSelected(int selected)
{
	selected_ = selected;
}

int ModManager::getSelected() const
{
	return selected_;
}

const std::string ModManager::getAlternativeExecutable(std::string gameFolderPath) const
{
	std::string alternativeExePath;

	for (auto& alternativeExe : config_.game.additionalExecutableFiles)
	{
		alternativeExePath = gameFolderPath + '\\' + alternativeExe;

		if (wxFileExists(alternativeExePath))
			return alternativeExe;
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

std::vector<Mod>& ModManager::getMods()
{
	return mods_;
}

bool ModManager::canActivateDeactivate() const
{
	if (selected_ < 0)
		return false;

	return true;
}

bool ModManager::canIncreasePriority() const
{
	if (selected_ < 0)
		return false;

	if (!mods_[selected_].getIsActive())
		return false;

	if (selected_ < 1)
		return false;

	return true;
}

bool ModManager::canDecreasePriority() const
{
	if (selected_ < 0)
		return false;

	if (!mods_[selected_].getIsActive())
		return false;

	if (selected_ > static_cast<int>(mods_.size() - 2))
		return false;

	if (!mods_[selected_ + 1].getIsActive())
		return false;

	return true;
}

bool ModManager::canPauseResume() const
{
	if (selected_ < 0)
		return false;

	if (!mods_[selected_].getIsActive())
		return false;

	return true;
}

bool ModManager::canOpenReadme() const
{
	if (selected_ < 0)
		return false;

	if (!mods_[selected_].getHasReadme())
		return false;

	return true;
}

bool ModManager::needsToApply()
{
	if (!getIsInitialized())
		return false;

	int lastActiveIndex = calculateLastActiveIndex();

	std::string modPath = constructModPath(lastActiveIndex, false);
	std::string moviePath = constructMoviePath(lastActiveIndex, false);
	if (modPath.compare(modConfig_.mod_path) == 0 && moviePath.compare(installConfig_.movie_path) == 0)
		return false;
	return true;
}

void ModManager::activateDeactivate()
{
	if (!canActivateDeactivate())
		return;

	if (mods_[selected_].getIsPaused())
	{
		mods_[selected_].setIsPaused(false);

		return;
	}

	if (mods_[selected_].getIsActive())
		deactivate(selected_);
	else
		activate(selected_);

	shouldSortInactives_ = true;
}

void ModManager::increasePriority()
{
	if (!canIncreasePriority())
		return;

	std::swap(mods_[selected_], mods_[selected_ - 1]);

	mods_[selected_].setIndex(selected_);
	mods_[selected_ - 1].setIndex(selected_ - 1);

	--selected_;
}

void ModManager::decreasePriority()
{
	if (!canDecreasePriority())
		return;

	std::swap(mods_[selected_], mods_[selected_ + 1]);

	mods_[selected_ + 1].setIndex(selected_ + 1);
	mods_[selected_].setIndex(selected_);

	++selected_;
}

void ModManager::movePriority(int sourceIndex, int targetIndex)
{
	if (sourceIndex < 0 || targetIndex < 0 ||
		sourceIndex == targetIndex ||
		sourceIndex >= mods_.size() ||
		!mods_[sourceIndex].getIsActive() ) // only active mods can be moved
		return;

	if (targetIndex >= mods_.size() || !mods_[targetIndex].getIsActive())
	{
		targetIndex = calculateLastActiveIndex();
	}

	if (sourceIndex > targetIndex)
		std::rotate(mods_.rend() - sourceIndex - 1, mods_.rend() - sourceIndex, mods_.rend() - targetIndex);
	else
		std::rotate(mods_.begin() + sourceIndex, mods_.begin() + sourceIndex + 1, mods_.begin() + targetIndex + 1);	
}

void ModManager::swapPriorities(int idx1, int idx2)
{
	if (idx1 < 0 || idx2 < 0 ||
		idx1 >= mods_.size() || idx2 >= mods_.size() ||
		(!mods_[idx1].getIsActive() && !mods_[idx2].getIsActive()) ) // return if both mods are inactive (one must be active to swap)
		return;

	const auto minmax = std::minmax(idx1, idx2);
	const int min = minmax.first;
	int max = minmax.second;

	// If max is inactive, replace it with the last index
	// Note: the minimum index mod must be active as otherwise both have to be inactive (which we already tested in the beginning)
	if (!mods_[max].getIsActive())
	{
		max = calculateLastActiveIndex();
	}
	
	std::swap(mods_[min], mods_[max]);
}

void ModManager::pauseResume()
{
	if (!canPauseResume())
		return;

	int index = selected_;

	if (!mods_[index].getIsActive())
		return;

	if (mods_[index].getIsPaused())
		mods_[index].setIsPaused(false);
	else
		mods_[index].setIsPaused(true);
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
	auto foundMod = std::find_if(mods_.begin(), mods_.end(), PredicateIsModNameEqualNoCase(name));

	if (foundMod != mods_.end())
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
			Mod mod;
			setModName(mod, modFolder);
			mods_.push_back(mod);
		}

		shouldSortInactives_ = true;
	}
}

bool ModManager::loadModsConfig()
{
	std::string token, remainder;

	std::vector<std::string> baseModPaths;
	remainder = config_.game.baseModPath;

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

	struct ActiveMod
	{
		std::string name;
		bool isPaused;
	};

	std::vector<ActiveMod> activeMods;
	std::vector<std::string> foreignPaths;

	while (Utils::tokenize(token, remainder, remainder, '+'))
	{
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

		if (foreignPaths.size() > 1)
		{
			msg += "Following mod paths will be removed from the search order.";
			msg += " Mods inside these folders will no longer be loaded:\n";
		}
		else
		{
			msg += "Following mod path will be removed from the search order.";
			msg += " Mods inside this folder will no longer be loaded:\n";
		}

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
	}

	refreshModListAndSaveToFile();

	return true;
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

	std::string additionalModPath = config_.game.baseModPath;
	Utils::stringTrim(additionalModPath);
	Utils::stringTrim(additionalModPath, '+');

	while (Utils::tokenize(token, additionalModPath, additionalModPath, '+'))
		++nPath;

	config_.game.maxActive = config_.game.maxPathCount - nPath;
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

	if (name.find('+') != std::string::npos)
		mod.setIsPlusSignInName(true);
	else
		mod.setIsPlusSignInName(false);
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
			type = "Replacement";
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

	int idxExt[2] = {-1, -1};
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

	mod.setHasCFG(dir.HasFiles("user.cfg"));
	mod.setHasCutscene(dir.HasSubDirs(config_.game.cutsceneFolder));
	mod.setHasSubtitle(dir.HasSubDirs(config_.game.subtitleFolder));
	mod.setHasDML(dir.HasFiles("*.dml"));
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
	if (selected_ < 0)
	{
		return;
	}
	if (selected_ >= getModCount())
	{
		selected_ = -1;

		return;
	}
	if (selected_ == mods_[selected_].getIndex())
	{
		return;
	}

	for (int i = 0; i < getModCount(); ++i)
	{
		if (mods_[i].getIndex() == selected_)
		{
			selected_ = i;

			return;
		}
	}

	selected_ = -1;
}

void ModManager::countStats()
{
	warningCount_ = 0;
	activeOrPausedCount_ = 0;
	activeGamesysCount_ = 0;

	for (auto& mod : mods_)
	{
		if (mod.getIsActive())
		{
			++activeOrPausedCount_;

			if (!mod.getIsPaused() && mod.getHasGamesys())
				++activeGamesysCount_;
		}
	}

	for (auto& mod : mods_)
	{
		if ((activeGamesysCount_ > 1) && mod.getHasGamesys() && mod.getIsActive() && !mod.getIsPaused())
			mod.setIsMultipleGamesys(true);
		else
			mod.setIsMultipleGamesys(false);

		if (mod.getHasWarning())
			++warningCount_;
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

	log << "Mods Setup Log - " << ApplicationInfo::full << std::endl;

	std::string dateTime;
	Utils::getCurrentDateTime(dateTime);

	log << "Created: " << dateTime << std::endl;
	log << std::endl;

	log << config_.game.currentExecutableFile << " version: " << gameVersion_ << std::endl;
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

		if (mod.getIsPlusSignInName())
		{
			me.warning += " Plus sign in folder name.";
		}

		if (mod.getIsMultipleGamesys())
		{
			me.warning += " Multiple gamesys mods are active.";
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

int ModManager::calculateLastActiveIndex()
{
	int lastActiveIndex = -1;
	for (size_t i = 0; i < mods_.size() && mods_[i].getIsActive(); ++i)
		lastActiveIndex = i;

	return lastActiveIndex;
}

const std::array<wxString, 16> ModManager::dataDirectories_ =
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
		"snd",
		"snd2",
		"song",
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
		"*.mis",
		"*.osm"
	}
};
