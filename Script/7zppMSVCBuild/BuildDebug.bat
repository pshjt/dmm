@echo off

rd "..\..\External\7zip-cpp\7zpp\Intermediate\Win32\Unicode Debug" /s /q
del "..\..\External\7zip-cpp\7zpp\Lib\Win32\7zpp_ud.*" /f

set build=Debug
call _Build.bat

pause
