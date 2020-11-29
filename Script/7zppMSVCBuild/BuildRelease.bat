@echo off

pushd "%~dp0"

rd "..\..\External\7zip-cpp\build\7zpp.dir\Release" /s /q
del "..\..\External\7zip-cpp\build\Release\7zpp.*" /f

set build=Release
call _Build.bat

popd