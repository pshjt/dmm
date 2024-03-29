### Build flags
set(wxBUILD_SHARED OFF CACHE BOOL "Build wxWidgets as shared libraries")
set(wxBUILD_USE_STATIC_RUNTIME ON CACHE BOOL "Link using the static runtime library")
set(wxBUILD_COMPATIBILITY "3.1" CACHE STRING "") 

### Use-flags from setup.h
# The CMake files of wxWidgets 3.2 are a bit buggy when it comes to flags.
# We might want to switch to having a copy of the setup.h file that we
# use to overwrite the existing one at the end of the configure step.

# Big contributers, easy to switch off
set(wxUSE_STL ON CACHE BOOL "use standard C++ classes for everything")
set(wxUSE_STD_CONTAINERS ON CACHE BOOL "use standard C++ container classes")
set(wxUSE_ZLIB "OFF" CACHE STRING "")
set(wxUSE_EXPAT "OFF" CACHE STRING "")
set(wxUSE_LIBJPEG "OFF" CACHE STRING "")
set(wxUSE_LIBPNG "OFF" CACHE STRING "")
set(wxUSE_LIBTIFF "OFF" CACHE STRING "")
set(wxUSE_NANOSVG "OFF" CACHE STRING "")
set(wxUSE_LIBLZMA "OFF" CACHE STRING "")
set(wxUSE_ARTPROVIDER_TANGO OFF CACHE BOOL "")
set(wxUSE_SVG OFF CACHE BOOL "")

