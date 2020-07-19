// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include <Shlobj.h>

#include "Message.hpp"
#include "Utils.hpp"

#include "ShellRun.hpp"

ShellRun::ShellRun()
{
}

bool ShellRun::open(const std::string& file, const std::string& workingDirectory /*= ""*/)
{
	errorMessage_.clear();

	SHELLEXECUTEINFOA shellExecuteInfo;
	ZeroMemory(&shellExecuteInfo, sizeof(shellExecuteInfo));

	shellExecuteInfo.cbSize = sizeof(shellExecuteInfo);
	shellExecuteInfo.fMask = SEE_MASK_FLAG_NO_UI;
	shellExecuteInfo.lpVerb = "open";
	shellExecuteInfo.lpFile = file.c_str();

	if (!workingDirectory.empty())
		shellExecuteInfo.lpDirectory = workingDirectory.c_str();

	shellExecuteInfo.nShow = SW_SHOW;

	BOOL isSuccess = ShellExecuteExA(&shellExecuteInfo);

	if (isSuccess)
		return true;

	DWORD lastError = GetLastError();

	if (lastError == ERROR_CANCELLED)
	{
		// Canceled by user, it's OK.
		return true;
	}

	errorMessage_ = Message::makeSystemErrorMessage(lastError);

	return false;
}

bool ShellRun::openFolderAndSelectItem(const std::string& item)
{
	errorMessage_.clear();

	ITEMIDLIST* itemIdList = ILCreateFromPathA(item.c_str());

	if (!itemIdList)
	{
		errorMessage_ = Message::makeSystemErrorMessage(GetLastError());

		return false;
	}

	HRESULT result = SHOpenFolderAndSelectItems(itemIdList, 0, nullptr, 0);
	ILFree(itemIdList);

	if (result == S_OK)
		return true;

	errorMessage_ = Message::makeSystemErrorMessage(result);

	return false;
}

std::string ShellRun::getErrorMessage() const
{
	return errorMessage_;
}