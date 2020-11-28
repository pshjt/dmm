## About
This is the home of the System Shock 2 Blud Mod Manager, originally developed by bluemess in 2012 (SS2BMM).
More info on its intended use and about mod installation for System Shock 2 in general can be obtained from [SystemShock.org](https://www.systemshock.org "SystemShock.org") and the 
[official thread](https://www.systemshock.org/index.php?topic=4790.0 "SS2BMM@SystemShock.org").

## Requirements
SS2BMM requires Windows Vista SP2 or later to run by default but can be compiled for WinXP SP2 given appropriate compilation settings.
A ready-for-use Visual Studio 2019 solution is provided.

## Setup
Clone repository with submodules:\
`git clone <URL> --recursive`

If you have cloned without this option, use\
`git submodule update --init --recursive`

### Compile 7zip-cpp
1) Go to ss2bmm\Script\7zppMSVCBuild
2) Run `PreBuild.bat`
3) Run `BuildAll.bat`

### Compile wxWidgets
1) Go to ss2bmm\Script\wxWidgetsMSVCBuild
2) `BuildAll.bat`
