
#include "RoomScreen.hpp"

#include <cmath>

Entity::Entity() :
health(ZOMBIE_START_HEALTH),
alive(false),
velX(0.0f),
velY(0.0f),
lifetime(1.0f)
{
}

Entity::Entity(Sprite sprite) :
sprite(sprite),
health(ZOMBIE_START_HEALTH),
alive(false),
velX(0.0f),
velY(0.0f),
lifetime(1.0f)
{
}

RoomScreen::RoomScreen() :
entityVector(ENTITY_VECTOR_START_SIZE),
flags(),
playerDiag(PLAYER_WALK_SPEED / std::sqrt(2.0f))
{
    zombieTexture.loadFromFile("res/zombie.png");

    zombieSpriteMap.insert(std::make_pair(0, std::vector<SpriteData>{
        // standing facing South
        SpriteData(sf::IntRect(0, 0, 32, 64), 1.0f)
    }));

    zombieSpriteMap.insert(std::make_pair(1, std::vector<SpriteData>{
        // walking South
        SpriteData(sf::IntRect(0, 64, 32, 64), ZOMBIE_WALK_FRAME_TIME),
        SpriteData(sf::IntRect(32, 64, 32, 64), ZOMBIE_WALK_FRAME_TIME)
    }));

    zombieSpriteMap.insert(std::make_pair(2, std::vector<SpriteData>{
        // standing facing North
        SpriteData(sf::IntRect(64, 0, 32, 64), 1.0f)
    }));

    zombieSpriteMap.insert(std::make_pair(3, std::vector<SpriteData>{
        // walking North
        SpriteData(sf::IntRect(64, 64, 32, 64), ZOMBIE_WALK_FRAME_TIME),
        SpriteData(sf::IntRect(96, 64, 32, 64), ZOMBIE_WALK_FRAME_TIME)
    }));

    zombieSpriteMap.insert(std::make_pair(4, std::vector<SpriteData>{
        // standing facing East
        SpriteData(sf::IntRect(128, 0, 32, 64), 1.0f)
    }));

    zombieSpriteMap.insert(std::make_pair(5, std::vector<SpriteData>{
        // walking East
        SpriteData(sf::IntRect(128, 64, 32, 64), ZOMBIE_WALK_FRAME_TIME),
        SpriteData(sf::IntRect(160, 64, 32, 64), ZOMBIE_WALK_FRAME_TIME)
    }));

    zombieSpriteMap.insert(std::make_pair(6, std::vector<SpriteData>{
        // standing facing West
        SpriteData(sf::IntRect(192, 0, 32, 64), 1.0f)
    }));

    zombieSpriteMap.insert(std::make_pair(7, std::vector<SpriteData>{
        // walking West
        SpriteData(sf::IntRect(192, 64, 32, 64), ZOMBIE_WALK_FRAME_TIME),
        SpriteData(sf::IntRect(224, 64, 32, 64), ZOMBIE_WALK_FRAME_TIME)
    }));

    baseZombieSprite.setTexture(zombieTexture);
    baseZombieSprite.setExternalMap(&zombieSpriteMap);

    playerTexture.loadFromFile("res/player.png");
    player.sprite.setTexture(playerTexture);
    player.health = PLAYER_START_HEALTH;

    // standing facing South
    player.sprite.registerMapping(0, SpriteData(
        sf::IntRect(0, 0, 32, 64), 1.0f
    ));

    // walking South
    player.sprite.registerMapping(1, SpriteData(
        sf::IntRect(0, 64, 32, 64), PLAYER_WALK_FRAME_TIME
    ));
    player.sprite.registerMapping(1, SpriteData(
        sf::IntRect(32, 64, 32, 64), PLAYER_WALK_FRAME_TIME
    ));

    // standing facing North
    player.sprite.registerMapping(2, SpriteData(
        sf::IntRect(64, 0, 32, 64), 1.0f
    ));

    // walking North
    player.sprite.registerMapping(3, SpriteData(
        sf::IntRect(64, 64, 32, 64), PLAYER_WALK_FRAME_TIME
    ));
    player.sprite.registerMapping(3, SpriteData(
        sf::IntRect(96, 64, 32, 64), PLAYER_WALK_FRAME_TIME
    ));

    // standing facing East
    player.sprite.registerMapping(4, SpriteData(
        sf::IntRect(128, 0, 32, 64), 1.0f
    ));

    // walking East
    player.sprite.registerMapping(5, SpriteData(
        sf::IntRect(128, 64, 32, 64), PLAYER_WALK_FRAME_TIME
    ));
    player.sprite.registerMapping(5, SpriteData(
        sf::IntRect(160, 64, 32, 64), PLAYER_WALK_FRAME_TIME
    ));

    // standing facing West
    player.sprite.registerMapping(6, SpriteData(
        sf::IntRect(192, 0, 32, 64), 1.0f
    ));

    // walking West
    player.sprite.registerMapping(7, SpriteData(
        sf::IntRect(192, 64, 32, 64), PLAYER_WALK_FRAME_TIME
    ));
    player.sprite.registerMapping(7, SpriteData(
        sf::IntRect(224, 64, 32, 64), PLAYER_WALK_FRAME_TIME
    ));

    player.sprite.setPosition(800.0f / 2.0f - 16.0f, 450.0f / 2.0f - 32.0f);

    // initialize weapons
    weaponsTexture.loadFromFile("res/weapons.png");

    weaponSMG.sprite.setTexture(weaponsTexture);
    weaponSMG.sprite.registerMapping(0, SpriteData(
        sf::IntRect(0, 0, 32, 32), 1.0f
    ));
    weaponSMG.sprite.setOrigin(0.0f, 16.0f);
    weaponSMGBullet.sprite.setTexture(weaponsTexture);
    weaponSMGBullet.sprite.registerMapping(0, SpriteData(
        sf::IntRect(0, 32, 16, 16), 1.0f
    ));
    weaponSMGBullet.sprite.setOrigin(0.0f, 8.0f);
    weaponSMGBullet.health = WEAPON_SMG_DAMAGE;
    weaponSMGBullet.lifetime = WEAPON_SMG_BULLET_LIFETIME;
    weaponSMGBullet.type = TYPE_SMG_BULLET;

    weaponRifle.sprite.setTexture(weaponsTexture);
    weaponRifle.sprite.registerMapping(0, SpriteData(
        sf::IntRect(32, 0, 48, 32), 1.0f
    ));
    weaponRifle.sprite.setOrigin(0.0f, 16.0f);
    weaponRifleBullet.sprite.setTexture(weaponsTexture);
    weaponRifleBullet.sprite.registerMapping(0, SpriteData(
        sf::IntRect(32, 32, 32, 16), 1.0f
    ));
    weaponRifleBullet.sprite.setOrigin(0.0f, 8.0f);
    weaponRifleBullet.health = WEAPON_RIFLE_DAMAGE;
    weaponRifleBullet.lifetime = WEAPON_RIFLE_BULLET_LIFETIME;
    weaponRifleBullet.type = TYPE_RIFLE_BULLET;

    weaponLaser.sprite.setTexture(weaponsTexture);
    weaponLaser.sprite.registerMapping(0, SpriteData(
        sf::IntRect(80, 0, 32, 32), 1.0f
    ));
    weaponLaser.sprite.setOrigin(0.0f, 16.0f);
    weaponLaserBullet.sprite.setTexture(weaponsTexture);
    weaponLaserBullet.sprite.registerMapping(0, SpriteData(
        sf::IntRect(80, 32, 32, 8), 1.0f
    ));
    weaponLaserBullet.sprite.setOrigin(0.0f, 4.0f);
    weaponLaserBullet.health = WEAPON_LASER_DAMAGE;
    weaponLaserBullet.lifetime = WEAPON_LASER_BULLET_LIFETIME;
    weaponLaserBullet.type = TYPE_LASER_BULLET;

    weaponShotgun.sprite.setTexture(weaponsTexture);
    weaponShotgun.sprite.registerMapping(0, SpriteData(
        sf::IntRect(112, 0, 48, 32), 1.0f
    ));
    weaponShotgun.sprite.setOrigin(0.0f, 16.0f);
    weaponShotgunBullet.sprite.setTexture(weaponsTexture);
    weaponShotgunBullet.sprite.registerMapping(0, SpriteData(
        sf::IntRect(112, 32, 16, 16), 1.0f
    ));
    weaponShotgunBullet.sprite.setOrigin(0.0f, 8.0f);
    weaponShotgunBullet.health = WEAPON_SHOTGUN_DAMAGE;
    weaponShotgunBullet.lifetime = WEAPON_SHOTGUN_BULLET_LIFETIME;
    weaponShotgunBullet.type = TYPE_SHOTGUN_BULLET;
}

