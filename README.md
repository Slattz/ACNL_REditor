# ACNL_REditor

This tool is a WIP Rom Editor For Animal Crossing: New Leaf.
* It supports *only* the latest ACNL update, and works with all ACNL games **except** Korean (at the time of writing).

ACNL_REditor is inspired by the amazing [pk3DS](https://github.com/kwsch/pk3DS) project by [kwsch](https://github.com/kwsch).

Compilation: Requires [QT](https://www.qt.io/). (Only QT 5.11.1 has been tested, milage may vary with other versions). 

## Features
* Shop Opening Hours Editor
* Item Price Randomiser
* Disable Region Check & Checksum Check
* Disable Resetti
* Reese/Nooklings/Leila Buy Items For Sell Price
* Shops Buy Native Fruit For Same Price As Foreign Fruit
* Force Weather
* Always Have Confetti In Sky
* Always Have Cherry Blossoms in Sky
* Player Speed Modifier
* Mosquito Never Bites
* Flowers Never Trample
* More I'm Probably Forgetting

## How To Use
1) Acquire the necessary files. These can be dumped from your console via the `ACNL_REditor File Dumper.gm9` script.
  To use the `ACNL_REditor File Dumper.gm9` script:
    1) Make sure you have latest version of GodMode9.
    2) Copy `ACNL_REditor File Dumper.gm9` to `SD:/gm9/scripts`.
    3) Open GodMode9. If using Luma3DS, this can be chainloaded. Upon turning on your console, hold `Start`, then choose GodMode9.
    4) Press the HOME Button, thhen navigate to `Scripts` and click on `ACNL_REditor File Dumper`.
    5) Choose the game you want to dump files from.
    6) Once finished with step 5, turn off your 3DS. Remove the SD Card and copy the `rom` folder from `SD:/ACNL_REditor/GAMETYPE/rom` to somewhere on your PC. 
        * `GAMETYPE` will be the game you dumped the files from.

2) Open ACNL REditor, navigate to the `File` tab and click `Open`. From here, find the `rom` folder you previously copied. Once found, click on it, then press `Select Folder`.
3) Make whatever changes you want.
4) When finished making changes, navigate to the `File` tab and click `Save`. This will create a new `luma` folder in the same directory as `ACNL_REditor.exe`. Copy the `luma` folder to the root of your SD Card, overwriting any files if asked.
5) Upon turning on your console, hold `Select` to open Luma's configuration. Navgate to `Enable game patching`, enable it if it is not already, then press `Start` to save the configuration.
6) You are now finished. Open your game, and enjoy the changes you made :slight_smile: