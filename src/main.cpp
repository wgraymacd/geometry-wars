#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>

int main(int argc, char* argv[])
{
    // create a new window of size w * h pixels
    // top-left of window is (0, 0) and bottom-right is (w, h)
    // you will have to read these from the config file
    const int wWidth = 1280;
    const int wHeight = 720;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
    window.setFramerateLimit(60);

    // let's make a shape that we will draw to the screen
    float circleRadius = 50;
    int circleSegments = 32;
    float circleSpeedX = 1.0f;
    float circleSpeedY = 0.5f; // screen rendering rounds everything, so you can have something like "half a pixel"
    bool drawCircle = true;
    bool drawText = true;

    // create the sfml circle shape based on our parameters
    sf::CircleShape circle(circleRadius, circleSegments);
    circle.setPosition(10.0f, 10.0f);

    // let's load a font
    sf::Font myFont;

    // attempt to load from file
    if (!myFont.loadFromFile("fonts/Cabal.ttf"))
    {
        // can't find font file
        std::cerr << "could not load font\n";
        exit(-1);
    }

    // set up text object that will be drawn to screen
    sf::Text text("Sample Text", myFont, 24);

    // position the top-left corner of the text so that the text aligns on the bottom
    // text character size is in pixels, so move the text up from the bottom by its height
    text.setPosition(0, wHeight - (float)text.getCharacterSize());

    // main game loop - continues for each frame while window is open
    while (window.isOpen())
    {
        // event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            // this event triggers when the window is closed
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // this event is triggered when a key is pressed
            if (event.type == sf::Event::KeyPressed)
            {
                // print the key that was pressed to the console
                std::cout << "key pressed with code = " << event.key.code << "\n";

                // example when X is pressed
                if (event.key.code == sf::Keyboard::X)
                {
                    // reverse the X direction of the circle on the screen
                    circleSpeedX *= -1.0f;
                }
            }
        }

        // basic animation - move circle each frame if it's still in frame
        circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);

        // basic rendering func calls
        window.clear(); // clear window of anything previously drawn
        if (drawCircle)
        {
            window.draw(circle);
        }
        if (drawText)
        {
            window.draw(text);
        }
        window.display();
    }

    return 0;
}