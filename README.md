## Enis Rama - Final Project Pacman

# Introduction 
Pac-Man is a 1980 maze action video game. In the original game the player controls Pac-Man, who must eat all the dots inside an enclosed maze while avoiding four colored ghosts. Eating cherries called "Power Pellets" causes the ghosts to temporarily turn blue, allowing Pac-Man to eat them for bonus points. The objective of the game is to eat all of the dots placed in the maze while avoiding four colored ghosts that pursue Pac-Man. 

In this project I will attempt to re-create this classic game in a desktop window and allow the user to navigate pacman around a maze and collect all the dots.

## C++ IDE and Libraries recommended to use:
    - Visual Studio Code (Windows, Linux, Mac).
    - SDL2 Library 

### Follow this tutorial to setup the workspace: [How to Setup SDL2 and VSCode to Start Programming OpenGL on a Mac](https://youtu.be/Dyz9O7s7B8w)
    - When following the tutorial and creating the Makefile, you need to update the 'LINKER_FLAGS' line. 
      It should look like this 'LINKER_FLAGS = -lsdl2 -lsdl2_image'

## Headers in main.cpp
* iostream
* SDL2/SDL.h
* SDL2/SDL_image.h
* vector
* ctime

## Executing
  - To execute the code, open a terminal in VSCode and type 'make' to build the object file
  - If you followed the setup tutorial correctly, the file should be saved in the build/debug folder
  - In the terminal type './build/debug/play' to run the file

### After following the sertup tutorial and make the necessary changes listed above, the code should be able to run on your machine.
