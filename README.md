## About
This is the home of the System Shock 2 Blud Mod Manager, originally developed by bluemess in 2012 (SS2BMM).
More info on its intended use and about mod installation for System Shock 2 in general can be obtained from [SystemShock.org](https://www.systemshock.org) and the 
[official thread](https://www.systemshock.org/index.php?topic=4790.0 "SS2BMM@SystemShock.org").

## Requirements
SS2BMM requires Windows Vista SP2 or later to run by default but can be compiled for WinXP given appropriate VCC settings.

### (Optional) 7-Zip
7-Zip is required to use the archive extraction feature and is bundled with the game in all standard installations (Steam, GOG, SS2Tool).
To provide a custom 7-Zip version,
1) Go to [7-zip.de](https://www.7-zip.de/) and download the 32-bit version.
2) Copy `7z.dll` to the same folder as `ss2bmm.exe`

## Setup
Clone the repository with submodules:\
`git clone https://github.com/pshjt/ss2bmm.git --recursive`

If you have cloned without this option, use\
`git submodule update --init --recursive`

### Compile 7zip-cpp
1) Go to ss2bmm\Script\7zppMSVCBuild
2) Run `PreBuild.bat`
3) Run `BuildAll.bat`

### Compile wxWidgets
1) Go to ss2bmm\Script\wxWidgetsMSVCBuild
2) `BuildAll.bat`

### (Optional) Get wxFormBuilder
1) Get and install [wxFormBuilder](https://github.com/wxFormBuilder/wxFormBuilder/releases/tag/v3.9.0)
2) Open `.\wxFormBuilder\SS2BlueModManager.fbp`
