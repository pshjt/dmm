@echo off
pushd "%~dp0"

if exist Release rd /s /q Release

if "%vsPlatform%"=="" (
	set vsPlatform=Community
)

"%programfiles(x86)%\Microsoft Visual Studio\2019\%vsPlatform%\MSBuild\Current\Bin\MSBuild.exe" /property:"Configuration=Release" /property:"Platform=Win32"

popd
@echo on