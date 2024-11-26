I am attempting to build a 2D video game from scratch using:
- ECS architecture
- C++ and only the SFML library (will attempt using lower-level libraries later on)


A1 info:
========

config.txt
----------

Window W H | width, height | int, int

Font F S R G B | font file name, font size, color values | std::string, int, int (x3)

Rectangle N X Y SX SY R G B W H | shape name, initial pos, initial speed, color, size | std::string, float (x2), float (x2), int (x3), float (x2)

Circle N X Y SX SY R G B R


guide
-----

in order to store an arbitrary number of shapes from the config file, must store them in a container (std::vector)

create custom class or struct which stores all the properties of a shape that are not stored within the sf::Shape class itself (like velocity, name, etc.)

use shape.getBlobalBounds() on sf::Shape objects to see if shape touches side of screen
- takes into account any scale, translation, etc. that has been applied to the shape  

also use text.getLocalBounds() for sf::Text objects to see sizes



A2 info:
========

player:
-------

shape defined in config file
must spawn in center of screen at beginning and after death (make sure only one active player)
player moves by speed read from config file using wasd controls
moves only in bounds of window 
shoots bullet towards pointer when mouse button clicked

enemies:
--------

spawn in random loc every x frames (defines in config file)
must not overlap sides of screen when spawning 
random num of vertices between min and max specified in config
random speed when spawning between min and max specified in config 
random color when spawning
bounce off edges of window
when large enemies collide with player or bullet, they're destroyed and small enemies (# enemies = # vertices) spawn and radially leave the point of death

score:
------

enemies have score component = num of vertices
when enemy killed, game score increased 
score displayed with font specified by config 

drawing:
--------

all entities given slow rotation
alpha channel should be set to ratio depending on how long the entity has left to live (from 255 when it first spawns to 0 when is destroys)

misc:
-----

'p' pauses game
'esc' closes game

config file:
------------

Window W H FL FS | frame limit, fullscreen mode | int, int

Font F S R G B

Player SR CR S FR FG FB OR OG OB OT V | shape rad, collision rad, speed, fill color (x3), outline color (x3), outline thickness, num vertices | int, int, float, then all ints

Enemy SR CR SMIN SMAX OR OG OB VMIN VMAX L SI | shape rad, collision rad, min and max speed, outline color (x3), outline thickness, min and max vertices, small lifespan, spawn interval | int, int, float (x2), then all ints

Bullet SR CR S FR FG FB OR OG OB OT V L | shape rad, collision rad, speed, fill color (x3), outline color (x3), outline thickness, num vertices, lifespan | int, int, float, then all ints

order hints:
------------

first implement Vec2
then addEntity and update in EntityManager
then basics of game class like spawnPlayer and sRender, spawnEnemy, spawnBullet
player movement with sUserInput and sMovement
EntityManager update to delete dead enemies
getEntities(tag)
collisions
config file reading
GUI

