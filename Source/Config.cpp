// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "Message.hpp"

#include "Config.hpp"

Config::Config()
{
	initialize();
}

Config::Config(const std::string& fileName)
{
	initialize();
	assignFile(fileName);
}

Config::~Config()
{
}

void Config::assignFile(const std::string& fileName)
{
	fileName_ = fileName;
}

bool Config::load()
{
	if (!wxFileExists(fileName_))
		return true;

	std::ifstream file(fileName_);

	if (file.fail())
	{
		errorMessage_ = Message::makeErrorMessage(Message::ErrorMessageType::ERROR_ACCESS_FILE, fileName_);
		return false;
	}

	std::vector<std::string> fileLines;

	std::string line;
	while (std::getline(file, line))
		fileLines.push_back(line);

	for (const auto& currentLine : fileLines)
	{
		std::string name, value;

		bool isKey = tokenize(name, value, currentLine);

		if (isKey)
		{
			auto it = findKey(name);

			if (it != keys_.end())
				it->value = value;
		}
	}

	setVariables();

	return true;
}

bool Config::saveOverwriting(const std::string& frontComment)
{
	std::ofstream file(fileName_);

	if (file.fail())
	{
		errorMessage_ = Message::makeErrorMessage(Message::ErrorMessageType::ERROR_WRITE_FILE, fileName_);
		return false;
	}

	if (!frontComment.empty())
		file << frontComment << std::endl;

	getVariables();

	for (const auto& i : keys_)
	{
		if (i.isReadOnly)
			continue;

		std::string line;
		computeKeyLine(i, line);

		file << line << std::endl;
	}

	if (file.fail())
	{
		errorMessage_ = Message::makeErrorMessage(Message::ErrorMessageType::ERROR_WRITE_FILE, fileName_);
		return false;
	}

	return true;
}

bool Config::saveUpdating()
{
	std::vector<std::string> fileLines;

	if (wxFileExists(fileName_))
	{
		std::ifstream file(fileName_);

		if (file.fail())
		{
			errorMessage_ = Message::makeErrorMessage(Message::ErrorMessageType::ERROR_ACCESS_FILE, fileName_);
			return false;
		}

		std::string line;
		while (std::getline(file, line))
			fileLines.push_back(line);

		Utils::trimTrailingNewLines(fileLines);
	}

	getVariables();

	bool isFileChanged = false;

	for (const auto& key : keys_)
	{
		if (key.isReadOnly)
			continue;

		bool isKeyLineFound = false;
		auto it = fileLines.rbegin();
		while (!isKeyLineFound && it != fileLines.rend())
		{
			std::string name, value;

			bool isKey = tokenize(name, value, *it);

			if (isKey)
				if (Utils::stringIsEqualNoCase(key.name, name))
				{
					isKeyLineFound = true;

					bool isNameTheSame = key.name.compare(name) == 0;
					bool isValueTheSame = key.value.compare(value) == 0;

					if (!isNameTheSame || !isValueTheSame)
					{
						computeKeyLine(key, *it);

						isFileChanged = true;
					}
				}

			++it;
		}

		if (!isKeyLineFound)
		{
			std::string line;
			computeKeyLine(key, line);
			fileLines.push_back(line);

			isFileChanged = true;
		}
	}

	if (isFileChanged)
	{
		std::ofstream file(fileName_);

		if (file.fail())
		{
			errorMessage_ = Message::makeErrorMessage(Message::ErrorMessageType::ERROR_WRITE_FILE, fileName_);
			return false;
		}

		for (const auto& line : fileLines)
			file << line << std::endl;

		if (file.fail())
		{
			errorMessage_ = Message::makeErrorMessage(Message::ErrorMessageType::ERROR_WRITE_FILE, fileName_);
			return false;
		}
	}

	return true;
}

std::string Config::getErrorMessage() const
{
	return errorMessage_;
}

void Config::configVariable(std::string& variable, const std::string& name, const char* defaultValue,
	bool isReadOnly /*= false*/)
{
	configVariable(variable, name, std::string(defaultValue), isReadOnly);
}

void Config::initialize()
{
	operationType_ = OperationType::ADD;
}

void Config::getVariables()
{
	operationType_ = OperationType::GET;
	keysIterator_ = keys_.begin();
	configVariables();
	operationType_ = OperationType::NONE;
}

void Config::setVariables()
{
	operationType_ = OperationType::SET;
	keysIterator_ = keys_.begin();
	configVariables();
	operationType_ = OperationType::NONE;
}

template <>
void Config::addVariable(std::string& variable, const std::string& name, const std::string& defaultValue, bool isReadOnly)
{
	variable = defaultValue;

	addKey(name, defaultValue, isReadOnly);
}

template <>
void Config::setVariable(std::string& variable)
{
	variable = keysIterator_++->value;
}

template <>
void Config::getVariable(const std::string& variable)
{
	keysIterator_++->value = variable;
}

void Config::addKey(const std::string& name, const std::string& defaultValue, bool isReadOnly)
{
	assert(findKey(name) == keys_.end());

	Key key;

	key.name = name;
	key.defaultValue = defaultValue;
	key.value = key.defaultValue;
	key.isReadOnly = isReadOnly;

	keys_.push_back(key);
}

std::vector<Config::Key>::iterator Config::findKey(const std::string& name)
{
	return std::find_if(keys_.begin(), keys_.end(), PredicateIsKeyNameEqualNoCase(name));
}

void Config::computeKeyLine(const Key& key, std::string& line)
{
	line = key.name;

	if (!key.value.empty())
	{
		line += " ";
		line += key.value;
	}
}

bool Config::tokenize(std::string& token, std::string& remainder, const std::string& sequence)
{
	if (!Utils::tokenize(token, remainder, sequence))
		return false;

	if (token[0] == ';')
		return false;

	return true;
}
