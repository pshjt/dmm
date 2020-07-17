@echo off

rd "..\..\External\7zip-cpp\build\7zpp.dir\Debug" /s /q
del "..\..\External\7zip-cpp\build\Debug\7zpp.*" /f
set build=Debug
call _Build.bat
if not %errorlevel%==0 goto stop

rd "..\..\External\7zip-cpp\build\7zpp.dir\Release" /s /q
del "..\..\External\7zip-cpp\build\Release\7zpp.*" /f
set build=Release
call _Build.bat
if not %errorlevel%==0 goto stop

@echo All builds successful.
@echo:

:stop
pause
