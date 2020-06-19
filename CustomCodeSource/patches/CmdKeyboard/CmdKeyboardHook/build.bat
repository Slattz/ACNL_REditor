@echo off

set FLAGS= -Os -Wall -Wno-unused-variable -fno-merge-constants -fno-omit-frame-pointer -mapcs-frame -mthumb-interwork

for %%f in (*.c) do (
    echo Compiling C Patch: %%~nf
    arm-none-eabi-gcc.exe %FLAGS% -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -mfpu=vfp -fshort-wchar -g -c %%~dpnf.c -o %%~dpnf.o
    arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .rodata=%%~dpnf_rodata.bin --dump-section .text=%%~dpnf_text.bin
    rem arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .data=%%~dpnf_data.bin --dump-section .bss=%%~dpnf_bss.bin
    rem arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .rodata.str2.2=%%~dpnf_rodata.str2.2.bin --dump-section abs=%%~dpnf_abs.bin
)

for %%f in (*.cpp) do (
    echo Compiling C++ Patch: %%~nf
    arm-none-eabi-g++.exe %FLAGS% -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -mfpu=vfp -fshort-wchar -g -c %%~dpnf.cpp -o %%~dpnf.o
    arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .rodata=%%~dpnf_rodata.bin --dump-section .text=%%~dpnf_text.bin
    rem arm-none-eabi-objcopy.exe %%~dpnf.o --dump-section .data=%%~dpnf_data.bin --dump-section .bss=%%~dpnf_bss.bin
)

pause