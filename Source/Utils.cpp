// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "Utils.hpp"

namespace Utils {
	void stringToLowerCase(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](int c) -> char { return static_cast<char>(::tolower(c)); });
	}

	void stringToUpperCase(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](int c) -> char { return static_cast<char>(::toupper(c)); });
	}

	bool stringIsEqualNoCase(const std::string& str1, const std::string& str2)
	{
		if (str1.length() != str2.length())
			return false;

		for (auto c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2)
			if (std::tolower(*c1) != std::tolower(*c2))
				return false;

		return true;
	}

	bool stringIsLessNoCase(const std::string& str1, const std::string& str2)
	{
		auto str1Begin = str1.begin();
		auto str1End = str1.end();
		auto str2Begin = str2.begin();
		auto str2End = str2.end();

		for (; str1Begin != str1End && str2Begin != str2End; ++str1Begin, ++str2Begin)
		{
			int lowerC1 = std::tolower(*str1Begin);
			int lowerC2 = std::tolower(*str2Begin);

			if (lowerC1 < lowerC2)
				return true;

			if (lowerC1 > lowerC2)
				return false;
		}

		return (str2Begin != str2End);
	}

	void stringTrimLeading(std::string& str)
	{
		str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), ::isspace));
	}

	void stringTrimTrailing(std::string& str)
	{
		str.erase(std::find_if_not(str.rbegin(), str.rend(), ::isspace).base(), str.end());
	}

	void stringTrim(std::string& str)
	{
		stringTrimLeading(str);
		stringTrimTrailing(str);
	}

	void stringTrimLeading(std::string& str, char c)
	{
		str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), CharPredicateIsEqual(c)));
	}

	void stringTrimTrailing(std::string& str, char c)
	{
		str.erase(std::find_if_not(str.rbegin(), str.rend(), CharPredicateIsEqual(c)).base(), str.end());
	}

	void stringTrim(std::string& str, char c)
	{
		stringTrimLeading(str, c);
		stringTrimTrailing(str, c);
	}

	void stringMakeNonBreakingSpaces(std::string& str)
	{
		std::replace(str.begin(), str.end(), ' ', '\x00A0');
	}

	void stringReplaceCharWithChar(std::string& str, char c1, char c2)
	{
		std::replace(str.begin(), str.end(), c1, c2);
	}

	bool tokenize(std::string& token, std::string& remainder, const std::string& sequence)
	{
		std::string str(sequence);
		Utils::stringTrim(str);

		if (str.empty())
			return false;

		auto it = std::find_if(str.begin(), str.end(), ::isspace);

		token = std::string(str.begin(), it);
		remainder = std::string(std::find_if_not(it, str.end(), ::isspace), str.end());

		return true;
	}

	bool tokenize(std::string& token, std::string& remainder, const std::string& sequence, char delimiter)
	{
		std::string str(sequence);
		Utils::stringTrim(str);
		Utils::stringTrim(str, delimiter);

		if (str.empty())
			return false;

		auto it = std::find_if(str.begin(), str.end(), CharPredicateIsEqual(delimiter));

		token = std::string(str.begin(), it);
		remainder = std::string(std::find_if_not(it, str.end(), CharPredicateIsEqual(delimiter)), str.end());

		Utils::stringTrimTrailing(token);
		Utils::stringTrimLeading(remainder);

		return true;
	}

	void trimTrailingNewLines(std::vector<std::string>& lines)
	{
		auto it = lines.rbegin();

		if (it == lines.rend())
			return;

		bool emptyLine = true;
		while (emptyLine && it != lines.rend())
		{
			std::string line = *it;
			Utils::stringTrim(line);

			if (!line.empty())
				emptyLine = false;
			else
				++it;
		}

		lines.erase(it.base(), lines.end());
	}

	void normalizePath(std::string& path)
	{
		assert(path.find_first_of('/') == std::string::npos);

		stringTrim(path);
		std::replace(path.begin(), path.end(), '/', '\\');

		if (path[0] == '.' && path[1] == '\\')
			path.erase(0, 2);

		stringTrim(path, '\\');
	}

	int getPathLength(const std::string& path)
	{
		std::string result(path);

		stringTrim(result);
		std::replace(result.begin(), result.end(), '\\', '/');

		if (result[0] == '.' && result[1] == '/')
			result.erase(0, 1);

		stringTrim(result, '/');
		return  result.length();
	}

	void listDirectoryContent(std::string& directoryContent, const std::string& path, int maxDepth, int indent /*= 0*/)
	{
		struct CmpNoCase
		{
			bool operator() (const wxString& str1, const wxString& str2) const
			{
				return str1.CmpNoCase(str2) < 0;
			}
		};

		if (maxDepth < 1)
			return;

		wxString basePath(path);

		std::vector<wxString> dirs;
		std::vector<wxString> files;

		{
			wxDir sysDir;
			if (!sysDir.Open(basePath))
				return;

			wxString foundName;

			bool isDirFound = sysDir.GetFirst(&foundName, wxEmptyString, wxDIR_DIRS | wxDIR_HIDDEN);
			while (isDirFound)
			{
				dirs.push_back(foundName);
				isDirFound = sysDir.GetNext(&foundName);
			}

			bool isFileFound = sysDir.GetFirst(&foundName, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN);
			while (isFileFound)
			{
				files.push_back(foundName);
				isFileFound = sysDir.GetNext(&foundName);
			}
		}

		std::sort(dirs.begin(), dirs.end(), CmpNoCase());
		std::sort(files.begin(), files.end(), CmpNoCase());

		for (auto& dir : dirs)
		{
			for (int i = 0; i < indent; ++i)
				directoryContent += '\t';

			directoryContent += std::string(dir) + '\\' + '\n';

			listDirectoryContent(directoryContent, std::string(basePath + '\\' + dir), maxDepth - 1, indent + 1);
		}

		for (auto& file : files)
		{
			for (int i = 0; i < indent; ++i)
				directoryContent += '\t';

			directoryContent += std::string(file) + '\n';
		}
	}

	void adjustSizeUp(wxWindow* window, int minWidth /*= -1*/, int minHeight /*= -1*/)
	{
		const float aspectRatio = 4.0f / 3.0f;

		wxSize size = window->GetSize();
		int w = std::max(minWidth, size.GetWidth());
		int h = std::max(minHeight, size.GetHeight());

		w = std::max(w, 1);
		h = std::max(h, 1);

		float currentAspectRatio = static_cast<float>(w) / static_cast<float>(h);

		if (currentAspectRatio < aspectRatio)
		{
			w = static_cast<int>(static_cast<float>(h) * aspectRatio);
		}
		else if (currentAspectRatio > aspectRatio)
		{
			h = static_cast<int>(static_cast<float>(w) / aspectRatio);
		}

		w = std::max(w, 1);
		h = std::max(h, 1);

		wxSize newSize(w, h);

		window->SetSize(newSize);
		window->Centre(wxBOTH);
	}

	void adjustSizeDown(wxWindow* window, int maxWidth /*= -1*/, int maxHeight /*= -1*/)
	{
		const float aspectRatio = 4.0f / 3.0f;

		wxSize size = window->GetSize();
		int w = size.GetWidth();
		int h = size.GetHeight();

		if (maxWidth > -1)
			w = maxWidth;

		if (maxHeight > -1)
			h = maxHeight;

		w = std::max(w, 1);
		h = std::max(h, 1);

		float currentAspectRatio = static_cast<float>(w) / static_cast<float>(h);

		if (currentAspectRatio < aspectRatio)
		{
			h = static_cast<int>(static_cast<float>(w) / aspectRatio);
		}
		else if (currentAspectRatio > aspectRatio)
		{
			w = static_cast<int>(static_cast<float>(h) * aspectRatio);
		}

		w = std::max(w, 1);
		h = std::max(h, 1);

		wxSize newSize(w, h);

		window->SetSize(newSize);
		window->Centre(wxBOTH);
	}

	bool stringToPoint(int& x, int& y, const std::string& str)
	{
		std::string strX, strY;

		if (!tokenize(strX, strY, str))
			return false;

		if (!stringTo(strX, x))
			return false;

		if (!stringTo(strY, y))
			return false;

		return true;
	}

	void pointToString(int x, int y, std::string& str)
	{
		str = toString(x);
		str += ' ';
		str += toString(y);
	}

	bool getVersionInfo(const std::string& filePath, std::string& productName, std::string& productVersion)
	{
		DWORD fileVersionInfoSize = GetFileVersionInfoSizeA(filePath.c_str(), nullptr);

		if (fileVersionInfoSize == 0)
			return false;

		std::vector<BYTE> data(fileVersionInfoSize);

		if (GetFileVersionInfoA(filePath.c_str(), 0, fileVersionInfoSize, &data[0]) == 0)
			return false;

		LPVOID productNameBuffer = nullptr;
		UINT productNameLength = 0;
		LPVOID productVersionBuffer = nullptr;
		UINT productVersionLength = 0;

		if (VerQueryValueA(&data[0], "\\StringFileInfo\\040904B0\\ProductName", &productNameBuffer, &productNameLength) == 0)
			return false;

		if (VerQueryValueA(&data[0], "\\StringFileInfo\\040904B0\\ProductVersion", &productVersionBuffer,
			&productVersionLength) == 0)
		{
			return false;
		}

		if (productNameLength < 1 || productVersionLength < 1)
			return false;

		--productNameLength;
		--productVersionLength;

		productName.assign(static_cast<char*>(productNameBuffer), productNameLength);
		productVersion.assign(static_cast<char*>(productVersionBuffer), productVersionLength);

		return true;
	}

	bool checkVersion(const std::string& requiredVersion, std::string& version)
	{
		auto dotCount = std::count(version.begin(), version.end(), '.');
		auto wrongCharacter = version.find_first_not_of("0123456789.");

		if (dotCount > 1 || wrongCharacter != std::string::npos)
		{
			version = "unknown";

			return false;
		}

		auto dot = std::find(requiredVersion.begin(), requiredVersion.end(), '.');

		int requiredMajor = 0;
		Utils::stringTo(std::string(requiredVersion.begin(), dot), requiredMajor);

		std::string requiredMinor(dot, requiredVersion.end());

		dot = std::find(version.begin(), version.end(), '.');

		int major = 0;
		Utils::stringTo(std::string(version.cbegin(), dot), major);

		std::string minor(dot, version.cend());

		if (requiredMajor > major)
			return false;
		else if (requiredMajor < major)
			return true;
		else
			return !std::lexicographical_compare(minor.begin(), minor.end(), requiredMinor.begin(), requiredMinor.end());
	}

	void getCurrentDateTime(std::string& dateTime)
	{
		wxDateTime dt = wxDateTime::Now();
		std::ostringstream oss;

		oss << dt.GetDay() << " ";
		oss << dt.GetMonthName(dt.GetMonth(), wxDateTime::Name_Full) << " ";
		oss << dt.GetYear() << ", ";

		oss.fill('0');

		oss << std::setw(2) << dt.GetHour() << ":";
		oss << std::setw(2) << dt.GetMinute() << ":";
		oss << std::setw(2) << dt.GetSecond();

		dateTime = oss.str();
	}
} // namespace Utils