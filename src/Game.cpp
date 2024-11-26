#include "Game.h"

#include <iostream>

Game::Game(const std::string& config)
{
    init(config);
}

void Game::init(const std::string& path)
{
    // read in the config file
    // use the structs in Game.h

    spawnPlayer();
}

std::shared_ptr<Entity> Game::player()
{
    auto& players = m_entities.getEntities("player");
    return players.front();
}

void Game::run()
{
    // add pause functionality
    
    while (m_running)
    {
        // update entity manager
        m_entities.update();

        sEnemySpawner();
        sMovement();
        sCollision();
        sUserInput();
        sGUI();
        sRender();

        // increment the current frame
        // may need to be moved when pause implemented
        m_currentFrame++;
    }
}

// respawn the player in the middle fo the screen
void Game::spawnPlayer()
{
    // finish adding all properties for he player with the correct values from the config file
    // this stuff is hardcoded example

    // create every entity by calling EntityManager.addEntity(tag)
    // this returns a std::shared_ptr<Entity> so we use auto to save typing
    auto entity = m_entities.addEntity("player");

    // give this entity a transform so it spawns at (200, 200) with velocity (1, 1) and angle 0
    entity->add<CTransform>(Vec2f(200.0f, 200.0f), Vec2f(1.0f, 1.0f), 0.0f);

    // the entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
    entity->add<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

    // add an input component to the player so we can use inputs
    entity->add<CInput>();
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
    // make sure enemy spawned properly with the m_enemyConfig variables, must be spawned within bounds of screen

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // spawn small enemies at the location of the input enemy e

    // when we create the smaller enemy, we have to read the values of the original enemy
    // spawn a number of small ones equal to number of vertices of bigger one
    // set each small enemy to same color as big one
    // each small enemy worth double points
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f& target)
{
    // bullet speed is given as a scalar
}

void Game::sMovement()
{
    // implement all entity movement in this function
    // should read the m_player->CInput component to determine if the player is moving

    // sample movement speed update
    auto& transform = player()->get<CTransform>();
    transform.pos.x += transform.velocity.x; // like this since I haven't implented Vec2 class yet, but would hust use operators
    transform.pos.y += transform.velocity.y;
}

void Game::sLifespan()
{
    // for all entities
    //     if entity has no lifespan component, skip it
    //     if entity has > 0 remaining lifespan, subtract 1
    //     if it has lifespan and it alive
    //         scale alpha channel
    //     if it has lifespan and its time is up
    //         destroy entity
}

void Game::sCollision()
{
    // implement all proper collision bwteen entities
    // use collision radius, not shape radius

    // nested for loops with bullets and enemies
}

void Game::sEnemySpawner()
{
    // do this
}

void Game::sGUI()
{
    // ImGui or something else
}

void Game::sRender()
{
    // change code below to draw all entities

    // sample drawing of player entity we created
    m_window.clear();

    // set the pos of the shape basedon the entity's transofrm->pos
    player()->get<CShape>().circle.setPosition(player()->get<CTransform>().pos);

    // set the rotation of the shape based on the entity's transofrm->angle
    player()->get<CTransform>().angle += 1.0f;
    player()->get<CShape>().circle.setRotation(player()->get<CTransform>().angle);

    // draw entity's sf::CircleShape
    m_window.draw(player()->get<CShape>().circle);

    // draw the ui last if I implement any

    m_window.display();
}

void Game::sUserInput()
{
    // handle user input here
    // only set player's input component vars
    // no logic here
    // movement system does the logic

    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                std::cout << "w pressed\n";
                // set player's input component "up" to true
                break;
            default:
                break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                std::cout << "w released\n";
                // set player's input component "up" to false
                break;
            default:
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "lmb clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
                
                // call spawn bullet
            }
        }
    }
}