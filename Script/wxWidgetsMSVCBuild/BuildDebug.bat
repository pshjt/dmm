@echo off

rd "..\..\External\wxWidgets\build\msw\vc_mswud" /s /q
rd "..\..\External\wxWidgets\lib\vc_lib\mswud" /s /q

set clean=true
set build=Debug
call _Build.bat

pause
