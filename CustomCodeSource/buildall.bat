@echo off

set PATCHES=patches
set FLAGS= -O2 -Wall -Wno-unused-variable

for /R %PATCHES% %%f in (/*.c) do (
    echo Compiling C Patch: %%~nf
    arm-none-eabi-gcc.exe %FLAGS% -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -mfpu=vfp -g -c %%~dpnf.c -o %%~dpnf.o
    arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .rodata=%%~dpnf_rodata.bin --dump-section .text=%%~dpnf_text.bin
    rem arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .data=%%~dpnf_data.bin --dump-section .bss=%%~dpnf_bss.bin
)

for /R %PATCHES% %%f in (/*.cpp) do (
    echo Compiling C++ Patch: %%~nf
    arm-none-eabi-g++.exe %FLAGS% -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -mfpu=vfp -g -c %%~dpnf.cpp -o %%~dpnf.o
    arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .rodata=%%~dpnf_rodata.bin --dump-section .text=%%~dpnf_text.bin
    rem arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .data=%%~dpnf_data.bin --dump-section .bss=%%~dpnf_bss.bin
)

echo.
pause