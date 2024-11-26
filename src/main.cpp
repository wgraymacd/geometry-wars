#include <iostream>
#include <fstream>

#include <cstdlib>  // for rand()

#include <SFML/Graphics.hpp>

// congif file line type structs
struct WindowConfig
{
    int h, w;
};
struct FontConfig
{
    std::string path;
    int size, r, g, b;
};
struct ShapeConfig
{
    std::string type, name;
    float x, y, vx, vy, w, h, rad;
    int r, g, b;
};

// config loader class
class ConfigLoader 
{
public:
    WindowConfig window;
    FontConfig font;
    std::vector<ShapeConfig> shapes;

    bool loadConfig(const std::string& filepath) 
    {
        std::ifstream file(filepath);
        if (!file.is_open()) 
        {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return false;
        }

        std::string type;

        while (file >> type) 
        {
            if (type == "Window") 
            {
                file >> window.w >> window.h;
            } 
            else if (type == "Font") 
            {
                file >> font.path >> font.size >> font.r >> font.g >> font.b;
            }
            else if (type == "Circle")
            {
                ShapeConfig shape;
                shape.type = type;
                file >> shape.name >> shape.x >> shape.y >> shape.vx >> shape.vy >> shape.r >> shape.g >> shape.b >> shape.rad;
                shapes.push_back(shape);
            }
            else if (type == "Rectangle") 
            {
                ShapeConfig shape;
                shape.type = type;
                file >> shape.name >> shape.x >> shape.y >> shape.vx >> shape.vy >> shape.r >> shape.g >> shape.b >> shape.w >> shape.h;
                shapes.push_back(shape);
            }
            else 
            {
                std::cerr << "Unknown type: " << type << std::endl;
            }
        }
        file.close();
        return true;
    }
};

int main(int argc, char* argv[])
{
    // debugging
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    // load config file data
    ConfigLoader loader;
    loader.loadConfig("../bin/config.txt");

    // create window and set framerate
    sf::RenderWindow window(sf::VideoMode(loader.window.w, loader.window.h), "SFML works!");
    window.setFramerateLimit(60);

    // load the font
    sf::Font font;
    if (!font.loadFromFile(loader.font.path)) // load and check for errors simultaneously
    {
        std::cerr << "Could not load font\n";
        exit(-1);
    }

    // create the sfml shapes with their names based on config parameters in loader object
    // assume only rectangles and circles
    std::vector<std::unique_ptr<sf::Shape>> sfmlShapes; // vector of pointers to shape objects (to have just one vector of shapes instead of one for circles and one for rectangles, sf::Shape is abstract, can't be instantiated)
    std::vector<sf::Text> sfmlTexts;
    for (const auto& shape : loader.shapes)
    {
        if (shape.type == "Rectangle")
        {
            std::unique_ptr<sf::RectangleShape> rectangle = std::make_unique<sf::RectangleShape>(sf::Vector2f(shape.w, shape.h)); // would be a good line to use auto keyword, but I want to be explicit
            rectangle->setFillColor(sf::Color(shape.r, shape.g, shape.b));
            rectangle->setPosition(shape.x, shape.y);
            sfmlShapes.push_back(std::move(rectangle));

            sf::Text text(shape.name, font, loader.font.size);
            text.setFillColor(sf::Color(loader.font.r, loader.font.g, loader.font.b));
            text.setPosition(sf::Vector2f(shape.x, shape.y));
            sfmlTexts.push_back(text);
        }
        else
        {
            std::unique_ptr<sf::CircleShape> circle = std::make_unique<sf::CircleShape>(shape.rad);
            circle->setFillColor(sf::Color(shape.r, shape.g, shape.b));
            circle->setPosition(shape.x, shape.y);
            sfmlShapes.push_back(std::move(circle));

            sf::Text text(shape.name, font, loader.font.size);
            text.setFillColor(sf::Color(loader.font.r, loader.font.g, loader.font.b));
            text.setPosition(sf::Vector2f(shape.x, shape.y));
            sfmlTexts.push_back(text);
        }
    }

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
                std::cout << "Key pressed with code = " << event.key.code << "\n";

                // example when X is pressed
                if (event.key.code == sf::Keyboard::Space)
                {
                    // random testing here
                    int random = rand() % loader.shapes.size();
                    sfmlShapes[random]->setRotation(45);
                    sfmlShapes[random]->setScale(sf::Vector2f(2.0f, 0.5f));
                }
            }
        }

        // basic animation - move shapes and texts each frame
        for (int i = 0; i < sfmlShapes.size(); i++)
        {
            // bounce checks, change velocities if needed
            sf::FloatRect box = sfmlShapes[i]->getGlobalBounds();
            if ((box.left <= 0) || (box.left + box.width >= loader.window.w))
            {
                loader.shapes[i].vx *= -1.0f;
            }
            if ((box.top <= 0) || (box.top + box.height >= loader.window.h))
            {
                loader.shapes[i].vy *= -1.0f;
            }

            // update positions
            sfmlShapes[i]->setPosition(sfmlShapes[i]->getPosition().x + loader.shapes[i].vx, sfmlShapes[i]->getPosition().y + loader.shapes[i].vy);
            sfmlTexts[i].setPosition(sfmlTexts[i].getPosition().x + loader.shapes[i].vx, sfmlTexts[i].getPosition().y + loader.shapes[i].vy);
        }

        // basic rendering func calls
        window.clear();
        for (int i = 0; i < sfmlShapes.size(); i++) 
        {
            window.draw(*sfmlShapes[i]);
            window.draw(sfmlTexts[i]);
        }
        window.display();
    }

    return 0;
}