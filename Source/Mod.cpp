// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "Utils.hpp"

#include "Mod.hpp"

Mod::Mod()
	: index_(-1)
	//, name_("")
	, isActive_(false)
	, isPaused_(false)
	//, type_("")
	//, readme("")
	, hasCFG_(false)
	, hasCutscene_(false)
	, hasDML_(false)
	, hasGamesys_(false)
	, hasSubtitle_(false)
	, hasOther_(false)
	, hasReadme_(false)
	, isNameTooLong_(false)
	, isPlusSignInName_(false)
	, isMultipleGamesys_(false)
	, shouldUpdateType_(true)
	, isChanged_(true)
{
}

bool Mod::operator < (const Mod& mod) const
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
	return hasCutscene_;
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

bool Mod::getHasOther() const
{
	return hasOther_;
}

bool Mod::getHasReadme() const
{
	return hasReadme_;
}

bool Mod::getIsNameTooLong() const
{
	return isNameTooLong_;
}

bool Mod::getIsPlusSignInName() const
{
	return isPlusSignInName_;
}

bool Mod::getIsMultipleGamesys() const
{
	return isMultipleGamesys_;
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

void Mod::setHasCutscene(bool hasCutscene)
{
	set(hasCutscene_, hasCutscene);
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

void Mod::setHasOther(bool hasOther)
{
	set(hasOther_, hasOther);
}

void Mod::setHasReadme(bool hasReadme)
{
	set(hasReadme_, hasReadme);
}

void Mod::setIsNameTooLong(bool isNameTooLong)
{
	set(isNameTooLong_, isNameTooLong);
}

void Mod::setIsPlusSignInName(bool isPlusSignInName)
{
	set(isPlusSignInName_, isPlusSignInName);
}

void Mod::setIsMultipleGamesys(bool isMultipleGamesys)
{
	set(isMultipleGamesys_, isMultipleGamesys);
}

void Mod::setShouldUpdateType(bool shouldUpdateType)
{
	shouldUpdateType_ = shouldUpdateType;
}

bool Mod::getIsUnrecognized() const
{
	if (hasCFG_ || hasCutscene_ || hasDML_ || hasGamesys_ || hasSubtitle_ || hasOther_)
		return false;

	return true;
}

bool Mod::getHasWarning() const
{
	if (getIsUnrecognized() || isNameTooLong_ || isPlusSignInName_ || isMultipleGamesys_)
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