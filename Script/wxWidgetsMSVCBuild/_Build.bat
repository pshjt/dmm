@echo off

pushd "%~dp0"

if "%vsPlatform%"=="" (
	set vsPlatform=Community
)
set vcPath=%programfiles(x86)%\Microsoft Visual Studio\2019\%vsPlatform%\VC\Auxiliary\Build\

set startDir=%cd%

if %build%==Debug (
	set config=ConfigDebug.vc
)

if %build%==Release (
	set config=ConfigRelease.vc
)

:start
copy "Config\%config%" "..\..\External\wxWidgets\build\msw\config.vc" /y
if not %errorlevel%==0 goto error

copy "Config\setup.h" "..\..\External\wxWidgets\include\wx\msw\setup.h" /y
if not %errorlevel%==0 goto error

cd "..\..\External\wxWidgets\build\msw"
if not %errorlevel%==0 goto error

call "%vcPath%vcvarsall.bat" x86

if %clean%==true nmake.exe -f makefile.vc clean
if not %errorlevel%==0 goto error

nmake.exe -f makefile.vc
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
del "..\..\External\wxWidgets\build\msw\config.vc" /f
del "..\..\External\wxWidgets\include\wx\msw\setup.h" /f

popd

exit /b %saveErrorLevel%
