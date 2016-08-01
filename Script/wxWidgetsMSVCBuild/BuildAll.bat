@echo off

call ClearAll.bat

set clean=false

set build=Debug
call _Build.bat
if not %errorlevel%==0 goto stop

set build=Release
call _Build.bat
if not %errorlevel%==0 goto stop

@echo All builds successful.
@echo:

:stop
pause
