
#include "RoomScreen.hpp"

#include <cmath>

#include <iostream>

#include <GDT/CollisionDetection.hpp>

#include "PathFinding.hpp"

Entity::Entity() :
sprite(),
health(ZOMBIE_START_HEALTH),
alive(false),
velX(0.0f),
velY(0.0f),
lifetime(1.0f),
pathMap()
{
}

Entity::Entity(Sprite sprite) :
sprite(sprite),
health(ZOMBIE_START_HEALTH),
alive(false),
velX(0.0f),
velY(0.0f),
lifetime(1.0f),
pathMap()
{
}

RoomScreen::RoomScreen() :
baseZombieSprite(),
zombieSpriteMap(),
entityVector(ENTITY_VECTOR_START_SIZE),
flags(),
playerDiag(PLAYER_WALK_SPEED / std::sqrt(2.0f)),
gen(std::random_device()()),
zeroToOneDist(0.0f, 1.0f),
currentWeapon(TYPE_SHOTGUN)
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
    player.lifetime = 0.0f;
    player.alive = true;

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
    weaponSMG.lifetime = 0.0f;
    weaponSMGBullet.sprite.setTexture(weaponsTexture);
    weaponSMGBullet.sprite.registerMapping(0, SpriteData(
        sf::IntRect(0, 32, 16, 16), 1.0f
    ));
    weaponSMGBullet.sprite.setOrigin(0.0f, 8.0f);
    weaponSMGBullet.health = WEAPON_SMG_DAMAGE;
    weaponSMGBullet.lifetime = WEAPON_SMG_BULLET_LIFETIME;
    weaponSMGBullet.type = TYPE_SMG_BULLET;
    weaponSMGBullet.alive = true;

    weaponRifle.sprite.setTexture(weaponsTexture);
    weaponRifle.sprite.registerMapping(0, SpriteData(
        sf::IntRect(32, 0, 48, 32), 1.0f
    ));
    weaponRifle.sprite.setOrigin(0.0f, 16.0f);
    weaponRifle.lifetime = 0.0f;
    weaponRifleBullet.sprite.setTexture(weaponsTexture);
    weaponRifleBullet.sprite.registerMapping(0, SpriteData(
        sf::IntRect(32, 32, 32, 16), 1.0f
    ));
    weaponRifleBullet.sprite.setOrigin(0.0f, 8.0f);
    weaponRifleBullet.health = WEAPON_RIFLE_DAMAGE;
    weaponRifleBullet.lifetime = WEAPON_RIFLE_BULLET_LIFETIME;
    weaponRifleBullet.type = TYPE_RIFLE_BULLET;
    weaponRifleBullet.alive = true;

    weaponLaser.sprite.setTexture(weaponsTexture);
    weaponLaser.sprite.registerMapping(0, SpriteData(
        sf::IntRect(80, 0, 32, 32), 1.0f
    ));
    weaponLaser.sprite.setOrigin(0.0f, 16.0f);
    weaponLaser.lifetime = 0.0f;
    weaponLaserBullet.sprite.setTexture(weaponsTexture);
    weaponLaserBullet.sprite.registerMapping(0, SpriteData(
        sf::IntRect(80, 32, 32, 8), 1.0f
    ));
    weaponLaserBullet.sprite.setOrigin(0.0f, 4.0f);
    weaponLaserBullet.health = WEAPON_LASER_DAMAGE;
    weaponLaserBullet.lifetime = WEAPON_LASER_BULLET_LIFETIME;
    weaponLaserBullet.type = TYPE_LASER_BULLET;
    weaponLaserBullet.alive = true;

    weaponShotgun.sprite.setTexture(weaponsTexture);
    weaponShotgun.sprite.registerMapping(0, SpriteData(
        sf::IntRect(112, 0, 48, 32), 1.0f
    ));
    weaponShotgun.sprite.setOrigin(0.0f, 16.0f);
    weaponShotgun.lifetime = 0.0f;
    weaponShotgunBullet.sprite.setTexture(weaponsTexture);
    weaponShotgunBullet.sprite.registerMapping(0, SpriteData(
        sf::IntRect(112, 32, 16, 16), 1.0f
    ));
    weaponShotgunBullet.sprite.setOrigin(0.0f, 8.0f);
    weaponShotgunBullet.health = WEAPON_SHOTGUN_DAMAGE;
    weaponShotgunBullet.lifetime = WEAPON_SHOTGUN_BULLET_LIFETIME;
    weaponShotgunBullet.type = TYPE_SHOTGUN_BULLET;
    weaponShotgunBullet.alive = true;

    // initialize offsets of player weapons
    playerWeaponOffsets.insert(std::make_pair(0, sf::Vector2f(7.0f, 28.0f)));
    playerWeaponOffsets.insert(std::make_pair(1, sf::Vector2f(7.0f, 28.0f)));

    playerWeaponOffsets.insert(std::make_pair(2, sf::Vector2f(25.0f, 27.0f)));
    playerWeaponOffsets.insert(std::make_pair(3, sf::Vector2f(25.0f, 27.0f)));

    playerWeaponOffsets.insert(std::make_pair(4, sf::Vector2f(28.0f, 29.0f)));
    playerWeaponOffsets.insert(std::make_pair(5, sf::Vector2f(28.0f, 29.0f)));

    playerWeaponOffsets.insert(std::make_pair(6, sf::Vector2f(5.0f, 29.0f)));
    playerWeaponOffsets.insert(std::make_pair(7, sf::Vector2f(5.0f, 29.0f)));

    // walls
    walls[0].left = 150;
    walls[0].top = 125;
    walls[0].width = 25;
    walls[0].height = 200;

    walls[1].left = 175;
    walls[1].top = 125;
    walls[1].width = 125;
    walls[1].height = 25;

    walls[2].left = 175;
    walls[2].top = 300;
    walls[2].width = 125;
    walls[2].height = 25;

    walls[3].left = 625;
    walls[3].top = 125;
    walls[3].width = 25;
    walls[3].height = 200;

    walls[4].left = 500;
    walls[4].top = 125;
    walls[4].width = 125;
    walls[4].height = 25;

    walls[5].left = 500;
    walls[5].top = 300;
    walls[5].width = 125;
    walls[5].height = 25;

    wallsF[0][0] = 150;
    wallsF[0][1] = 125;
    wallsF[0][2] = 150 + 25;
    wallsF[0][3] = 125;
    wallsF[0][4] = 150 + 25;
    wallsF[0][5] = 125 + 200;
    wallsF[0][6] = 150;
    wallsF[0][7] = 125 + 200;

    wallsF[1][0] = 175;
    wallsF[1][1] = 125;
    wallsF[1][2] = 175 + 125;
    wallsF[1][3] = 125;
    wallsF[1][4] = 175 + 125;
    wallsF[1][5] = 125 + 25;
    wallsF[1][6] = 175;
    wallsF[1][7] = 125 + 25;

    wallsF[2][0] = 175;
    wallsF[2][1] = 300;
    wallsF[2][2] = 175 + 125;
    wallsF[2][3] = 300;
    wallsF[2][4] = 175 + 125;
    wallsF[2][5] = 300 + 25;
    wallsF[2][6] = 175;
    wallsF[2][7] = 300 + 25;

    wallsF[3][0] = 625;
    wallsF[3][1] = 125;
    wallsF[3][2] = 625 + 25;
    wallsF[3][3] = 125;
    wallsF[3][4] = 625 + 25;
    wallsF[3][5] = 125 + 200;
    wallsF[3][6] = 625;
    wallsF[3][7] = 125 + 200;

    wallsF[4][0] = 500;
    wallsF[4][1] = 125;
    wallsF[4][2] = 500 + 125;
    wallsF[4][3] = 125;
    wallsF[4][4] = 500 + 125;
    wallsF[4][5] = 125 + 25;
    wallsF[4][6] = 500;
    wallsF[4][7] = 125 + 25;

    wallsF[5][0] = 500;
    wallsF[5][1] = 300;
    wallsF[5][2] = 500 + 125;
    wallsF[5][3] = 300;
    wallsF[5][4] = 500 + 125;
    wallsF[5][5] = 300 + 25;
    wallsF[5][6] = 500;
    wallsF[5][7] = 300 + 25;

    wall.setFillColor(sf::Color(127, 50, 0));

    // other initializations
    createZombie(50, 50);

    zombieFightMusic.openFromFile("res/LD37_ZombieFight.ogg");
    zombieFightMusic.setLoop(true);
    zombieFightMusic.play();
}

