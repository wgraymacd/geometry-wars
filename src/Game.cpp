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

    // create window
    m_window.create(sf::VideoMode(1280, 720), "A2"); // read window values in from config later
    m_window.setFramerateLimit(60);

    // spawn player
    spawnPlayer();
}

std::shared_ptr<Entity> Game::player() // helper func to get player from entity manager
{
    auto& players = m_entityManager.getEntities("player");
    return players.front();
}

void Game::run()
{
    while (m_running)
    {
        // update entity manager
        m_entityManager.update();

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

void Game::setPaused(bool paused)
{
    // pause functionality
}

// respawn the player in the middle fo the screen
void Game::spawnPlayer()
{
    // finish adding all properties for the player with the correct values from the config file
    // this stuff is hardcoded example

    // create every entity by calling EntityManager.addEntity(tag)
    // this returns a std::shared_ptr<Entity> so we use auto to save typing
    std::shared_ptr<Entity> entity = m_entityManager.addEntity("player");

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

    std::shared_ptr<Entity> entity = m_entityManager.addEntity("enemy");

    entity->add<CTransform>(Vec2f(400.0f, 200.0f), Vec2f(-1.0f, 1.0f), 0.0f);
    entity->add<CShape>(32.0f, 5, sf::Color(10, 10, 10), sf::Color(255, 255, 0), 4.0f);

    m_lastEnemySpawnTime = m_currentFrame;

    std::cout << "enemy created\n";
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

    // player
    CTransform& transform = player()->get<CTransform>();
    transform.velocity = Vec2f(0.0f, 0.0f);
    CInput& input = player()->get<CInput>();
    if (input.left)
    {
        transform.velocity.x = -1.0f;
    }
    if (input.right)
    {
        transform.velocity.x = 1.0f;
    }
    if (input.up)
    {
        transform.velocity.y = -1.0f;
    }
    if (input.down)
    {
        transform.velocity.y = 1.0f;
    }
    transform.pos += transform.velocity;

    // enemies
    for (const std::shared_ptr<Entity>& enemy : m_entityManager.getEntities("enemy"))
    {
        CTransform& transform = enemy->get<CTransform>();
        // if (enemy->get<CShape>().circle.getGlobalBounds().left <= ) // check for out of bounds shapes and flip velocities accordingly
        transform.pos += transform.velocity;
    }

    // bullets
    for (const std::shared_ptr<Entity>& bullet : m_entityManager.getEntities("bullet"))
    {
        CTransform& transform = bullet->get<CTransform>();
        transform.pos += transform.velocity;
    }
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
    std::cout << m_currentFrame << " and " << m_lastEnemySpawnTime << "\n";
    if (m_currentFrame >= m_lastEnemySpawnTime + 60)
    {
        std::cout << "spawning enemy\n";
        spawnEnemy();
    }
}

void Game::sGUI()
{
    // ImGui or something else
}

void Game::sRender()
{
    m_window.clear();

    // set the pos of the shape based on the entity's transform->pos
    // player
    player()->get<CShape>().circle.setPosition(player()->get<CTransform>().pos);
    // enemies
    for (auto& enemy : m_entityManager.getEntities("enemy"))
    {
        enemy->get<CShape>().circle.setPosition(enemy->get<CTransform>().pos);
    }
    // bullets
    for (auto& bullet : m_entityManager.getEntities("bullet"))
    {
        bullet->get<CShape>().circle.setPosition(bullet->get<CTransform>().pos);
    }

    // set the rotation of the shape based on the entity's transform->angle
    // player 
    player()->get<CTransform>().angle += 1.0f;
    player()->get<CShape>().circle.setRotation(player()->get<CTransform>().angle);
    // enemies
    for (auto& enemy : m_entityManager.getEntities("enemy"))
    {
        enemy->get<CTransform>().angle += 1.0f;
        enemy->get<CShape>().circle.setRotation(enemy->get<CTransform>().angle);
    }

    // draw entity's sf::CircleShape
    // player
    m_window.draw(player()->get<CShape>().circle);
    // enemies
    for (auto& enemy : m_entityManager.getEntities("enemy"))
    {
        m_window.draw(enemy->get<CShape>().circle);
    }
    // bullets
    for (auto& bullet : m_entityManager.getEntities("bullet"))
    {
        m_window.draw(bullet->get<CShape>().circle);
    }

    // draw the ui last if I implement any

    // display everything drawn
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
                player()->get<CInput>().up = true; // I think better to keep calling player() on each case since will click this less than 60 times per second, so making variable called input in this function would be more taxing
                break;
            case sf::Keyboard::A:
                std::cout << "a pressed\n";
                player()->get<CInput>().left = true;
                break;
            case sf::Keyboard::S:
                std::cout << "s pressed\n";
                player()->get<CInput>().down = true;
                break;
            case sf::Keyboard::D:
                std::cout << "d pressed\n";
                player()->get<CInput>().right = true;
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
                player()->get<CInput>().up = false;
                break;
            case sf::Keyboard::A:
                std::cout << "a pressed\n";
                player()->get<CInput>().left = false;
                break;
            case sf::Keyboard::S:
                std::cout << "s pressed\n";
                player()->get<CInput>().down = false;
                break;
            case sf::Keyboard::D:
                std::cout << "d pressed\n";
                player()->get<CInput>().right = false;
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