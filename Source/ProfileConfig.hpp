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