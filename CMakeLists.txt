cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(DarkModManager LANGUAGES CXX)

#-- Globals 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Use static runtime library with MSCV for release and debug.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# Use Unicode.
add_compile_definitions(UNICODE _UNICODE)


#-- Executable: dmm
add_executable(dmm WIN32)
# Add source file and embedded resources.
file(GLOB_RECURSE DMM_SOURCES CONFIGURE_DEPENDS Source/*.cpp Source/*.hpp wxFormBuilder/*.cpp wxFormBuilder/*.h)
list(APPEND DMM_SOURCES Resource/Resource.rc)
target_sources(dmm PRIVATE ${DMM_SOURCES})

target_include_directories(dmm PRIVATE Source Source/Precompiled)

if(WIN32 AND MSVC)
	# Set the project structure in the IDE to follow the file structure on disc.
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DMM_SOURCES} ) 
	# Set dmm as startup project.
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT dmm)
	# Enable DPI awareness.
	set_property(TARGET dmm PROPERTY VS_DPI_AWARE "PerMonitor")
	# Setup a precompiled header.
	target_compile_options(dmm PRIVATE "/YuPrecompiled.hpp")
	set_source_files_properties(Source/Precompiled/Precompiled.cpp PROPERTIES COMPILE_FLAGS /Yc)
endif()


#-- Lib: wxWidgets
message(STATUS "wxWidgets")
include(CMakeLists_wxWidgetsFlags.cmake)
add_compile_definitions(
    "$<$<CONFIG:Release,MinSizeRel>:wxDEBUG_LEVEL=0>"
    "$<$<CONFIG:RelWithDebInfo,Debug>:wxDEBUG_LEVEL=2>"
)
add_subdirectory(External/wxWidgets)
# Add all wxwidgets modules you want to link to the next command.
target_link_libraries(dmm wx::core) 


#-- Lib: 7zip-cpp
message(STATUS "7zip-cpp")
add_subdirectory(External/7zip-cpp)
target_include_directories(dmm PRIVATE External/7zip-cpp/Include)
target_link_libraries(dmm 7zpp)
