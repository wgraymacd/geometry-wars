#include "Game.h"

#include <iostream>
#include <fstream>
#include <random>

Game::Game(const std::string& config)
{
    init(config);
}

void Game::init(const std::string& path)
{
    // read in the config file
    // use the structs in Game.h
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << path << std::endl;
        exit(-1);
    }
    std::string type;
    while (file >> type)
    {
        // create window
        if (type == "Window") 
        { 
            int w, h, fps, fs;
            file >> w >> h >> fps >> fs;
            if (fs)
            {
                m_window.create(sf::VideoMode::getDesktopMode(), "A2", sf::Style::Fullscreen);
            }
            else
            {
                m_window.create(sf::VideoMode(w, h), "A2");
            }
            m_window.setFramerateLimit(fps);
        }
        // handle font and text setup
        else if (type == "Font")
        {
            std::string path;
            int size, r, g, b;
            file >> path >> size >> r >> g >> b;
            if (!m_font.loadFromFile(path))
            {
                std::cerr << "Could not load font: " << path << std::endl;
            }
            m_text.setFont(m_font);
            m_text.setCharacterSize(size);
            m_text.setFillColor(sf::Color(r, g, b));
        }
        // load entity data into config structs
        else if (type == "Player")
        {
            file >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
        }
        else if (type == "Enemy")
        {
            file >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
        }
        else if (type == "Bullet")
        {
            file >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
        }
        else
        {
            std::cerr << "Unknown type in config.txt: " << type << std::endl;
            exit(-1);
        }
    }
    file.close();

    // spawn player
    spawnPlayer();
}

// helper for player retrieval from entity manager
std::shared_ptr<Entity> Game::player()
{
    auto& players = m_entityManager.getEntities("player");
    return players.front();
}

// helper for random number generation
// use rand() instead if generating tons of random numbers, change #includes
int getRandInt(int min, int max)
{
    // create a random device and seed the generator
    static std::random_device rd; 
    static std::mt19937 generator(rd());
    
    // define the range
    std::uniform_int_distribution<int> distribution(min, max);

    // generate the number
    return distribution(generator);
}

float getRandFloat(float min, float max)
{
    // create a random device and seed the generator
    static std::random_device rd; 
    static std::mt19937 generator(rd());
    
    // define the range
    std::uniform_real_distribution<float> distribution(min, max);

    // generate the number
    return distribution(generator);
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

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
    // create every entity by calling EntityManager.addEntity(tag)
    // this returns a std::shared_ptr<Entity> so we use auto to save typing
    std::shared_ptr<Entity> entity = m_entityManager.addEntity("player");

    entity->add<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

    entity->add<CTransform>(Vec2f(m_window.getSize().x / 2, m_window.getSize().y / 2), Vec2f(0.0f, 0.0f), 0.0f);

    entity->add<CInput>();
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
    std::shared_ptr<Entity> entity = m_entityManager.addEntity("enemy");

    entity->add<CShape>(m_enemyConfig.SR, getRandInt(m_enemyConfig.VMIN, m_enemyConfig.VMAX), sf::Color(getRandInt(0, 255), getRandInt(0, 255), getRandInt(0, 255)), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);

    float angle = getRandFloat(0, 2 * M_PI);
    entity->add<CTransform>(Vec2f(getRandInt(m_enemyConfig.CR, m_window.getSize().x - m_enemyConfig.CR), getRandInt(m_enemyConfig.CR, m_window.getSize().y - m_enemyConfig.CR)), Vec2f(cosf(angle), sinf(angle)) * getRandInt(m_enemyConfig.SMIN, m_enemyConfig.SMAX), 0.0f);

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
    if (m_currentFrame >= m_lastEnemySpawnTime + m_enemyConfig.SI)
    {
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