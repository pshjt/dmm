[![dmm-build Actions Status](https://github.com/pshjt/dmm/workflows/dmm-build/badge.svg)](https://github.com/pshjt/dmm/actions)
[![Github all releases](https://img.shields.io/github/downloads/pshjt/dmm/total.svg)](https://GitHub.com/pshjt/dmm/releases/)
## About
This is the home of the Dark engine Mod Manager (DMM), a fork of [SS2BMM](https://github.com/pshjt/ss2bmm) meant for all Dark engine games, System Shock 2 as well as Thief and Thief II.
More info on its intended use and about mod installation can be obtained from [its ReadMe](https://pshjt.github.io/dmm/), and the official threads at
[SystemShock.org](https://www.systemshock.org/index.php?topic=4790.0 "DMM@SystemShock.org") and [TTLG.com](https://www.ttlg.com/forums/showthread.php?t=151204 "DMM@TTLG.com").

## Requirements
DMM requires Windows Vista SP2 or later to run by default but can be compiled for WinXP given appropriate VC++ settings.

### (Optional) 7-Zip
7-Zip is required to use the archive extraction feature and is bundled with the games in all standard installations (Steam, GOG, SS2Tool).
To provide a custom 7-Zip version,
1) Go to [7-zip.org](https://7-zip.org/) and download the 32-bit version.
2) Copy `7z.dll` to the same folder as `dmm.exe`.

## Build
Clone the repository with submodules:\
`git clone https://github.com/pshjt/dmm.git --recursive`

If you have cloned without this option, use\
`git submodule update --init --recursive`

The project requires `cmake >=3.19` and is meant to be built with the Windows 10 SDK.

The following steps build DMM as a 32-bit application using Visual Studio 2019:
1) While in the root folder, execute\
   `cmake -G "Visual Studio 16 2019" -B "Build" -A Win32`\
   to generate the solution file in the folder `Build`.
2) To build immediately from command line, execute\
   `cmake --build Build --config Release --target "dmm"`.\
   Alternatively, open the generated solution and build the `dmm` project from the IDE.

### (Optional) Get wxFormBuilder
1) Get and install [wxFormBuilder](https://github.com/wxFormBuilder/wxFormBuilder/releases/tag/v4.0.0).
2) Open `wxFormBuilder\DarkModManager.fbp`, do your changes, and use `Generate Code`.

## Contribute
You want to improve the mod manager or otherwise help with development? Great! Contributions are always welcome.
Create your own branch, use a rebase workflow, and open a pull request once you're good to go.

## License
Dark engine Mod Manager is licensed under the MIT License.
The project relies on [wxWidgets](https://www.wxwidgets.org/) and [7z-cpp](https://github.com/getnamo/7zip-cpp). Refer to their websites for respective licensing information.