RoomScreen::~RoomScreen()
{
}

void RoomScreen::update(float dt, sf::RenderWindow& window)
{
#ifndef NDEBUG
//    std::cout << "Start update...";
#endif
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

    // continue firing
    if(flags.test(4) && (currentWeapon == TYPE_SMG || currentWeapon == TYPE_LASER))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f pos = player.sprite.getPosition() + playerWeaponOffsets.at(player.sprite.getPhase());
        fireWeapon(currentWeapon,
            std::atan2(-((float)mousePos.y - pos.y),
                (float)mousePos.x - pos.x),
            pos);
    }

    // update reload times
    weaponSMG.lifetime -= dt;
    if(weaponSMG.lifetime < 0.0f)
    {
        weaponSMG.lifetime = 0.0f;
    }
    weaponRifle.lifetime -= dt;
    if(weaponRifle.lifetime < 0.0f)
    {
        weaponRifle.lifetime = 0.0f;
    }
    weaponLaser.lifetime -= dt;
    if(weaponLaser.lifetime < 0.0f)
    {
        weaponLaser.lifetime = 0.0f;
    }
    weaponShotgun.lifetime -= dt;
    if(weaponShotgun.lifetime < 0.0f)
    {
        weaponShotgun.lifetime = 0.0f;
    }

    // update entities
    for(auto iter = entityVector.begin();
        iter != entityVector.end();
        ++iter)
    {
        if(iter->alive)
        {
            if(iter->type >= TYPE_SMG_BULLET &&
                iter->type <= TYPE_SHOTGUN_BULLET)
            {
                iter->lifetime -= dt;
                if(iter->lifetime <= 0.0f)
                {
                    iter->alive = false;
                    continue;
                }
                for(auto iter1 = entityVector.begin();
                    iter1 != entityVector.end();
                    ++iter1)
                {
                    if(iter1->alive && iter1->type == TYPE_ZOMBIE)
                    { // collision detection between bullet and zombie
                        float zombieBox[8] = {
                            iter1->sprite.getPosition().x,
                            iter1->sprite.getPosition().y,
                            iter1->sprite.getPosition().x + 32.0f,
                            iter1->sprite.getPosition().y,
                            iter1->sprite.getPosition().x + 32.0f,
                            iter1->sprite.getPosition().y + 64.0f,
                            iter1->sprite.getPosition().x,
                            iter1->sprite.getPosition().y + 64.0f
                        };
                        auto checkCollision = [&iter, &iter1] (float* array, sf::Vector2f& point) -> bool {
                            if(GDT::isWithinPolygon(array, 8, point.x, point.y))
                            {
                                iter1->health -= iter->health;
                                if(iter1->health <= 0)
                                {
                                    iter1->alive = false;
                                }
                                iter->alive = false;
                                iter1->lifetime = ZOMBIE_HIT_FADE_TIME;
                                return true;
                            }
                            return false;
                        };
                        sf::Vector2f point = iter->sprite.getTransform() * sf::Vector2f();
                        if(checkCollision(zombieBox, point))
                        {
                            break;
                        }
                        else if(iter->type == TYPE_SMG_BULLET || iter->type == TYPE_SHOTGUN_BULLET)
                        {
                            point = iter->sprite.getTransform() * sf::Vector2f(16.0f, 0.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                            point = iter->sprite.getTransform() * sf::Vector2f(16.0f, 16.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                            point = iter->sprite.getTransform() * sf::Vector2f(0.0f, 16.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                        }
                        else if(iter->type == TYPE_RIFLE_BULLET)
                        {
                            point = iter->sprite.getTransform() * sf::Vector2f(32.0f, 0.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                            point = iter->sprite.getTransform() * sf::Vector2f(32.0f, 16.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                            point = iter->sprite.getTransform() * sf::Vector2f(0.0f, 16.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                        }
                        else// if(iter->type == TYPE_LASER_BULLET)
                        {
                            point = iter->sprite.getTransform() * sf::Vector2f(32.0f, 0.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                            point = iter->sprite.getTransform() * sf::Vector2f(32.0f, 8.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                            point = iter->sprite.getTransform() * sf::Vector2f(0.0f, 8.0f);
                            if(checkCollision(zombieBox, point))
                            {
                                break;
                            }
                        }
                    }
                }
            } // all bullet types
            else if(iter->type == TYPE_ZOMBIE)
            {
                if(iter->destination == iter->currentDestination ||
                    iter->pathMap.empty())
                {  // get next destination
                    int32_t zombieX = iter->sprite.getPosition().x / PATH_FINDING_BLOCK_SIZE;
                    int32_t zombieY = iter->sprite.getPosition().y / PATH_FINDING_BLOCK_SIZE;
                    int32_t playerX = player.sprite.getPosition().x / PATH_FINDING_BLOCK_SIZE;
                    int32_t playerY = player.sprite.getPosition().y / PATH_FINDING_BLOCK_SIZE;

                    auto pathMap = shortestPath(zombieX, zombieY, playerX, playerY,
                        [this] (int32_t x, int32_t y) -> bool {
                            if(x < 0 || y < 0 ||
                                x > 800.0f / PATH_FINDING_BLOCK_SIZE || y > 450.0f / PATH_FINDING_BLOCK_SIZE
                                )
                            {
                                return true;
                            }
                            for(unsigned int i = 0; i < 6; ++i)
                            {
                                if(GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_SIZE,
                                        y * PATH_FINDING_BLOCK_SIZE) ||
                                    GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_SIZE + PATH_FINDING_BLOCK_SIZE,
                                        y * PATH_FINDING_BLOCK_SIZE) ||
                                    GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_SIZE + PATH_FINDING_BLOCK_SIZE,
                                        y * PATH_FINDING_BLOCK_SIZE + PATH_FINDING_BLOCK_SIZE) ||
                                    GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_SIZE,
                                        y * PATH_FINDING_BLOCK_SIZE + PATH_FINDING_BLOCK_SIZE) ||
                                    GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_SIZE + PATH_FINDING_BLOCK_SIZE / 2.0f,
                                        y * PATH_FINDING_BLOCK_SIZE + PATH_FINDING_BLOCK_SIZE / 2.0f))
                                {
                                    return true;
                                }
                            }
                            return false;
                        }
                    );
                    iter->pathMap = invertPathMap(pathMap, playerX, playerY);
                    uint64_t zombieXY;
                    ((int32_t*) &zombieXY)[0] = zombieX;
                    ((int32_t*) &zombieXY)[1] = zombieY;
                    if(iter->pathMap.find(zombieXY) != iter->pathMap.end())
                    {
                        iter->currentDestination = iter->pathMap.at(zombieXY);
                    }
                    if(iter->pathMap.find(iter->currentDestination) != iter->pathMap.end())
                    {
                        iter->currentDestination = iter->pathMap.at(iter->currentDestination);
                    }
                    ((int32_t*) &iter->destination)[0] = playerX;
                    ((int32_t*) &iter->destination)[1] = playerY;
#ifndef NDEBUG
//                    std::cout << "New pathmap ";
#endif
                }

                // check zombie to path finding block
                float zombieBox[8] = {
                    iter->sprite.getPosition().x,
                    iter->sprite.getPosition().y,
                    iter->sprite.getPosition().x + 32.0f,
                    iter->sprite.getPosition().y,
                    iter->sprite.getPosition().x + 32.0f,
                    iter->sprite.getPosition().y + 64.0f,
                    iter->sprite.getPosition().x,
                    iter->sprite.getPosition().y + 64.0f
                };
                bool colliding = GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * 25.0f,
                        ((int32_t*)&iter->currentDestination)[1] * 25.0f) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * 25.0f + 25.0f,
                        ((int32_t*)&iter->currentDestination)[1] * 25.0f) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * 25.0f + 25.0f,
                        ((int32_t*)&iter->currentDestination)[1] * 25.0f + 25.0f) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * 25.0f,
                        ((int32_t*)&iter->currentDestination)[1] * 25.0f + 25.0f);
                while (colliding)
                {
                    iter->currentDestination = iter->pathMap.at(iter->currentDestination);
                    colliding = GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * 25.0f,
                        ((int32_t*)&iter->currentDestination)[1] * 25.0f) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * 25.0f + 25.0f,
                        ((int32_t*)&iter->currentDestination)[1] * 25.0f) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * 25.0f + 25.0f,
                        ((int32_t*)&iter->currentDestination)[1] * 25.0f + 25.0f) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * 25.0f,
                        ((int32_t*)&iter->currentDestination)[1] * 25.0f + 25.0f);
                    if(iter->destination == iter->currentDestination)
                    {
                        break;
                    }
                }

                // move
                float magnitude;
                iter->velX = (((int32_t*) &iter->currentDestination)[0] * PATH_FINDING_BLOCK_SIZE
                    + PATH_FINDING_BLOCK_SIZE / 2.0f) - iter->sprite.getPosition().x;
                iter->velY = (((int32_t*) &iter->currentDestination)[1] * PATH_FINDING_BLOCK_SIZE
                    + PATH_FINDING_BLOCK_SIZE / 2.0f) - iter->sprite.getPosition().y;

                magnitude = std::sqrt(iter->velX * iter->velX + iter->velY * iter->velY);

                iter->velX = iter->velX / magnitude * ZOMBIE_WALK_SPEED;
                iter->velY = iter->velY / magnitude * ZOMBIE_WALK_SPEED;

                if(iter->velX > ZOMBIE_WALK_SPEED / 3.0f)
                {
                    iter->sprite.setPhase(5);
                }
                else if(iter->velX < -ZOMBIE_WALK_SPEED / 3.0f)
                {
                    iter->sprite.setPhase(7);
                }
                else if(iter->velY > ZOMBIE_WALK_SPEED / 3.0f)
                {
                    iter->sprite.setPhase(1);
                }
                else if(iter->velY < -ZOMBIE_WALK_SPEED / 3.0f)
                {
                    iter->sprite.setPhase(3);
                }

                unsigned char zombieHit = 255 * (1.0f - (iter->lifetime / ZOMBIE_HIT_FADE_TIME));
                iter->sprite.setColor(sf::Color(255, zombieHit, zombieHit));

                iter->lifetime -= dt;
                if(iter->lifetime < 0.0f)
                {
                    iter->lifetime = 0.0f;
                }

                // check collision with player
                if(player.lifetime == 0.0f)
                {
                    float playerBox[8] = {
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y + 64.0f,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y + 64.0f
                    };
                    if(GDT::isWithinPolygon(playerBox, 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y) ||
                        GDT::isWithinPolygon(playerBox, 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y) ||
                        GDT::isWithinPolygon(playerBox, 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y + 64.0f) ||
                        GDT::isWithinPolygon(playerBox, 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y + 64.0f))
                    {
                        player.health -= ZOMBIE_DAMAGE;
                        if(player.health < 0)
                        {
                            player.alive = false;
                        }
                        player.lifetime = PLAYER_HIT_FADE_TIME;
                    }
                }
