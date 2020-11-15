// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

namespace Utils
{
	void stringToLowerCase(std::string& str);
	void stringToUpperCase(std::string& str);
	bool stringIsEqualNoCase(const std::string& str1, const std::string& str2);
	bool stringIsLessNoCase(const std::string& str1, const std::string& str2);
	template <typename T>
	std::string toString(const T& value);
	template <typename T>
	void toString(const T& value, std::string& str);
	template <typename T>
	bool isStringValid(std::string& str);
	template <typename T>
	T stringTo(std::string& str);
	template <typename T>
	bool stringTo(const std::string& str, T& value);
	void stringTrimLeading(std::string& str);
	void stringTrimTrailing(std::string& str);
	void stringTrim(std::string& str);
	void stringTrimLeading(std::string& str, char c);
	void stringTrimTrailing(std::string& str, char c);
	void stringTrim(std::string& str, char c);
	void stringMakeNonBreakingSpaces(std::string& str);
	void stringReplaceCharWithChar(std::string& str, char c1, char c2);

	bool tokenize(std::string& token, std::string& remainder, const std::string& sequence);
	bool tokenize(std::string& token, std::string& remainder, const std::string& sequence, char delimiter);

	void trimTrailingNewLines(std::vector<std::string>& lines);

	void normalizePath(std::string& path);
	int getPathLength(const std::string& path);
	void listDirectoryContent(std::string& directoryContent, const std::string& path, int maxDepth, int indent = 0);

	void adjustSizeUp(wxWindow* window, int minWidth = -1, int minHeight = -1);
	void adjustSizeDown(wxWindow* window, int maxWidth = -1, int maxHeight = -1);
	bool stringToPoint(int& x, int& y, const std::string& str);
	void pointToString(int x, int y, std::string& str);

	bool getVersionInfo(const std::string& filePath, std::string& productName, std::string& productVersion);
	bool checkVersion(const std::string& requiredVersion, std::string& version);
	void getCurrentDateTime(std::string& dateTime);

	class CharPredicateIsEqual;
	class StringPredicateIsEqualNoCase;
} // namespace Utils

namespace Utils
{
	template <typename T>
	std::string toString(const T& value)
	{
		std::ostringstream oss;
		oss << value;

		return oss.str();
	}

	template <typename T>
	void toString(const T& value, std::string& str)
	{
		std::ostringstream oss;
		oss << value;

		str = oss.str();
	}

	template <typename T>
	bool isStringValid(std::string& str)
	{
		std::istringstream iss(str);
		T value;
		iss >> std::noskipws >> value;

		return iss.eof() && !iss.fail();
	}

	template <typename T>
	T stringTo(std::string& str)
	{
		std::istringstream iss(str);
		T value;
		iss >> value;

		return value;
	}

	template <typename T>
	bool stringTo(const std::string& str, T& value)
	{
		std::istringstream iss(str);
		T temp;
		iss >> std::noskipws >> temp;

		if (iss.eof() && !iss.fail())
		{
			value = temp;
			return true;
		}

		return false;
	}

	class CharPredicateIsEqual
	{
	public:
		CharPredicateIsEqual(char c) : c_(c)
		{
		}

		bool operator ()(char c) const
		{
			return c_ == c;
		}

	private:
		char c_;
	};

	class StringPredicateIsEqualNoCase
	{
	public:
		StringPredicateIsEqualNoCase(const std::string& str) : str_(str)
		{
		}

		bool operator ()(const std::string& str) const
		{
			return stringIsEqualNoCase(str_, str);
		}

	private:
		std::string str_;
	};
} // namespace Utils
