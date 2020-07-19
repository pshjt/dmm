// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "Config.hpp"

class InstallConfig : public Config
{
public:
	std::string movie_path;

	InstallConfig()
	{
		configVariables();
	}

	// ~InstallConfig() {}

private:
	virtual void configVariables()
	{
		CVAR(movie_path, "");
	}
};