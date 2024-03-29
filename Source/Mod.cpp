#include <Precompiled.hpp>

#include "Utils.hpp"

#include "Mod.hpp"

Mod::Mod()
	: index_(-1)
	, isActive_(false)
	, isPaused_(false)
	, hasCFG_(false)
	, hasDML_(false)
	, hasGamesys_(false)
	, hasMis_(false)
	, hasSubtitle_(false)
	, hasOther_(false)
	, hasReadme_(false)
	, hasScript_(false)
	, isNameTooLong_(false)
	, isInvalidCharInName_(false)
	, isMultipleGamesys_(false)
	, isMultipleMis_(false)
	, hasDuplicateFolders_(false)
	, shouldUpdateType_(true)
	, isChanged_(true)
{
}

bool Mod::operator <(const Mod& mod) const
{
	return Utils::stringIsLessNoCase(name_, mod.name_);
}

int Mod::getIndex() const
{
	return index_;
}

const std::string& Mod::getName() const
{
	return name_;
}

const std::string& Mod::getReadme() const
{
	return readme_;
}

const std::string& Mod::getCutsceneFolder() const
{
	return cutsceneFolder_;
}

bool Mod::getIsActive() const
{
	return isActive_;
}

bool Mod::getIsPaused() const
{
	return isPaused_;
}

const std::string& Mod::getType() const
{
	return type_;
}

bool Mod::getHasCFG() const
{
	return hasCFG_;
}

bool Mod::getHasCutscene() const
{
	return !cutsceneFolder_.empty();
}

bool Mod::getHasDML() const
{
	return hasDML_;
}

bool Mod::getHasGamesys() const
{
	return hasGamesys_;
}

bool Mod::getHasSubtitle() const
{
	return hasSubtitle_;
}

bool Mod::getHasMis() const
{
	return hasMis_;
}

bool Mod::getHasOther() const
{
	return hasOther_;
}

bool Mod::getHasReadme() const
{
	return hasReadme_;
}

bool Mod::getHasScript() const
{
	return hasScript_;
}

bool Mod::getIsNameTooLong() const
{
	return isNameTooLong_;
}

bool Mod::getIsInvalidCharInName() const
{
	return isInvalidCharInName_;
}

bool Mod::getIsMultipleGamesys() const
{
	return isMultipleGamesys_;
}

bool Mod::getIsMultipleMis() const
{
	return isMultipleMis_;
}

bool Mod::getHasDuplicateFolders() const
{
	return hasDuplicateFolders_;
}

bool Mod::getShouldUpdateType() const
{
	return shouldUpdateType_;
}

void Mod::setIndex(int index)
{
	set(index_, index);
}

void Mod::setName(const std::string& name)
{
	set(name_, name);
}

void Mod::setReadme(const std::string& readme)
{
	set(readme_, readme);
}

void Mod::setCutsceneFolder(const std::string& cutsceneFolder)
{
	set(cutsceneFolder_, cutsceneFolder);
}

void Mod::setIsActive(bool isActive)
{
	setIsPaused(false);
	set(isActive_, isActive);
}

void Mod::setIsPaused(bool isPaused)
{
	set(isPaused_, isPaused);
}

void Mod::setType(const std::string& type)
{
	set(type_, type);
}

void Mod::setHasCFG(bool hasCFG)
{
	set(hasCFG_, hasCFG);
}

void Mod::setHasDML(bool hasDML)
{
	set(hasDML_, hasDML);
}

void Mod::setHasGamesys(bool hasGamesys)
{
	set(hasGamesys_, hasGamesys);
}

void Mod::setHasSubtitle(bool hasSubtitle)
{
	set(hasSubtitle_, hasSubtitle);
}

void Mod::setHasMis(bool hasMis)
{
	set(hasMis_, hasMis);
}

void Mod::setHasOther(bool hasOther)
{
	set(hasOther_, hasOther);
}

void Mod::setHasReadme(bool hasReadme)
{
	set(hasReadme_, hasReadme);
}

void Mod::setHasScript(bool hasScript)
{
	set(hasScript_, hasScript);
}

void Mod::setIsNameTooLong(bool isNameTooLong)
{
	set(isNameTooLong_, isNameTooLong);
}

void Mod::setIsInvalidCharInName(bool isInvalidCharInName)
{
	set(isInvalidCharInName_, isInvalidCharInName);
}

void Mod::setIsMultipleGamesys(bool isMultipleGamesys)
{
	set(isMultipleGamesys_, isMultipleGamesys);
}

void Mod::setIsMultipleMis(bool isMultipleMis)
{
	set(isMultipleMis_, isMultipleMis);
}

void Mod::setHasDuplicateFolders(bool hasDuplicateFolders)
{
	set(hasDuplicateFolders_, hasDuplicateFolders);
}

void Mod::setShouldUpdateType(bool shouldUpdateType)
{
	shouldUpdateType_ = shouldUpdateType;
}

void Mod::resetWarning()
{
	setIsMultipleGamesys(false);
	setIsMultipleMis(false);
}

bool Mod::getIsUnrecognized() const
{
	if (hasCFG_ || getHasCutscene() || hasDML_ || hasGamesys_ || hasMis_ || hasSubtitle_ || hasOther_ || hasScript_)
		return false;

	return true;
}

bool Mod::getHasWarning() const
{
	if (getIsUnrecognized() || isNameTooLong_ || isInvalidCharInName_ || hasDuplicateFolders_ || isMultipleGamesys_ || isMultipleMis_)
		return true;

	return false;
}

bool Mod::getIsChanged()
{
	if (isChanged_)
	{
		isChanged_ = false;

		return true;
	}

	return false;
}