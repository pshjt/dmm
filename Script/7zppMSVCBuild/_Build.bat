@echo off

set vcPath=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\
set vcVersion=12.0

set startDir=%cd%
goto start

goto :eof


:start
mkdir "..\..\External\7zip-cpp\7zpp\temp"
if not %errorlevel%==0 goto error
copy "..\..\External\7zip-cpp\7zpp\7zpp.v*" "..\..\External\7zip-cpp\7zpp\temp" /y
if not %errorlevel%==0 goto error
copy ".\Config\7zpp.*" "..\..\External\7zip-cpp\7zpp" /y
if not %errorlevel%==0 goto error

cd "..\..\External\7zip-cpp\7zpp"
if not %errorlevel%==0 goto error

call "%vcPath%vcvarsall.bat" x86
if not %VisualStudioVersion%==%vcVersion% goto error

if %build%==Debug (
	MSBuild.exe 7zpp.vcxproj /property:"Configuration=Unicode Debug" /property:"Platform=Win32"
	if not %errorlevel%==0 goto error
)
if %build%==Release (
	MSBuild.exe 7zpp.vcxproj /property:"Configuration=Unicode Release" /property:"Platform=Win32"
	if not %errorlevel%==0 goto error
)
if not %errorlevel%==0 goto error

set saveErrorLevel=0
@echo:
@echo %build% build successful.
@echo:
goto cleanup

:error
set saveErrorLevel=%errorlevel%
@echo:
@echo %build% build error. Process terminated.
@echo:

:cleanup
cd %startDir%

copy "..\..\External\7zip-cpp\7zpp\temp\7zpp.v*" "..\..\External\7zip-cpp\7zpp\" /y
rd "..\..\External\7zip-cpp\7zpp\temp" /s /q

exit /b %saveErrorLevel%