RoomScreen::~RoomScreen()
{
}

void RoomScreen::update(float dt)
{
    // player input
    if(flags.test(0) && !flags.test(1))
    {
        // left
        if(flags.test(2) && !flags.test(3))
        {
            // Up
            player.velX = -playerDiag;
            player.velY = -playerDiag;
        }
        else if(!flags.test(2) && flags.test(3))
        {
            // Down
            player.velX = -playerDiag;
            player.velY = playerDiag;
        }
        else
        {
            // only left
            player.velX = -PLAYER_WALK_SPEED;
            player.velY = 0.0f;
        }
        if(player.sprite.getPhase() != 7)
        {
            player.sprite.setPhase(7);
        }
    }
    else if(!flags.test(0) && flags.test(1))
    {
        // right
        if(flags.test(2) && !flags.test(3))
        {
            // Up
            player.velX = playerDiag;
            player.velY = -playerDiag;
        }
        else if(!flags.test(2) && flags.test(3))
        {
            // Down
            player.velX = playerDiag;
            player.velY = playerDiag;
        }
        else
        {
            // only right
            player.velX = PLAYER_WALK_SPEED;
            player.velY = 0.0f;
        }
        if(player.sprite.getPhase() != 5)
        {
            player.sprite.setPhase(5);
        }
    }
    // neither left nor right, just up or down or no movement
    else if(flags.test(2) && !flags.test(3))
    {
        // just Up
        player.velX = 0.0f;
        player.velY = -PLAYER_WALK_SPEED;
        if(player.sprite.getPhase() != 3)
        {
            player.sprite.setPhase(3);
        }
    }
    else if(!flags.test(2) && flags.test(3))
    {
        // just Down
        player.velX = 0.0f;
        player.velY = PLAYER_WALK_SPEED;
        if(player.sprite.getPhase() != 1)
        {
            player.sprite.setPhase(1);
        }
    }
    else
    {
        // no movement at all
        player.velX = 0.0f;
        player.velY = 0.0f;
        switch(player.sprite.getPhase())
        {
        default:
        case 0:
        case 1:
            player.sprite.setPhase(0);
            break;
        case 2:
        case 3:
            player.sprite.setPhase(2);
            break;
        case 4:
        case 5:
            player.sprite.setPhase(4);
            break;
        case 6:
        case 7:
            player.sprite.setPhase(6);
            break;
        }
    }

    // update entities
    for(auto iter = entityVector.begin();
        iter != entityVector.end();
        ++iter)
    {
        if(iter->alive)
        {
            iter->sprite.update(dt);
            iter->sprite.move(
                iter->velX * dt,
                iter->velY * dt);
        }
    }

    player.sprite.update(dt);
    player.sprite.move(
        player.velX * dt,
        player.velY * dt
    );
}

