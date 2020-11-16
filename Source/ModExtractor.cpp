#include "Precompiled.hpp"
#include "Message.hpp"

#include "ModExtractor.hpp"
#include "ApplicationConfig.hpp"



ModExtractor::ModExtractor(class wxWindow* parentWindow, const class ApplicationConfig& config)
	: parentWindow_(parentWindow),
	  config_(config)
{
}

ModExtractor::~ModExtractor()
{
	if(libraryLoaded_)
	{
		lib_7zdll_.Free();
	}	
}

bool ModExtractor::canExtract()
{
	return loadSevenZipLibrary(); // Lazy loading of 7zip.dll
}


bool ModExtractor::loadSevenZipLibrary()
{
	if (libraryLoaded_)
		return true;

	
	wxString pathDLL = config_.game.folderPath + "\\7z.dll";
	libraryLoaded_ = lib_7zdll_.Load(pathDLL.ToStdWstring());

	if (!libraryLoaded_)
	{
		pathDLL = config_.application.executableFolderPath + "\\7z.dll";
		libraryLoaded_ = lib_7zdll_.Load(pathDLL.ToStdWstring());
	}

	return libraryLoaded_;
}

void ModExtractor::extractModArchives(const wxString pathModFolder, const wxArrayString& modArchives)
{
	const bool SHOW_PROGRESS = true; // turn off if you don't want a progress bar
	
	if (!loadSevenZipLibrary()) // Lazy loading of 7zip.dll
	{
		showMessage(Message::MessageType::ERROR_OK, "Couldn't load 7z.dll .\n");
		return;
	}

	wxArrayString failedMods;
	float progressCounter = 1;

	wxProgressDialog progressBar_(wxString("Extracting mod archives..."), wxEmptyString, 1000, parentWindow_,
		wxPD_SMOOTH | wxPD_AUTO_HIDE | wxPD_ESTIMATED_TIME);
	if (!SHOW_PROGRESS)
	{
		progressBar_.Update(1000, wxEmptyString, nullptr);
		progressBar_.Hide();
	}

	for (auto& archiveName : modArchives)
	{
		if (SHOW_PROGRESS)
		{
			progressBar_.Update(
				static_cast<int>(progressCounter / static_cast<float>(modArchives.Count()) * 1000 * 0.99),
				wxString("Extracting mod " + wxFileName(archiveName).GetName()), nullptr);
			progressBar_.Show();
		}

		wxString pathExtract = pathModFolder + "\\" + wxFileName(archiveName).GetName();
		SevenZip::SevenZipExtractor modExtractor(lib_7zdll_, archiveName.ToStdWstring());

		if (modExtractor.DetectCompressionFormat())
		{
			const bool isSuccess = modExtractor.ExtractArchive(pathExtract.ToStdWstring(), nullptr);
			if (!isSuccess)
				failedMods.Add(archiveName + "\n");
		}
		else
		{
			failedMods.Add(archiveName + "\n");
		}

		progressCounter++;
	}
	if (SHOW_PROGRESS)
		progressBar_.Update(1000, wxString("Done."), nullptr);

	if (!failedMods.IsEmpty())
	{
		std::string error_output;
		for (auto& failed : failedMods)
			error_output = error_output + failed;

		showMessage(Message::MessageType::ERROR_OK, "Couldn't extract mod archives:\n", error_output);
	}
}
