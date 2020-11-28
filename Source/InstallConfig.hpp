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
	void configVariables() override
	{
		CVAR(movie_path, "");
	}
};