void RoomScreen::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        if(event.key.code == sf::Keyboard::Left ||
            event.key.code == sf::Keyboard::A)
        {
            // move left
            flags.set(0);
        }
        else if(event.key.code == sf::Keyboard::Right ||
            event.key.code == sf::Keyboard::D)
        {
            // move right
            flags.set(1);
        }
        else if(event.key.code == sf::Keyboard::Up ||
            event.key.code == sf::Keyboard::W)
        {
            // move up
            flags.set(2);
        }
        else if(event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::S)
        {
            // move down
            flags.set(3);
        }
    }
    else if(event.type == sf::Event::KeyReleased)
    {
        if(event.key.code == sf::Keyboard::Left ||
            event.key.code == sf::Keyboard::A)
        {
            // release left
            flags.reset(0);
        }
        else if(event.key.code == sf::Keyboard::Right ||
            event.key.code == sf::Keyboard::D)
        {
            // release right
            flags.reset(1);
        }
        else if(event.key.code == sf::Keyboard::Up ||
            event.key.code == sf::Keyboard::W)
        {
            // release up
            flags.reset(2);
        }
        else if(event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::S)
        {
            // release down
            flags.reset(3);
        }
    }
}

void RoomScreen::draw(sf::RenderWindow& window)
{
    for(auto iter = entityVector.begin();
        iter != entityVector.end();
        ++iter)
    {
        if(iter->alive)
        {
            iter->sprite.draw(window);
        }
    }

    player.sprite.draw(window);
}

unsigned int RoomScreen::switchScreen()
{
    return 0;
}

void RoomScreen::createZombie(float x, float y)
{
    unsigned int i = 0;
    while(entityVector.at(i).alive)
    {
        ++i;
        if(i >= entityVector.size())
        {
            entityVector.resize(entityVector.size() * 2);
        }
    }
    entityVector.at(i).alive = true;
    entityVector.at(i).health = ZOMBIE_START_HEALTH;
    baseZombieSprite.setPosition(x, y);
    entityVector.at(i).sprite = baseZombieSprite;
}

void RoomScreen::spawnBullet(char type, float x, float y, float angle)
{
    unsigned int i = 0;
    while(entityVector.at(i).alive)
    {
        ++i;
        if(i >= entityVector.size())
        {
            entityVector.resize(entityVector.size() * 2);
        }
    }
    entityVector.at(i).alive = true;
}

