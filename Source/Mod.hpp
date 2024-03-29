#pragma once

class Mod
{
public:
	Mod();
	// Mod(const Mod&); // Default.
	// Mod& operator = (const Mod&); // Default.
	// ~Mod() {}

	bool operator <(const Mod& mod) const;

	int getIndex() const;
	const std::string& getName() const;
	const std::string& getReadme() const;
	const std::string& getCutsceneFolder() const;
	bool getIsActive() const;
	bool getIsPaused() const;
	const std::string& getType() const;

	bool getHasCFG() const;
	bool getHasCutscene() const;
	bool getHasDML() const;
	bool getHasGamesys() const;
	bool getHasSubtitle() const;
	bool getHasMis() const;
	bool getHasOther() const;
	bool getHasReadme() const;
	bool getHasScript() const;

	bool getIsNameTooLong() const;
	bool getIsInvalidCharInName() const;
	bool getIsMultipleGamesys() const;
	bool getIsMultipleMis() const;
	bool getHasDuplicateFolders() const;

	bool getShouldUpdateType() const;

	void setIndex(int index);
	void setName(const std::string& name);
	void setReadme(const std::string& readme);
	void setCutsceneFolder(const std::string& readme);
	void setIsActive(bool isActive);
	void setIsPaused(bool isPaused);
	void setType(const std::string& type);

	void setHasCFG(bool hasCFG);
	void setHasDML(bool hasDML);
	void setHasGamesys(bool hasGamesys);
	void setHasSubtitle(bool hasSubtitle);
	void setHasMis(bool hasMis);
	void setHasOther(bool hasOther);
	void setHasReadme(bool hasReadme);
	void setHasScript(bool hasScript);

	void setIsNameTooLong(bool isNameTooLong);
	void setIsInvalidCharInName(bool isInvalidCharInName);
	void setIsMultipleGamesys(bool isMultipleGamesys);
	void setIsMultipleMis(bool isMultipleMis);
	void setHasDuplicateFolders(bool hasDuplicateFolders);

	void setShouldUpdateType(bool shouldUpdateType);

	void resetWarning();

	bool getIsUnrecognized() const;
	bool getHasWarning() const;
	bool getIsChanged();

private:
	int index_;
	std::string name_;
	std::string readme_;
	std::string cutsceneFolder_;
	bool isActive_;
	bool isPaused_;
	std::string type_;

	bool hasCFG_;
	bool hasDML_;
	bool hasGamesys_;
	bool hasSubtitle_;
	bool hasMis_;
	bool hasOther_;
	bool hasReadme_;
	bool hasScript_;

	bool isNameTooLong_;
	bool isInvalidCharInName_;
	bool isMultipleGamesys_;
	bool isMultipleMis_;
	bool hasDuplicateFolders_;

	bool shouldUpdateType_;
	bool isChanged_;

	template <typename T>
	void set(T& var, const T& newValue);
};

template <typename T>
void Mod::set(T& var, const T& newValue)
{
	if (var != newValue)
	{
		var = newValue;
		isChanged_ = true;
	}
}