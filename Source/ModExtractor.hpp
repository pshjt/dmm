#pragma once

/*7zpp*/
#define NOMINMAX

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4100) // virtual 7zpp callback functions
#endif

#include <7zpp/7zpp.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/**
 * Used for updating a wxProgressDialog when extracting archives.
 */
class ProgressCallbackHandler : public SevenZip::ProgressCallback
{
public:
	ProgressCallbackHandler() = delete;
	ProgressCallbackHandler(wxProgressDialog& progressDialog)
		: progress_(progressDialog)
	{};

	void OnStartWithTotal(const SevenZip::TString&, unsigned long long totalBytes) override
	{
		totalBytes_ = totalBytes;
	}

	void OnProgress(const SevenZip::TString&, unsigned long long bytesCompleted) override
	{
		totalBytesCompleted_ += bytesCompleted;
		progress_.Update(static_cast<double>(totalBytesCompleted_) / static_cast<double>(totalBytes_) * 1000.0);
	}

	void OnDone(const SevenZip::TString&) override
	{
		progress_.Update(1000);
	}

	bool OnCheckBreak() override { return false; };
	void OnFileDone(const SevenZip::TString&, const SevenZip::TString&, unsigned long long) override {};

private:
	wxProgressDialog& progress_;
	unsigned long long totalBytes_ = 0;
	unsigned long long totalBytesCompleted_ = 0;
};

/**
 * Handles extraction of mod archive files. The 7z.dll library is loaded lazily.
 */
class ModExtractor
{
public:
	ModExtractor() = delete;
	ModExtractor(class wxWindow* parentWindow, const class ApplicationConfig& config);
	~ModExtractor();

	bool canExtract();
	void extractModArchives(const wxString pathModFolder, const wxArrayString& modArchives);

private:
	bool loadSevenZipLibrary();

	class wxWindow* parentWindow_;
	const class ApplicationConfig& config_;

	SevenZip::SevenZipLibrary lib_7zdll_;
	bool libraryLoaded_ = false;
};