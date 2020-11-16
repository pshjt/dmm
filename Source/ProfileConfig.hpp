// Copyright 2013-2020 Marvin
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "Config.hpp"

class ProfileConfig : public Config
{
public:
	std::string mod_path;

	ProfileConfig()
	{
		configVariables();
	}

private:
	void configVariables() override
	{
		CVAR(mod_path, "");
	}
};
