I am attempting to build a 2D video game from scratch using:
- ECS architecture
- C++ and only the SFML library (will attempt using lower-level libraries later on)


A1 info:

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

use shape.getLocalBounds() on sf::Shape objects to see if shape touches side of screen
- gives local pos (relative to shape.getPosition(), not the window) of the top left of the rectangle and the width and height of the rectangle 
- takes into account any scale, translation, etc. that has been applied to the shape  

also use text.getLocalBounds() for sf::Text objects

