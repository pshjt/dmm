// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "Config.hpp"

class ModConfig : public Config
{
public:
	std::string mod_path;
	std::string uber_mod_path;

	ModConfig()
	{
		configVariables();
	}

	// ~ModConfig() {}

private:
	virtual void configVariables()
	{
		CVAR_READ_ONLY(uber_mod_path, "");
		CVAR(mod_path, "");
	}
};