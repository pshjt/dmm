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
 * \brief 
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
