## About

The Dark engine mod manager (DMM) lets you install and organize mods for System Shock 2, Thief, and Thief II in a straightforward manner.

## Requirements

| Game           | Version        |
|:---------------|:---------------|
| System Shock 2 | v2.4 or later  |
| Thief          | v1.19 or later |
| Thief II       | v1.19 or later |

DMM requires Windows Vista SP2 or later to run.

For Linux support, use [Wine](https://www.winehq.org/).

## Installation

DMM is part of [SS2Tool](https://www.systemshock.org/index.php?topic=4141.0) and is installed alongside it.

For a manual installation, download [the latest release](https://github.com/pshjt/dmm/releases/latest/download/dmm.exe) and preferably copy it to your game's main folder.

## How to use

#### Install mods
Launch DMM. If the game is not yet detected, press **Select game folder...** and select your game's main folder.

![Select game folder](assets/images/dmm_1.webp "Select game folder")

Press **Install mod archives...** and select the mod archives (see [Mod format](#mod-format)) to install. You can select multiple files at once using <kbd>Shift</kbd> and <kbd>Ctrl</kbd>. Confirm by clicking **Open**. Mod archives in the main mod folder (`DMM` by default) are installed automatically.

![Select archives to install](assets/images/dmm_archive.webp "Select archives to install")

Activate mods and arrange their priorities as needed via buttons or drag & drop. Mods that are higher on the list override every mod below them.

![Finished installation with example mod list](assets/images/dmm_2.webp "Finished installation with example mod list")

When finished, press **Apply changes** and **Launch game** to play.

If you want further information about a mod, you can press **Lookup (WWW)** to open a Google search or **Open a ReadMe** file if present. If you have no further use of a mod, you can **Delete** it, which removes the mod from both DMM and your hard drive. 

#### Mod profiles
To save your current mod order for later re-use, open the profile manager via **Manage profiles...**.

Press **Save profile...** to save your current profile or **Load file...** to load a previous profile.

![Profile manager](assets/images/dmm_profile1.webp "Profile manager")

The result window shows a preview of your loaded profile. Mods which were deleted in the meantime cannot be loaded and are marked as such.

![Profile preview](assets/images/dmm_profile2.webp "Profile preview")

Press **Use** to apply the loaded profile, or simply close the window to discard your choice.

#### Shortcuts

| Function          | Key                                |
|:------------------|:-----------------------------------|
| Increase priority | <kbd>Alt</kbd> + <kbd>&uarr;</kbd> |
| Decrease priority | <kbd>Alt</kbd> + <kbd>&darr;</kbd> |
| (De-)Activate     | <kbd>&crarr;</kbd>                 |
| Pause/Resume      | <kbd>Space</kbd>                   |
| Delete            | <kbd>Del</kbd>                     |
| Apply changes     | <kbd>Ctrl</kbd> + <kbd>s</kbd>      |

## Configuration

Configuration values are stored in a file `dmm.cfg` in the same folder as DMM. The most important values are detailed here.

| Key                 | Value (default)                | Function                                                       |
|:--------------------|:-------------------------------|:---------------------------------------------------------------|
| game.modPathSuffix  | `.\patch_ext`                  | Path appended to `mod_path` in `cam_mod.ini`                   |
| game.modPathPrefix  |                                | Path prepended to `mod_path` in `cam_mod.ini`                  |
| game.baseMoviePath  | `.\cutscenes+.\Data\cutscenes` | Path appended to `movie_path` in game installation config      |
| game.modsFolder     | `DMM`                          | Main mod folder. Installed mods are automatically copied to it |
| game.archivesFolder | `DMMArchives`                  | Folder that automatically installed mod archives are copied to |


## Mod format
A mod may come in two formats - archived, or as a folder.

In archive form, e.g. as a `7z` or `zip` file, all main game files (`*.mis`, `*.dml`, `*.gam`, ... ) and folders (`\obj`, `\mesh`, ...) should be placed at the root of the archive.

In folder form, those same files should be collected in one single folder and placed in the DMM folder.