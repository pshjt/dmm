// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

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
