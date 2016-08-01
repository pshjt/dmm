@echo off

rd "..\..\External\7zip-cpp\7zpp\Intermediate\Win32\Unicode Release" /s /q
del "..\..\External\7zip-cpp\7zpp\Lib\Win32\7zpp_u.*" /f

set build=Release
call _Build.bat

pause
