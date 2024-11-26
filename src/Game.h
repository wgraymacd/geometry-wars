#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.hpp"

struct PlayerConfig 
{  
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
    float S;
};
struct EnemyConfig 
{
    int SR, CR, OR, OG, OB, VMIN, VMAX, L, SI;
    float SMIN, SMAX;
};
struct BulletConfig 
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
    float S;
};

class Game
{
    sf::RenderWindow m_window;
    EntityManager m_entities;
    sf::Font m_font;
    sf::Text m_text;
    PlayerConfig m_playerConfig;
    EnemyConfig m_EnemyConfig;
    BulletConfig m_BulletConfig;
    sf::Clock m_deltaClock;
    int m_score = 0;
    int m_currentFrame = 0;
    int m_lastEnemySpawnTime = 0;
    bool m_paused = false;
    bool m_running = true;

    void init(const std::string& config); // init game state with config file
    void setPaused(bool paused);

    // systems
    void sMovement();
    void sUserInput();
    void sLifespan();
    void sRender();
    void sGUI();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2f& mousePos);

    std::shared_ptr<Entity> player();

public:

    Game(const std::string& config); // constructor takes in game config

    void run();
};