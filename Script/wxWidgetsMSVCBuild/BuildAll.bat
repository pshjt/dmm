@echo off

pushd "%~dp0"

call ClearAll.bat

set clean=false

set build=Debug
call _Build.bat

set build=Release
call _Build.bat

@echo All builds successful.
@echo:

popd