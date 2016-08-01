// Copyright 2013-2014 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

/*General*/
#include <cassert>
#include <cctype>

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


/*7zpp*/
#define NOMINMAX
#include <windows.h>
#include <atlbase.h>
#include <vector>
#include <deque>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4100) // virtual 7zpp callback functions
#endif

#include <7zpp/7zpp.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif


/*wxWidgets*/
#ifdef NDEBUG
	#define wxNO_NET_LIB
	#define wxNO_XML_LIB
	#define wxNO_REGEX_LIB
	#define wxNO_EXPAT_LIB
	#define wxNO_JPEG_LIB
	#define wxNO_PNG_LIB
	#define wxNO_TIFF_LIB
	#define wxNO_ZLIB_LIB
	//#define wxNO_ADV_LIB
	#define wxNO_HTML_LIB
	#define wxNO_GL_LIB
	#define wxNO_QA_LIB
	#define wxNO_XRC_LIB
	#define wxNO_AUI_LIB
	#define wxNO_PROPGRID_LIB
	#define wxNO_RIBBON_LIB
	#define wxNO_RICHTEXT_LIB
	#define wxNO_MEDIA_LIB
	#define wxNO_STC_LIB
	#define wxNO_WEBVIEW_LIB
#endif

#ifndef WX_PRECOMP
	#define WX_PRECOMP
#endif

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4996)
#endif

#include <wx/wxprec.h>

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include <wx/display.h>
#include <wx/fswatcher.h>
#include <wx/hyperlink.h>
#include <wx/imaglist.h>
#include <wx/stdpaths.h>
#include <wx/progdlg.h>
#include <wx/richmsgdlg.h>


/*wxFormBuilder GUI*/
#include "../../wxFormBuilder/SS2BlueModManager.h"
