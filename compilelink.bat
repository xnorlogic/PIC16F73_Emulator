echo off
cls
echo Get ready!...
g++ -c -o PIC16F73.o PIC16F73.c
g++ -c -o PIC16F73EMU.o PIC16F73EMU.c
g++ -o PicEmulator PIC16F73.o PIC16F73EMU.o
PicEmulator.exe