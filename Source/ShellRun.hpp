#pragma once

class ShellRun
{
public:
	ShellRun();

	bool open(const std::string& file, const std::string& workingDirectory = "");
	bool openFolderAndSelectItem(const std::string& item);
	std::string getErrorMessage() const;

private:
	std::string errorMessage_;
};