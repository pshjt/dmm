#pragma once

#include "ApplicationInfo.hpp"

namespace Message
{
	enum class MessageType
	{
		ERROR_OK,
		WARNING_OK,
		INFORMATION_OK,
		WARNING_OK_CANCEL,
		QUESTION_YES_CANCEL,
	};

	enum class MessageAnswer
	{
		OK,
		YES,
		NO,
		CANCEL,
		UNKNOWN
	};

	MessageAnswer showMessage(MessageType messageType, const std::string& message,
		const std::string& extendedMessage = "",
		const std::string& caption = ApplicationInfo::name);

	enum class ErrorMessageType
	{
		ERROR_ACCESS_FILE,
		ERROR_WRITE_FILE,
		ERROR_ACCESS_DIRECTORY,
		ERROR_CREATE_DIRECTORY
	};

	std::string makeErrorMessage(ErrorMessageType type, const std::string& item);
	std::string makeSystemErrorMessage(DWORD systemErrorCode);
} // namespace Message