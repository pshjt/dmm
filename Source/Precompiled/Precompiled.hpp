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
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

/*wxWidgets*/
#ifdef NDEBUG
#define wxNO_ADV_LIB
#define wxNO_AUI_LIB
#define wxNO_HTML_LIB
#define wxNO_MEDIA_LIB
#define wxNO_NET_LIB
#define wxNO_PROPGRID_LIB
#define wxNO_QA_LIB
#define wxNO_RICHTEXT_LIB
#define wxNO_WEBVIEW_LIB
#define wxNO_XML_LIB
#define wxNO_REGEX_LIB
#define wxNO_EXPAT_LIB
#define wxNO_JPEG_LIB
#define wxNO_PNG_LIB
#define wxNO_TIFF_LIB
#define wxNO_ZLIB_LIB
#endif

#ifndef WX_PRECOMP
#define WX_PRECOMP
#endif

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 4996)
#endif
//
#include <wx/wxprec.h>
//
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
#include "../../wxFormBuilder/DarkModManager.h"