# Smaller aspects
set(wxUSE_ON_FATAL_EXCEPTION OFF CACHE BOOL "")
set(wxUSE_DEBUGREPORT OFF CACHE BOOL "")
set(wxUSE_EXCEPTIONS OFF CACHE BOOL "")
set(wxUSE_CMDLINE_PARSER OFF CACHE BOOL "")
set(wxUSE_SECRETSTORE OFF CACHE BOOL "")
set(wxUSE_SPELLCHECK OFF CACHE BOOL "")
set(wxUSE_STOPWATCH OFF CACHE BOOL "")
set(wxUSE_DIALUP_MANAGER OFF CACHE BOOL "")
set(wxUSE_SOCKETS OFF CACHE BOOL "")
set(wxUSE_FILESYSTEM OFF CACHE BOOL "")
set(wxUSE_FS_ZIP OFF CACHE BOOL "")
set(wxUSE_FS_ARCHIVE OFF CACHE BOOL "")
set(wxUSE_FS_INET OFF CACHE BOOL "")
set(wxUSE_ARCHIVE_STREAMS OFF CACHE BOOL "")
set(wxUSE_ZIPSTREAM OFF CACHE BOOL "")
set(wxUSE_TARSTREAM OFF CACHE BOOL "")
set(wxUSE_APPLE_IEEE OFF CACHE BOOL "")
set(wxUSE_JOYSTICK OFF CACHE BOOL "")
set(wxUSE_FONTENUM OFF CACHE BOOL "")
set(wxUSE_FONTMAP OFF CACHE BOOL "")
set(wxUSE_MIMETYPE OFF CACHE BOOL "")
set(wxUSE_WEBREQUEST OFF CACHE BOOL "")
set(wxUSE_PROTOCOL_FILE OFF CACHE BOOL "")
set(wxUSE_PROTOCOL_FTP OFF CACHE BOOL "")
set(wxUSE_PROTOCOL_HTTP OFF CACHE BOOL "")
set(wxUSE_URL OFF CACHE BOOL "")
set(wxUSE_ANY OFF CACHE BOOL "")
set(wxUSE_REGEX OFF CACHE BOOL "")
set(wxUSE_SYSTEM_OPTIONS OFF CACHE BOOL "")
set(wxUSE_SOUND OFF CACHE BOOL "")
set(wxUSE_MEDIACTRL OFF CACHE BOOL "")
set(wxUSE_XRC OFF CACHE BOOL "")
set(wxUSE_XML OFF CACHE BOOL "")
set(wxUSE_AUI OFF CACHE BOOL "")
set(wxUSE_RIBBON OFF CACHE BOOL "")
set(wxUSE_PROPGRID OFF CACHE BOOL "")
set(wxUSE_STC OFF CACHE BOOL "")
set(wxUSE_WEBVIEW OFF CACHE BOOL "")
set(wxUSE_POPUPWIN OFF CACHE BOOL "")
set(wxUSE_TIPWINDOW OFF CACHE BOOL "")
set(wxUSE_TOOLBAR OFF CACHE BOOL "")
set(wxUSE_TOOLBAR_NATIVE OFF CACHE BOOL "")
set(wxUSE_TOOLBOOK OFF CACHE BOOL "")
set(wxUSE_GRID OFF CACHE BOOL "")
set(wxUSE_HOTKEY OFF CACHE BOOL "")
set(wxUSE_CARET OFF CACHE BOOL "")
set(wxUSE_GEOMETRY OFF CACHE BOOL "")
set(wxUSE_INFOBAR OFF CACHE BOOL "")
set(wxUSE_NOTIFICATION_MESSAGE OFF CACHE BOOL "")
set(wxUSE_PREFERENCES_EDITOR OFF CACHE BOOL "")
set(wxUSE_RICHTOOLTIP OFF CACHE BOOL "")
set(wxUSE_VALIDATORS OFF CACHE BOOL "")
set(wxUSE_COMMON_DIALOGS OFF CACHE BOOL "")
set(wxUSE_MDI OFF CACHE BOOL "")
set(wxUSE_DOC_VIEW_ARCHITECTURE OFF CACHE BOOL "")
set(wxUSE_MDI_ARCHITECTURE OFF CACHE BOOL "")
set(wxUSE_PRINTING_ARCHITECTURE OFF CACHE BOOL "")
set(wxUSE_HTML OFF CACHE BOOL "")
set(wxUSE_GLCANVAS OFF CACHE BOOL "")
set(wxUSE_RICHTEXT OFF CACHE BOOL "")
set(wxUSE_DRAG_AND_DROP OFF CACHE BOOL "")
set(wxUSE_ACCESSIBILITY OFF CACHE BOOL "")
set(wxUSE_DC_TRANSFORM_MATRIX OFF CACHE BOOL "")
set(wxUSE_TGA OFF CACHE BOOL "")
set(wxUSE_GIF OFF CACHE BOOL "")
set(wxUSE_PNM OFF CACHE BOOL "")
set(wxUSE_PCX OFF CACHE BOOL "")
set(wxUSE_PALETTE OFF CACHE BOOL "")
set(wxUSE_OLE_AUTOMATION OFF CACHE BOOL "")
set(wxUSE_ACTIVEX OFF CACHE BOOL "")
set(wxUSE_CRASHREPORT OFF CACHE BOOL "")
set(wxUSE_GRAPHICS_CONTEXT OFF CACHE BOOL "")
set(wxUSE_ADDREMOVECTRL OFF CACHE BOOL "")
set(wxUSE_ACTIVITYINDICATOR OFF CACHE BOOL "")
set(wxUSE_ANIMATIONCTRL OFF CACHE BOOL "")
set(wxUSE_BANNERWINDOW  OFF CACHE BOOL "")
set(wxUSE_BMPBUTTON     OFF CACHE BOOL "")
set(wxUSE_CALENDARCTRL  OFF CACHE BOOL "")
set(wxUSE_CHECKLISTBOX  OFF CACHE BOOL "")
set(wxUSE_COLOURPICKERCTRL OFF CACHE BOOL "")
set(wxUSE_COMBOBOX      OFF CACHE BOOL "")
set(wxUSE_COMMANDLINKBUTTON OFF CACHE BOOL "")
set(wxUSE_DATAVIEWCTRL  OFF CACHE BOOL "")
set(wxUSE_DATEPICKCTRL  OFF CACHE BOOL "")
set(wxUSE_DIRPICKERCTRL OFF CACHE BOOL "")
set(wxUSE_EDITABLELISTBOX OFF CACHE BOOL "")
set(wxUSE_FILECTRL      OFF CACHE BOOL "")
set(wxUSE_FILEPICKERCTRL OFF CACHE BOOL "")
set(wxUSE_FONTPICKERCTRL OFF CACHE BOOL "")
set(wxUSE_RADIOBOX      OFF CACHE BOOL "")
set(wxUSE_SCROLLBAR     OFF CACHE BOOL "")
set(wxUSE_SEARCHCTRL    OFF CACHE BOOL "")
set(wxUSE_SLIDER        OFF CACHE BOOL "")
set(wxUSE_SPINBTN       OFF CACHE BOOL "")
set(wxUSE_SPINCTRL      OFF CACHE BOOL "")
set(wxUSE_TIMEPICKCTRL  OFF CACHE BOOL "")
set(wxUSE_TOGGLEBTN     OFF CACHE BOOL "")
set(wxUSE_TREELISTCTRL  OFF CACHE BOOL "")
set(wxUSE_HELP  OFF CACHE BOOL "")
set(wxUSE_WXHTML_HELP  OFF CACHE BOOL "")
set(wxUSE_BITMAPCOMBOBOX  OFF CACHE BOOL "")
set(wxUSE_COMBOCTRL  OFF CACHE BOOL "")
set(wxUSE_ODCOMBOBOX  OFF CACHE BOOL "")

## Flags that don't turn off  properly due to a bug in the wxWidgets 3.2 CMake files
# set(wxUSE_VARIANT OFF CACHE BOOL "")
