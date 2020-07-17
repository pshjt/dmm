@echo off

set vcPath=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\
set vcVersion=16.0
set vcCmakeKey="Visual Studio 16 2019"

set startDir=%cd%
goto start

goto :eof


:start
cd "..\..\External\7zip-cpp\build"
call "%vcPath%vcvarsall.bat" x86
if not %VisualStudioVersion%==%vcVersion% goto error

if %build%==Debug (
	MSBuild.exe 7zpp.vcxproj /property:"Configuration=Debug" /property:"Platform=Win32"
	if not %errorlevel%==0 goto error
)
if %build%==Release (
	MSBuild.exe 7zpp.vcxproj /property:"Configuration=Release" /property:"Platform=Win32"
	if not %errorlevel%==0 goto error
)
if not %errorlevel%==0 goto error

set saveErrorLevel=0
@echo:
@echo %build% build successful.
@echo:
goto cleanup

:error
cd %startDir%
set saveErrorLevel=%errorlevel%
@echo:
@echo %build% build error. Process terminated.
@echo:

:cleanup
cd %startDir%

exit /b %saveErrorLevel%
