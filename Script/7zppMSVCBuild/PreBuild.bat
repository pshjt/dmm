@echo off

rd "..\..\External\7zip-cpp\build\" /s /q
mkdir "..\..\External\7zip-cpp\build\"

set vcCmakeKey="Visual Studio 16 2019"

set startDir=%cd%
goto start

goto :eof


:start
copy "Config\CMakeLists.txt" "..\..\External\7zip-cpp\CMakeLists.txt" /y
if not %errorlevel%==0 goto error

rd "..\..\External\7zip-cpp\build\" /s /q
mkdir "..\..\External\7zip-cpp\build\"
cd "..\..\External\7zip-cpp\build"
cmake -DCMAKE_CXX_FLAGS_RELEASE="/MT" -G %vcCmakeKey% -A Win32 ../
if not %errorlevel%==0 goto error

set saveErrorLevel=0
@echo:
@echo CMake Pre-Build successful.
@echo:
goto cleanup

:error
set saveErrorLevel=%errorlevel%
@echo:
@echo %build% build error. Process terminated.
@echo:

:cleanup
cd %startDir%

exit /b %saveErrorLevel%
