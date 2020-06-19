@echo off
cd "./Text2Item"
call "build.bat"
cd "../"
echo.

cd "./ChangeTime"
call "build.bat"
cd "../"
echo.

cd "./CmdKeyboardHook"
call "build.bat"
cd "../"
echo.

pause