#ifndef NDEBUG
//                std::cout << "Zombie destination: " << ((int32_t*)&iter->destination)[0] <<
//                    ", " << ((int32_t*)&iter->destination)[1] << std::endl;
#endif
            } // TYPE_ZOMBIE


            // movement checks
            iter->sprite.update(dt);
            if(iter->type == TYPE_ZOMBIE)
            {
                bool hit = false;
                iter->sprite.move(
                    iter->velX * dt,
                    0);
                for(unsigned int i = 0; i < 6; ++i)
                {
                    if(GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y + 64.0f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y + 64.0f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y + 21.3f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y + 21.3f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y + 42.7f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y + 42.7f))
                    {
                        hit = true;
                        break;
                    }
                }
                if(hit)
                {
                    iter->sprite.move(
                        -iter->velX * dt / 1.5f,
                        0
                    );
                    hit = false;
                }

                iter->sprite.move(
                    0,
                    iter->velY * dt);
                for(unsigned int i = 0; i < 6; ++i)
                {
                    if(GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y + 64.0f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y + 64.0f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y + 21.3f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y + 21.3f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x,
                            iter->sprite.getPosition().y + 42.7f) ||
                        GDT::isWithinPolygon(wallsF[i], 8,
                            iter->sprite.getPosition().x + 32.0f,
                            iter->sprite.getPosition().y + 42.7f))
                    {
                        hit = true;
                        break;
                    }
                }
                if(hit)
                {
                    iter->sprite.move(
                        0,
                        -iter->velY * dt / 1.5f
                    );
                }
            }
            else if(iter->type >= TYPE_SMG_BULLET &&
                iter->type <= TYPE_SHOTGUN_BULLET)
            {
                iter->sprite.move(
                    iter->velX * dt,
                    iter->velY * dt
                );
                if(iter->type == TYPE_SMG_BULLET || iter->type == TYPE_SHOTGUN_BULLET)
                {
                    sf::Vector2f pos = iter->sprite.getTransform() * sf::Vector2f(0, 0);
                    sf::Vector2f pos2 = iter->sprite.getTransform() * sf::Vector2f(16.0f, 16.0f);
                    for(unsigned int i = 0; i < 6; ++i)
                    {
                        if(GDT::isWithinPolygon(wallsF[i], 8,
                                pos.x,
                                pos.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos2.x,
                                pos.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos2.x,
                                pos2.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos.x,
                                pos2.y))
                        {
                            iter->alive = false;
                            break;
                        }
                    }
                }
                else if(iter->type == TYPE_RIFLE_BULLET)
                {
                    sf::Vector2f pos = iter->sprite.getTransform() * sf::Vector2f(0, 0);
                    sf::Vector2f pos2 = iter->sprite.getTransform() * sf::Vector2f(32.0f, 16.0f);
                    for(unsigned int i = 0; i < 6; ++i)
                    {
                        if(GDT::isWithinPolygon(wallsF[i], 8,
                                pos.x,
                                pos.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos2.x,
                                pos.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos2.x,
                                pos2.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos.x,
                                pos2.y))
                        {
                            iter->alive = false;
                            break;
                        }
                    }
                }
                else if(iter->type == TYPE_LASER_BULLET)
                {
                    sf::Vector2f pos = iter->sprite.getTransform() * sf::Vector2f(0, 0);
                    sf::Vector2f pos2 = iter->sprite.getTransform() * sf::Vector2f(32.0f, 8.0f);
                    for(unsigned int i = 0; i < 6; ++i)
                    {
                        if(GDT::isWithinPolygon(wallsF[i], 8,
                                pos.x,
                                pos.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos2.x,
                                pos.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos2.x,
                                pos2.y) ||
                            GDT::isWithinPolygon(wallsF[i], 8,
                                pos.x,
                                pos2.y))
                        {
                            iter->alive = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    player.sprite.update(dt);
    if(player.alive)
    {
        float box[8];
        bool hit = false;

        player.sprite.move(
            player.velX * dt,
            0
        );
        if(player.sprite.getPosition().x < 0 ||
            player.sprite.getPosition().x + 32.0f > 800.0f)
        {
            hit = true;
        }
        else
        {
            for(unsigned int i = 0; i < 6; ++i)
            {
                box[0] = walls[i].left;
                box[1] = walls[i].top;

                box[2] = walls[i].left + walls[i].width;
                box[3] = walls[i].top;

                box[4] = walls[i].left + walls[i].width;
                box[5] = walls[i].top + walls[i].height;

                box[6] = walls[i].left;
                box[7] = walls[i].top + walls[i].height;

                if(GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y + 64.0f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y + 64.0f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y + 21.3f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y + 21.3f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y + 42.7f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y + 42.7f))
                {
                    hit = true;
                    break;
                }
            }
        }
        if(hit)
        {
            player.sprite.move(
                -player.velX * dt,
                0
            );
            hit = false;
        }

        player.sprite.move(
            0,
            player.velY * dt
        );
        if(player.sprite.getPosition().y < 0 ||
            player.sprite.getPosition().y + 64.0f > 450.0f)
        {
            hit = true;
        }
        else
        {
            for(unsigned int i = 0; i < 6; ++i)
            {
                box[0] = walls[i].left;
                box[1] = walls[i].top;

                box[2] = walls[i].left + walls[i].width;
                box[3] = walls[i].top;

                box[4] = walls[i].left + walls[i].width;
                box[5] = walls[i].top + walls[i].height;

                box[6] = walls[i].left;
                box[7] = walls[i].top + walls[i].height;

                if(GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y + 64.0f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y + 64.0f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y + 21.3f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y + 21.3f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x,
                        player.sprite.getPosition().y + 42.7f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 32.0f,
                        player.sprite.getPosition().y + 42.7f))
                {
                    hit = true;
                    break;
                }
            }
        }
        if(hit)
        {
            player.sprite.move(
                0,
                -player.velY * dt
            );
        }
    }
    player.lifetime -= dt;
    if(player.lifetime < 0.0f)
    {
        player.lifetime = 0.0f;
    }
    unsigned char playerHit = 255 * (1.0f - player.lifetime / PLAYER_HIT_FADE_TIME);
    player.sprite.setColor(sf::Color(255, playerHit, playerHit));

#ifndef NDEBUG
//    std::cout << " End Update." << std::endl;
//    std::cout << "Entities size: " << entityVector.size() << std::endl;
#endif
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
        else if(event.key.code == sf::Keyboard::Escape)
        {
            // reset game
            flags.set(5);
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
    else if(event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f pos = player.sprite.getPosition() + playerWeaponOffsets.at(player.sprite.getPhase());
        fireWeapon(currentWeapon,
            std::atan2(-((float)event.mouseButton.y - pos.y),
                (float)event.mouseButton.x - pos.x),
            pos);
        flags.set(4);
    }
    else if(event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        flags.reset(4);
    }
}

void RoomScreen::draw(sf::RenderWindow& window)
{
#ifndef NDEBUG
//    std::cout << "Start draw...";
#endif
    for(unsigned int i = 0; i < 6; ++i)
    {
        wall.setPosition(walls[i].left, walls[i].top);
        wall.setSize(sf::Vector2f(walls[i].width, walls[i].height));
        window.draw(wall);
    }

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

#ifndef NDEBUG
//    std::cout << " End draw." << std::endl;
#endif
}

unsigned int RoomScreen::switchScreen()
{
    if(flags.test(5))
    {
        return 1;
    }
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
    entityVector.at(i).lifetime = 0.0f;
    entityVector.at(i).type = TYPE_ZOMBIE;
    baseZombieSprite.setPosition(x, y);
    entityVector.at(i).sprite = baseZombieSprite;
}

void RoomScreen::fireWeapon(char type, float angle, const sf::Vector2f& firePos)
{
    switch(type)
    {
    case TYPE_SMG:
        if(weaponSMG.lifetime > 0.0f)
        {
            break;
        }
        weaponSMG.lifetime = WEAPON_SMG_RELOAD_TIME;
        spawnBullet(TYPE_SMG_BULLET, firePos.x, firePos.y, angle);
        break;
    case TYPE_RIFLE:
        if(weaponRifle.lifetime > 0.0f)
        {
            break;
        }
        weaponRifle.lifetime = WEAPON_RIFLE_RELOAD_TIME;
        spawnBullet(TYPE_RIFLE_BULLET, firePos.x, firePos.y, angle);
        break;
    case TYPE_LASER:
        if(weaponLaser.lifetime > 0.0f)
        {
            break;
        }
        weaponLaser.lifetime = WEAPON_LASER_RELOAD_TIME;
        spawnBullet(TYPE_LASER_BULLET, firePos.x, firePos.y, angle);
        break;
    case TYPE_SHOTGUN:
        if(weaponShotgun.lifetime > 0.0f)
        {
            break;
        }
        weaponShotgun.lifetime = WEAPON_SHOTGUN_RELOAD_TIME;
        spawnBullet(TYPE_SHOTGUN_BULLET, firePos.x, firePos.y, angle);
        break;
    }
}

void RoomScreen::spawnBullet(char type, float x, float y, float angle)
{
    if(type != TYPE_SHOTGUN_BULLET)
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
        switch(type)
        {
        case TYPE_SMG_BULLET:
            angle = angle + (zeroToOneDist(gen) - 0.5f) * WEAPON_SMG_SPREAD;
            entityVector.at(i) = weaponSMGBullet;
            entityVector.at(i).velX = std::cos(angle) * WEAPON_SMG_BULLET_SPEED;
            entityVector.at(i).velY = -std::sin(angle) * WEAPON_SMG_BULLET_SPEED;
            break;
        case TYPE_RIFLE_BULLET:
            entityVector.at(i) = weaponRifleBullet;
            entityVector.at(i).velX = std::cos(angle) * WEAPON_RIFLE_BULLET_SPEED;
            entityVector.at(i).velY = -std::sin(angle) * WEAPON_RIFLE_BULLET_SPEED;
            break;
        case TYPE_LASER_BULLET:
            entityVector.at(i) = weaponLaserBullet;
            entityVector.at(i).velX = std::cos(angle) * WEAPON_LASER_BULLET_SPEED;
            entityVector.at(i).velY = -std::sin(angle) * WEAPON_LASER_BULLET_SPEED;
            break;
        default:
            break;
        }
        entityVector.at(i).sprite.setPosition(x, y);
        entityVector.at(i).sprite.setRotation(-angle * 180.0f / std::acos(-1.0f));
    }
    else
    {
        unsigned int indices[WEAPON_SHOTGUN_NUMBER_OF_BULLETS];
        indices[0] = 0;
        float randomizedAngle;
        for(unsigned int i = 0; i < WEAPON_SHOTGUN_NUMBER_OF_BULLETS; ++i)
        {
            // find dead entity
            while(entityVector.at(indices[i]).alive)
            {
                ++(indices[i]);
                if(indices[i] >= entityVector.size())
                {
                    entityVector.resize(entityVector.size() * 2);
                }
            }
            // make shotgun bullet
            entityVector.at(indices[i]) = weaponShotgunBullet;

            randomizedAngle = angle + (zeroToOneDist(gen) - 0.5f) * WEAPON_SHOTGUN_SPREAD;
            entityVector.at(indices[i]).velX = std::cos(randomizedAngle) * WEAPON_SHOTGUN_BULLET_SPEED;
            entityVector.at(indices[i]).velY = -std::sin(randomizedAngle) * WEAPON_SHOTGUN_BULLET_SPEED;

            entityVector.at(indices[i]).sprite.setPosition(x, y);
            entityVector.at(indices[i]).sprite.setRotation(-randomizedAngle * 180.0f / std::acos(-1.0f));

            // prepare for next iteration if there is one
            if(i + 1 < WEAPON_SHOTGUN_NUMBER_OF_BULLETS)
            {
                indices[i+1] = indices[i] + 1;
            }
        }
    }
}

