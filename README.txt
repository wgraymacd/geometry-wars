Big-Picture Goal:
=================
I am building a 2D video game from scratch using:
- ECS architecture
- C++ and only the SFML library (will attempt using lower-level libraries later on)
Ultimately, I will build a 3D game engine (or simulator, something like that), focusing mainly on the physics, math, and logic. I aim to master 3D math for use in games, simulators, AR/VR, etc.

This Repo:
==========
This repo is a recreation of Geometry Wars, a small project I took on to learn some things before creating my own game.

Acknowledgements:
-----------------
I referrenced skeleton code for this project from a course assignment written by Dave Churchill (https://www.youtube.com/@DaveChurchill), whose course I am currently following (being offered Fall 2024).

Project Info:
-------------

player:

shape defined in config file
must spawn in center of screen at beginning and after death (make sure only one active player)
player moves by speed read from config file using wasd controls
moves only in bounds of window 
shoots bullet towards pointer when mouse button clicked

enemies:

spawn in random loc every x frames (defined in config file)
must not overlap sides of screen when spawning 
random num of vertices between min and max specified in config
random speed when spawning between min and max specified in config 
random color when spawning
bounce off edges of window
when large enemies collide with player or bullet, they're destroyed and small enemies (# enemies = # vertices) spawn and radially leave the point of death

score:

enemies have score component = num of vertices
when enemy killed, game score increased 
score displayed with font specified by config 

drawing:

all entities given slow rotation
alpha channel should be set to ratio depending on how long the entity has left to live (from 255 when it first spawns to 0 when is destroys)

misc:

'p' pauses game
'esc' closes game

config file:

- Window W H FL FS | width, height, frame limit, fullscreen mode | int, int, int, int
- Font F S R G B | font file name, font size, color values | std::string, int, int (x3)
- Player SR CR S FR FG FB OR OG OB OT V | shape rad, collision rad, speed, fill color (x3), outline color (x3), outline thickness, num vertices | int, int, float, then all ints
- Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI | shape rad, collision rad, min and max speed, outline color (x3), outline thickness, min and max vertices, small lifespan, spawn interval | int, int, float (x2), then all ints
- Bullet SR CR S FR FG FB OR OG OB OT V L | shape rad, collision rad, speed, fill color (x3), outline color (x3), outline thickness, num vertices, lifespan | int, int, float, then all ints
