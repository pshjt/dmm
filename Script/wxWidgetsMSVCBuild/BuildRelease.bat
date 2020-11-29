@echo off

pushd "%~dp0"

rd "..\..\External\wxWidgets\build\msw\vc_mswu" /s /q
rd "..\..\External\wxWidgets\lib\vc_lib\mswu" /s /q

set clean=true
set build=Release
call _Build.bat

popd
