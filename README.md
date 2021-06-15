Instructions for Build:

Pre-Requisites:
Set up your Pi Pico C++ Environment by following the official documentation at: https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf

1. Clone Repo and enter the directory
2. Run "mkdir build"
3. Run "cd build"
4. Run "cmake .."
5. run "make -j4"
6. Copy the resulting sig_gen.uf2 to your Pi Pico


Note:
Generate new Logos from: http://www.majer.ch/lcd/adf_bitmap.php

Credit: SSD1306 library from https://github.com/mbober1/RPi-Pico-SSD1306-library

Image:

![alt text](https://github.com/djh82uk/Pico-SigGen/blob/main/SigGen.jpg)
