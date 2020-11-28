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
	void configVariables() override
	{
		CVAR_READ_ONLY(uber_mod_path, "");
		CVAR(mod_path, "");
	}
};