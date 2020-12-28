#include <Precompiled.hpp>

#include "ApplicationInfo.hpp"

namespace ApplicationInfo
{
	const std::string name = "Dark Mod Manager";
	const std::string versionMajor = "1";
	const std::string versionMinor = "0";
	const std::string releaseType = "";

	const std::string copyright = "\x00A9 2020 Marvin";

	const std::string license =
		"Dark Mod Manager is licensed under the MIT License.\n"
		"\n"
		"Copyright 2020 Marvin\n"
		"\n"
		"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated "
		"documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation "
		"the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to "
		"permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
		"\n"
		"The above copyright notice and this permission notice shall be included in all copies or substantial portions of "
		"the Software.\n"
		"\n"
		"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO "
		"THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
		"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, "
		"TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
		"SOFTWARE.\n"
		"================================================\n"
		"\n"
		"7z - cpp Modifications placed under MIT\n"
		"\n"
		"The MIT License(MIT)\n"
		"\n"
		"Copyright(c) Jan Kaniewski(Getnamo), Keith J.Jones(keithjjones)\n"
		"\n"
		"Permission is hereby granted, free of charge, to any person obtaining a copy "
		"of this software and associated documentation files(the \"Software\"), to deal "
		"in the Software without restriction, including without limitation the rights "
		"to use, copy, modify, merge, publish, distribute, sublicense, and / or sell "
		"copies of the Software, and to permit persons to whom the Software is "
		"furnished to do so, subject to the following conditions :\n"
		"\n"
		"The above copyright notice and this permission notice shall be included in all "
		"copies or substantial portions of the Software.\n"
		"\n"
		"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
		"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
		"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE "
		"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
		"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
		"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
		"SOFTWARE.\n"
		"\n"
		"================================================\n"
		"\n"
		"SevenZip++ (C++ wrapper) by Chadwick McNab is placed in the public domain. bitbucket.org/cmcnab/sevenzip/wiki/Home \n"
		"\n"
		"================================================\n"
		"LZMA SDK(LZMA 7 - zip headers) is placed in the public domain. www.7-zip.org/sdk.html \n"
		"================================================\n"
		"This software uses code of 7z.dll licensed under the LGPL + unrar restriction version 2.1 and its source "
		"can be downloaded at www.7 - zip.org \n"
		"7 - Zip\n"
		"~~~~~\n"
		"License for use and distribution\n"
		"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
		"\n"
		"7 - Zip Copyright(C) 1999 - 2015 Igor Pavlov.\n"
		"\n"
		"Licenses for files are :\n"
		"\n"
		"1) 7z.dll: GNU LGPL + unRAR restriction\n"
		"2) All other files : GNU LGPL\n"
		"\n"
		"The GNU LGPL + unRAR restriction means that you must follow both "
		"GNU LGPL rules and unRAR restriction rules.\n"
		"\n"
		"Note :\n"
		"You can use 7 - Zip on any computer, including a computer in a commercial "
		"organization.You don't need to register or pay for 7-Zip.\n"
		"\n"
		"GNU LGPL information\n"
		"--------------------\n"
		"\n"
		"This library is free software; you can redistribute it and / or "
		"modify it under the terms of the GNU Lesser General Public "
		"License as published by the Free Software Foundation; either "
		"version 2.1 of the License, or(at your option) any later version.\n"
		"This library is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU "
		"Lesser General Public License for more details.\n"
		"\n"
		"You can receive a copy of the GNU Lesser General Public License from "
		"http ://www.gnu.org/\n"
		"\n"
		"\n"
		"unRAR restriction\n"
		"-----------------\n"
		"\n"
		"The decompression engine for RAR archives was developed using source "
		"code of unRAR program.\n"
		"All copyrights to original unRAR code are owned by Alexander Roshal.\n"
		"\n"
		"The license for original unRAR code has the following restriction :\n"
		"The unRAR sources cannot be used to re - create the RAR compression algorithm, "
		"which is proprietary.Distribution of modified unRAR sources in separate form "
		"or as a part of other software is permitted, provided that it is clearly "
		"stated in the documentation and source comments that the code may "
		"not be used to develop a RAR(WinRAR) compatible archiver.\n"
		"--\n"
		"Igor Pavlov\n";
} // namespace ApplicationInfo

namespace ApplicationInfo
{
	const std::string version = [&]()
	{
		std::string str;

		if (!versionMajor.empty())
			str += versionMajor;

		if (!versionMinor.empty())
		{
			str += ".";
			str += versionMinor;
		}

		if (!releaseType.empty())
		{
			if (!str.empty())
				str += " ";

			str += releaseType;
		}

#ifdef _DEBUG
		if (!str.empty())
			str += " ";

		str += "Debug";
#endif

		return str;
	}();

	const std::string full = [&]()
	{
		std::string str;

		str += name;

		if (!version.empty())
		{
			if (!str.empty())
				str += " ";

			str += version;
		}

		return str;
	}();
} // namespace ApplicationInfo