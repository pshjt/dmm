// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "Utils.hpp"

#include "Message.hpp"

namespace Message
{
	MessageAnswer showMessage(MessageType messageType, const std::string& message,
	                          const std::string& extendedMessage /*= ""*/,
	                          const std::string& caption /*= applicationinfo::name*/)
	{
#ifdef _DEBUG
		wxLogDebug(wxString() << "Message: " << message);

		if (!extendedMessage.empty())
			wxLogDebug(wxString() << "Extended message: " << extendedMessage);

		wxLogDebug(wxEmptyString);
#endif

		long style = 0;

		switch (messageType)
		{
		case MessageType::ERROR_OK:
			style = wxOK | wxICON_ERROR;
			break;

		case MessageType::WARNING_OK:
			style = wxOK | wxICON_WARNING;
			break;

		case MessageType::INFORMATION_OK:
			style = wxOK | wxICON_INFORMATION;
			break;

		case MessageType::WARNING_OK_CANCEL:
			style = wxOK | wxCANCEL | wxICON_WARNING;
			break;

		case MessageType::QUESTION_YES_CANCEL:
			style = wxOK | wxCANCEL | wxICON_QUESTION;
			break;
		}

		style |= wxCENTRE;

		wxMessageDialog dialog(nullptr, wxString(message), wxString(caption), style);

		if (!extendedMessage.empty())
			dialog.SetExtendedMessage(wxString(extendedMessage));

		if (messageType == MessageType::QUESTION_YES_CANCEL)
			dialog.SetOKLabel(wxMessageDialog::ButtonLabel(wxString("Yes")));
		else
			dialog.SetOKLabel(wxMessageDialog::ButtonLabel(wxString("OK")));

		dialog.SetYesNoCancelLabels(wxMessageDialog::ButtonLabel(wxString("Yes")),
		                            wxMessageDialog::ButtonLabel(wxString("No")),
		                            wxMessageDialog::ButtonLabel(wxString("Cancel")));

		int answer = dialog.ShowModal();
		MessageAnswer messageAnswer;

		switch (answer)
		{
		case wxID_OK:
			if (messageType == MessageType::QUESTION_YES_CANCEL)
				messageAnswer = MessageAnswer::YES;
			else
				messageAnswer = MessageAnswer::OK;

			break;

		case wxID_YES:
			messageAnswer = MessageAnswer::YES;
			break;

		case wxID_NO:
			messageAnswer = MessageAnswer::NO;
			break;

		case wxID_CANCEL:
			messageAnswer = MessageAnswer::CANCEL;
			break;

		default:
			messageAnswer = MessageAnswer::UNKNOWN;
		}

		return messageAnswer;
	}

	std::string makeErrorMessage(ErrorMessageType type, const std::string& item)
	{
		std::string errorMessage;

		switch (type)
		{
		case ErrorMessageType::ERROR_ACCESS_FILE:
			errorMessage = "Failed to access file:\n" + item;
			break;

		case ErrorMessageType::ERROR_WRITE_FILE:
			errorMessage = "Failed to write file:\n" + item;
			break;

		case ErrorMessageType::ERROR_ACCESS_DIRECTORY:
			errorMessage = "Failed to access folder:\n" + item;
			break;

		case ErrorMessageType::ERROR_CREATE_DIRECTORY:
			errorMessage = "Failed to create folder:\n" + item;
			break;
		}

		return errorMessage;
	}

	std::string makeSystemErrorMessage(DWORD systemErrorCode)
	{
		std::string systemErrorMessage = "System error code: " + Utils::toString(systemErrorCode);

		LPSTR errorText = nullptr;

		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			systemErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
			reinterpret_cast<LPSTR>(&errorText),
			0,
			nullptr
		);

		if (errorText)
		{
			systemErrorMessage += "\n\n";
			systemErrorMessage += errorText;
			systemErrorMessage.erase(systemErrorMessage.find_last_not_of("\r\n") + 1, std::string::npos);

			LocalFree(errorText);
		}

		return systemErrorMessage;
	}
} // namespace Message
