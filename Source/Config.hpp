// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

#include "Utils.hpp"

#define CVAR(variable, defaultValue) configVariable(variable, #variable, defaultValue);
#define CVAR_READ_ONLY(variable, defaultValue) configVariable(variable, #variable, defaultValue, true);

class Config
{
public:
	Config();
	Config(const std::string& fileName);
	virtual ~Config();

	void assignFile(const std::string& fileName);
	bool load();
	bool saveOverwriting(const std::string& frontComment);
	bool saveUpdating();

	std::string getErrorMessage() const;

protected:
	virtual void configVariables() = 0;

	template <typename T>
	void configVariable(T& variable, const std::string& name, const T& defaultValue,
	                    bool isReadOnly = false);

	void configVariable(std::string& variable, const std::string& name, const char* defaultValue,
	                    bool isReadOnly = false);

private:
	enum class OperationType
	{
		ADD,
		GET,
		SET,
		NONE,
	};

	struct Key
	{
		std::string name;
		std::string defaultValue;
		std::string value;
		bool isReadOnly;
	};

	class PredicateIsKeyNameEqualNoCase
	{
	public:
		PredicateIsKeyNameEqualNoCase(const std::string& str) : str_(str)
		{
		}

		bool operator ()(const Key& key)
		{
			return Utils::stringIsEqualNoCase(str_, key.name);
		}

	private:
		std::string str_;
	};

	// Non copyable.
	Config(const Config&); // Not to implement.
	Config& operator =(const Config&); // Not to implement.

	void initialize();
	void getVariables();
	void setVariables();

	template <typename T>
	void addVariable(T& variable, const std::string& name, const T& defaultValue, bool isReadOnly);
	template <typename T>
	void setVariable(T& variable);
	template <typename T>
	void getVariable(const T& variable);

	void addKey(const std::string& name, const std::string& defaultValue, bool isReadOnly);
	std::vector<Key>::iterator findKey(const std::string& name);
	static void computeKeyLine(const Key& key, std::string& line);
	bool tokenize(std::string& token, std::string& remainder, const std::string& sequence);

	std::string fileName_;
	OperationType operationType_;
	std::vector<Key> keys_;
	std::vector<Key>::iterator keysIterator_;

	std::string errorMessage_;
};

template <typename T>
void Config::configVariable(T& variable, const std::string& name, const T& defaultValue, bool isReadOnly /*= false*/)
{
	switch (operationType_)
	{
	case OperationType::ADD:
		addVariable(variable, name, defaultValue, isReadOnly);
		break;

	case OperationType::GET:
		getVariable(variable);
		break;

	case OperationType::SET:
		setVariable(variable);
		break;

	case OperationType::NONE:
		break;
	}
}

template <typename T>
void Config::addVariable(T& variable, const std::string& name, const T& defaultValue, bool isReadOnly)
{
	variable = defaultValue;

	addKey(name, Utils::toString(defaultValue), isReadOnly);
}

template <typename T>
void Config::setVariable(T& variable)
{
	Utils::stringTo(keysIterator_++->value, variable);
}

template <typename T>
void Config::getVariable(const T& variable)
{
	Utils::toString(variable, keysIterator_++->value);
}
