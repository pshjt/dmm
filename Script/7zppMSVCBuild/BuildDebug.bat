@echo off

rd "..\..\External\7zip-cpp\build\7zpp.dir\Debug" /s /q
del "..\..\External\7zip-cpp\build\Debug\7zpp.*" /f

set build=Debug
call _Build.bat

pause
