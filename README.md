# ACNL_REditor [![Don8](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=88BG7AVZ7KW6E&source=url)

This tool is a WIP Rom Editor For Animal Crossing: New Leaf.

* It supports *only* the latest ACNL update (v1.5), and works with all ACNL games **except** Korean.

ACNL_REditor is inspired by the amazing [pk3DS](https://github.com/kwsch/pk3DS) project by [kwsch](https://github.com/kwsch).

Compilation: Requires [QT](https://www.qt.io/). (Only QT 5.13.1 has been tested, milage may vary with other versions).

## Features

* Villagers Never Move Out
* Command Keyboard
* Item Price Changer & Randomiser
* Camera Zoom Out
* Button Remapper
* Shop Opening Hours Editor
* Player Speed Modifier
* Disable Resetti
* Flowers Never Trample
* Pick From All Tours On The Island
* Can Type More Than 3 Numbers On The Island
* Reese/Nooklings/Leila Buy Items For Sell Price
* Always Have Confetti and/or Cherry Blossoms In Sky
* Disable Region, Checksum and Secure Value Checks
* **And More!**

## How To Use

1) Acquire the necessary files. These can be dumped from your console via the [`ACNL_REditor File Dumper.gm9`](https://github.com/Slattz/ACNL_REditor/blob/master/ACNL_REditor%20File%20Dumper.gm9) script.
  To use the `ACNL_REditor File Dumper.gm9` script:
    1) Make sure you have latest version of GodMode9.
    2) Copy `ACNL_REditor File Dumper.gm9` to `SD:/gm9/scripts`.
    3) Open GodMode9. If using Luma3DS, this can be chainloaded. Upon turning on your console, hold `Start`, then choose GodMode9.
    4) Press the HOME Button, thhen navigate to `Scripts` and click on `ACNL_REditor File Dumper`.
    5) Choose the game you want to dump files from.
    6) Once finished with step 5, turn off your 3DS. Remove the SD Card and copy the `rom` folder from `SD:/ACNL_REditor/GAMETYPE/rom` to somewhere on your PC.
        * `GAMETYPE` will be the game you dumped the files from.

2) Open ACNL REditor, navigate to the `File` tab and click `Open`. From here, find the `rom` folder you previously copied. Once found, click on it, then press `Select Folder`.
3) Make whatever changes you want in ACNL_REditor.
4) When finished making changes, navigate to the `File` tab and click `Save`. This will create a new `luma` folder in the same directory as the `ACNL_REditor.exe`. Copy the `luma` folder to the root of your SD Card, overwriting any files, if asked.
5) Upon turning on your console, hold `Select` to open Luma's configuration. Navgate to `Enable game patching`, enable it if it is not already, then press `Start` to save the configuration.
6) You are now finished. Open your game, and enjoy the changes you made :slight_smile:
