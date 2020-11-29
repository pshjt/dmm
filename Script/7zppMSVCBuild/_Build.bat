@echo off

pushd "%~dp0"

if "%vsPlatform%"=="" (
	set vsPlatform=Community
)
set vcPath=%programfiles(x86)%\Microsoft Visual Studio\2019\%vsPlatform%\MSBuild\Current\Bin\

set startDir=%cd%


:start
cd "..\..\External\7zip-cpp\build"

if %build%==Debug (
	"%vcPath%MSBuild.exe" 7zpp.vcxproj /property:"Configuration=Debug" /property:"Platform=Win32"
	if not %errorlevel%==0 goto error
)
if %build%==Release (
	"%vcPath%MSBuild.exe" 7zpp.vcxproj /property:"Configuration=Release" /property:"Platform=Win32"
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
popd
exit /b %saveErrorLevel%
