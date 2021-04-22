[![dmm-build-release Actions Status](https://github.com/pshjt/dmm/workflows/dmm-build-release/badge.svg)](https://github.com/pshjt/dmm/actions)
## About
This is the home of the Dark mod manager, a fork of [SS2BMM](https://github.com/pshjt/ss2bmm) meant for all Dark engine games, System Shock 2 as well as Thief and Thief II.
More info on its intended use and about mod installation with respect to System Shock 2 can be obtained from [its homepage](https://pshjt.github.io/dmm/) and the 
[official thread](https://www.systemshock.org/index.php?topic=4790.0 "DMM@SystemShock.org") .

## Requirements
DMM requires Windows Vista SP2 or later to run by default but can be compiled for WinXP given appropriate VC++ settings.

### (Optional) 7-Zip
7-Zip is required to use the archive extraction feature and is bundled with the games in all standard installations (Steam, GOG, SS2Tool).
To provide a custom 7-Zip version,
1) Go to [7-zip.org](https://7-zip.org/) and download the 32-bit version.
2) Copy `7z.dll` to the same folder as `dmm.exe`

## Setup
Clone the repository with submodules:\
`git clone https://github.com/pshjt/dmm.git --recursive`

If you have cloned without this option, use\
`git submodule update --init --recursive`

The project was built with Visual Studio 2019 Community and requires `cmake >=3.15`.

### Compile 7zip-cpp
1) Go to `dmm\Script\7zppMSVCBuild`
2) Run `PreBuild.bat`
3) Run `BuildAll.bat`

### Compile wxWidgets
1) Go to `dmm\Script\wxWidgetsMSVCBuild`
2) Run `BuildAll.bat`

### (Optional) Get wxFormBuilder
1) Get and install [wxFormBuilder](https://github.com/wxFormBuilder/wxFormBuilder/releases/tag/v3.9.0)
2) Open `.\wxFormBuilder\DarkModManager.fbp`

## Contribute
You want to improve the mod manager or otherwise help with development? Great! Contributions are always welcome.
Create your own branch, use a rebase workflow, and open a pull request once you're good to go.

## License
Dark Mod Manager is licensed under the MIT License.
The project relies on [wxWidgets](https://www.wxwidgets.org/) and [7z-cpp](https://github.com/getnamo/7zip-cpp). Refer to their websites for respective licensing information.
