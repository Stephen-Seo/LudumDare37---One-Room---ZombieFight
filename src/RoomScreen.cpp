
#include "RoomScreen.hpp"

#include <cmath>
#include <string>
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
currentWeapon(TYPE_SMG),
currentLevel(1),
zombieCount(1),
playerRegenTimer(0.0f),
itemBoxEntity()
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
    walls[0].left = 200;
    walls[0].top = 125;
    walls[0].width = 25;
    walls[0].height = 100;

    walls[1].left = 225;
    walls[1].top = 125;
    walls[1].width = 100;
    walls[1].height = 25;

    walls[2].left = 575;
    walls[2].top = 250;
    walls[2].width = 25;
    walls[2].height = 100;

    walls[3].left = 475;
    walls[3].top = 325;
    walls[3].width = 100;
    walls[3].height = 25;
/*
    walls[4].left = 500;
    walls[4].top = 125;
    walls[4].width = 125;
    walls[4].height = 25;

    walls[5].left = 500;
    walls[5].top = 300;
    walls[5].width = 125;
    walls[5].height = 25;
*/
    for(unsigned int i = 0; i < 4; ++i)
    {
        wallsF[i][0] = walls[i].left;
        wallsF[i][1] = walls[i].top;
        wallsF[i][2] = walls[i].left + walls[i].width;
        wallsF[i][3] = walls[i].top;
        wallsF[i][4] = walls[i].left + walls[i].width;
        wallsF[i][5] = walls[i].top + walls[i].height;
        wallsF[i][6] = walls[i].left;
        wallsF[i][7] = walls[i].top + walls[i].height;
    }
/*
    wallsFAlternate[0][0] = 150;
    wallsFAlternate[0][1] = 125 - 25;
    wallsFAlternate[0][2] = 150 + 25;
    wallsFAlternate[0][3] = 125 - 25;
    wallsFAlternate[0][4] = 150 + 25;
    wallsFAlternate[0][5] = 125 + 200;
    wallsFAlternate[0][6] = 150;
    wallsFAlternate[0][7] = 125 + 200;

    wallsFAlternate[1][0] = 175;
    wallsFAlternate[1][1] = 125 - 25;
    wallsFAlternate[1][2] = 175 + 125;
    wallsFAlternate[1][3] = 125 - 25;
    wallsFAlternate[1][4] = 175 + 125;
    wallsFAlternate[1][5] = 125 + 25;
    wallsFAlternate[1][6] = 175;
    wallsFAlternate[1][7] = 125 + 25;

    wallsFAlternate[2][0] = 175;
    wallsFAlternate[2][1] = 300 - 25;
    wallsFAlternate[2][2] = 175 + 125;
    wallsFAlternate[2][3] = 300 - 25;
    wallsFAlternate[2][4] = 175 + 125;
    wallsFAlternate[2][5] = 300 + 25;
    wallsFAlternate[2][6] = 175;
    wallsFAlternate[2][7] = 300 + 25;

    wallsFAlternate[3][0] = 625;
    wallsFAlternate[3][1] = 125 - 25;
    wallsFAlternate[3][2] = 625 + 25;
    wallsFAlternate[3][3] = 125 - 25;
    wallsFAlternate[3][4] = 625 + 25;
    wallsFAlternate[3][5] = 125 + 200;
    wallsFAlternate[3][6] = 625;
    wallsFAlternate[3][7] = 125 + 200;

    wallsFAlternate[4][0] = 500;
    wallsFAlternate[4][1] = 125 - 25;
    wallsFAlternate[4][2] = 500 + 125;
    wallsFAlternate[4][3] = 125 - 25;
    wallsFAlternate[4][4] = 500 + 125;
    wallsFAlternate[4][5] = 125 + 25;
    wallsFAlternate[4][6] = 500;
    wallsFAlternate[4][7] = 125 + 25;

    wallsFAlternate[5][0] = 500;
    wallsFAlternate[5][1] = 300 - 25;
    wallsFAlternate[5][2] = 500 + 125;
    wallsFAlternate[5][3] = 300 - 25;
    wallsFAlternate[5][4] = 500 + 125;
    wallsFAlternate[5][5] = 300 + 25;
    wallsFAlternate[5][6] = 500;
    wallsFAlternate[5][7] = 300 + 25;
*/
    wall.setFillColor(sf::Color(127, 50, 0));

    // other initializations
    createZombie(-50, -50);

    // music
    zombieFightMusic.openFromFile("res/LD37_ZombieFight.ogg");
    zombieFightMusic.setLoop(true);
    zombieFightMusic.setVolume(70.0f);
    zombieFightMusic.play();

    // sfx
    smgSfxBuffer.loadFromFile("res/smg.ogg");
    rifleSfxBuffer.loadFromFile("res/rifle.ogg");
    laserSfxBuffer.loadFromFile("res/laser.ogg");
    shotgunSfxBuffer.loadFromFile("res/shotgun.ogg");
    zombieHitBuffer.loadFromFile("res/zombieHit.ogg");
    zombieDeadBuffer.loadFromFile("res/zombieDead.ogg");
    weaponGetBuffer.loadFromFile("res/weaponGet.ogg");

    smgSfxSound.setBuffer(smgSfxBuffer);
    rifleSfxSound.setBuffer(rifleSfxBuffer);
    laserSfxSound.setBuffer(laserSfxBuffer);
    shotgunSfxSound.setBuffer(shotgunSfxBuffer);
    zombieHitSound.setBuffer(zombieHitBuffer);
    zombieDeadSound.setBuffer(zombieDeadBuffer);
    weaponGetSound.setBuffer(weaponGetBuffer);

    // hp bar
    hpBar.setSize(sf::Vector2f(HP_BAR_WIDTH, HP_BAR_HEIGHT));
    hpBar.setOutlineThickness(1.0f);
    hpBar.setOutlineColor(sf::Color::White);

    // item box
    itemBoxEntity.alive = true;
    itemBoxEntity.type = TYPE_ITEM_BOX;
    itemBoxEntity.sprite.setTexture(weaponsTexture);
    itemBoxEntity.sprite.registerMapping(0, SpriteData(
        sf::IntRect(0, 48, 32, 32), 0.1f
    ));
    itemBoxEntity.sprite.registerMapping(0, SpriteData(
        sf::IntRect(32, 48, 32, 32), 0.1f
    ));
    itemBoxEntity.sprite.registerMapping(0, SpriteData(
        sf::IntRect(64, 48, 32, 32), 0.1f
    ));
    itemBoxEntity.sprite.registerMapping(0, SpriteData(
        sf::IntRect(96, 48, 32, 32), 0.1f
    ));
    itemBoxEntity.sprite.registerMapping(0, SpriteData(
        sf::IntRect(128, 48, 32, 32), 0.1f
    ));
    itemBoxEntity.lifetime = ITEM_BOX_EXPIRE_TIME;

    // current weapon icon
    currentWeaponIcon.setTexture(weaponsTexture);
    currentWeaponIcon.setPosition(0.0f, 450.0f - 32.0f);

    // setup weapon count
    ammoCount.insert(std::make_pair(TYPE_SMG, 100));
    ammoCount.insert(std::make_pair(TYPE_RIFLE, 0));
    ammoCount.insert(std::make_pair(TYPE_LASER, 0));
    ammoCount.insert(std::make_pair(TYPE_SHOTGUN, 0));

    // font
    font.loadFromFile("res/ClearSans-Regular.ttf");

    // ammo text
    currentAmmoCount.setFont(font);
    currentAmmoCount.setCharacterSize(30);
    currentAmmoCount.setFillColor(sf::Color::White);
    currentAmmoCount.setOutlineColor(sf::Color::Black);
    currentAmmoCount.setOutlineThickness(2.0);
    currentAmmoCount.setPosition(2.0f, 450.0f - 64.0f);
    updateWeaponInfo();

    // info text
    infoText.setFont(font);
    infoText.setCharacterSize(30);
    infoText.setFillColor(sf::Color::White);
    infoText.setOutlineColor(sf::Color::Green);
    infoText.setOutlineThickness(1.0f);

//    displayInfo("Spacebar/RightClick to change weapons\n(when you get them)");
    displayInfo("WASD - Movement\nMouse Left Click - Fire current weapon");

    // reload meter
    reloadMeter.setSize(sf::Vector2f(32.0f, 5.0f));
    reloadMeter.setPosition(0.0f, 450.0f - 5.0f);

#ifndef NDEBUG
    zombieTarget.setSize(sf::Vector2f(PATH_FINDING_BLOCK_WIDTH, PATH_FINDING_BLOCK_HEIGHT));
    zombieTarget.setFillColor(sf::Color(255, 255, 0, 127));
#endif
}

RoomScreen::~RoomScreen()
{
}

void RoomScreen::update(float dt, sf::RenderWindow& window)
{
#ifndef NDEBUG
//    std::cout << "Start update...";
#endif
    // check zombie count
    if(zombieCount == 0)
    {
        ++currentLevel;
        if(currentLevel != LEVEL_TO_WIN)
        {
            for(unsigned int i = 0; i < currentLevel; ++i)
            {
                float r = zeroToOneDist(gen);
                if(r < 0.25f)
                {
                    createZombie(-50.0f, zeroToOneDist(gen) * 450.0f - 32.0f);
                }
                else if(r < 0.5f)
                {
                    createZombie(zeroToOneDist(gen) * 800.0f - 16.0f, -70.0f);
                }
                else if(r < 0.75f)
                {
                    createZombie(850.0f, zeroToOneDist(gen) * 450.0f - 32.0f);
                }
                else
                {
                    createZombie(zeroToOneDist(gen) * 800.0f - 16.0f, 470.0f);
                }
            }
        }
        else
        {
            zombieFightMusic.stop();
            zombieFightMusic.openFromFile("res/survive.ogg");
            zombieFightMusic.setVolume(100.0f);
            zombieFightMusic.setLoop(false);
            zombieFightMusic.play();
            displayInfo("You Survived!!");
        }
        zombieCount = currentLevel;
    }

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
                        auto checkCollision = [this, &iter, &iter1] (float* array, sf::Vector2f& point) -> bool {
                            if(GDT::isWithinPolygon(array, 8, point.x, point.y))
                            {
                                iter1->health -= iter->health;
                                if(iter1->health <= 0)
                                {
                                    iter1->alive = false;
                                    zombieDeadSound.play();
                                    --zombieCount;
                                    if(zeroToOneDist(gen) < ITEM_BOX_DROP_RATE)
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
                                        itemBoxEntity.sprite.setPosition(
                                            iter->sprite.getPosition().x,
                                            iter->sprite.getPosition().y + 16.0f
                                        );
                                        entityVector.at(i) = itemBoxEntity;
                                    }
                                }
                                else
                                {
                                    zombieHitSound.play();
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
                    int32_t zombieX = iter->sprite.getPosition().x / PATH_FINDING_BLOCK_WIDTH;
                    int32_t zombieY = iter->sprite.getPosition().y / PATH_FINDING_BLOCK_HEIGHT;
                    int32_t playerX = player.sprite.getPosition().x / PATH_FINDING_BLOCK_WIDTH;
                    int32_t playerY = player.sprite.getPosition().y / PATH_FINDING_BLOCK_HEIGHT;

                    auto pathMap = shortestPath(zombieX, zombieY, playerX, playerY,
                        [this] (int32_t x, int32_t y) -> bool {
                            if(x < -100.0f / PATH_FINDING_BLOCK_WIDTH ||
                                y < -100.0f / PATH_FINDING_BLOCK_HEIGHT ||
                                x > (800.0f + 100.0f) / PATH_FINDING_BLOCK_WIDTH ||
                                y > (450.0f + 100.0f) / PATH_FINDING_BLOCK_HEIGHT)
                            {
                                return true;
                            }
                            for(unsigned int i = 0; i < 4; ++i)
                            {
                                if(GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_WIDTH,
                                        y * PATH_FINDING_BLOCK_HEIGHT) ||
                                    GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_WIDTH + PATH_FINDING_BLOCK_WIDTH,
                                        y * PATH_FINDING_BLOCK_HEIGHT) ||
                                    GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_WIDTH + PATH_FINDING_BLOCK_WIDTH,
                                        y * PATH_FINDING_BLOCK_HEIGHT + PATH_FINDING_BLOCK_HEIGHT) ||
                                    GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_WIDTH,
                                        y * PATH_FINDING_BLOCK_HEIGHT + PATH_FINDING_BLOCK_HEIGHT) ||
                                    GDT::isWithinPolygon(wallsF[i], 8,
                                        x * PATH_FINDING_BLOCK_WIDTH + PATH_FINDING_BLOCK_WIDTH / 2.0f,
                                        y * PATH_FINDING_BLOCK_HEIGHT + PATH_FINDING_BLOCK_HEIGHT / 2.0f))
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

//                    iter->currentDestination = iter->actualDestination;
                    iter->destinationResetTime = 0.0f;
//                    iter->collideX = 0;
//                    iter->collideY = 0;
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
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH + PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH + PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT + PATH_FINDING_BLOCK_HEIGHT) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT + PATH_FINDING_BLOCK_HEIGHT);
                while (colliding)
                {
                    iter->currentDestination = iter->pathMap.at(iter->currentDestination);
//                    iter->currentDestination = iter->actualDestination;
                    iter->destinationResetTime = 0.0f;
                    colliding = GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH + PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH + PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT + PATH_FINDING_BLOCK_HEIGHT) ||
                    GDT::isWithinPolygon(zombieBox, 8,
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT + PATH_FINDING_BLOCK_HEIGHT);
//                    iter->collideX = 0;
//                    iter->collideY = 0;
#ifndef NDEBUG
                    zombieTarget.setPosition(
                        ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH,
                        ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT
                    );
#endif
                    if(iter->destination == iter->currentDestination)
                    {
                        break;
                    }
                }

                if(iter->destinationResetTime > 0)
                {
                    iter->destinationResetTime -= dt;
                    if(iter->destinationResetTime < 0)
                    {
                        iter->destinationResetTime = 0.0f;
//                        iter->currentDestination = iter->actualDestination;
                    }
                }
                // move
                float magnitude;
                iter->velX = ((int32_t*)&iter->currentDestination)[0] * PATH_FINDING_BLOCK_WIDTH;
//                    + PATH_FINDING_BLOCK_SIZE / 2.0f;
                iter->velY = ((int32_t*)&iter->currentDestination)[1] * PATH_FINDING_BLOCK_HEIGHT;
//                    + PATH_FINDING_BLOCK_SIZE / 2.0f;
/*
                for(unsigned int i = 0; i < 6; ++i)
                {
                    // check right
                    if(GDT::isWithinPolygon(wallsF[0], 8,
                        iter->velX + PATH_FINDING_BLOCK_SIZE,
                        iter->velY))
                    {
                        iter->velX -= PATH_FINDING_BLOCK_SIZE;
                    }
                    // check left
                    else if(GDT::isWithinPolygon(wallsF[0], 8,
                        iter->velX - PATH_FINDING_BLOCK_SIZE,
                        iter->velY))
                    {
                        iter->velX += PATH_FINDING_BLOCK_SIZE;
                    }
                    // check up
                    if(GDT::isWithinPolygon(wallsF[0], 8,
                        iter->velX,
                        iter->velY - PATH_FINDING_BLOCK_SIZE))
                    {
                        iter->velY += PATH_FINDING_BLOCK_SIZE;
                    }
                    // check down
                    else if(GDT::isWithinPolygon(wallsF[0], 8,
                        iter->velX,
                        iter->velY + PATH_FINDING_BLOCK_SIZE))
                    {
                        iter->velY -= PATH_FINDING_BLOCK_SIZE;
                    }
                }
                */
                iter->velX = iter->velX - iter->sprite.getPosition().x;// - 16.0f;
                iter->velY = iter->velY - iter->sprite.getPosition().y;// - 32.0f;
                /*
                iter->velX = (((int32_t*) &iter->currentDestination)[0] * PATH_FINDING_BLOCK_SIZE
                    + PATH_FINDING_BLOCK_SIZE / 2.0f)
                    - iter->sprite.getPosition().x - 16.0f;
                iter->velY = (((int32_t*) &iter->currentDestination)[1] * PATH_FINDING_BLOCK_SIZE
                    + PATH_FINDING_BLOCK_SIZE / 2.0f)
                    - iter->sprite.getPosition().y - 32.0f;
                */
                /*
                iter->velX += iter->collideX * PATH_FINDING_BLOCK_SIZE;
                iter->velY += iter->collideY * PATH_FINDING_BLOCK_SIZE;
                */
                /*
                if(iter->velX > 0)
                {
                    iter->velX += iter->collideX * PATH_FINDING_BLOCK_SIZE;
                }
                else
                {
                    iter->velX -= iter->collideX * PATH_FINDING_BLOCK_SIZE;
                }
                if(iter->velY > 0)
                {
                    iter->velY += iter->collideY * PATH_FINDING_BLOCK_SIZE;
                }
                else
                {
                    iter->velY -= iter->collideY * PATH_FINDING_BLOCK_SIZE;
                }
                */

                magnitude = std::sqrt(iter->velX * iter->velX + iter->velY * iter->velY);

                iter->velX = iter->velX / magnitude * ZOMBIE_WALK_SPEED * (1.0f + iter->lifetime / 2.0f);
                iter->velY = iter->velY / magnitude * ZOMBIE_WALK_SPEED * (1.0f + iter->lifetime / 2.0f);

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
            else if(iter->type == TYPE_ITEM_BOX)
            {
                // check if player picked up box
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
                        iter->sprite.getPosition().y + 32.0f) ||
                    GDT::isWithinPolygon(playerBox, 8,
                        iter->sprite.getPosition().x,
                        iter->sprite.getPosition().y + 32.0f))
                {
                    iter->alive = false;
                    float r = zeroToOneDist(gen);
                    if(r < 0.25f)
                    {
                        ammoCount.at(TYPE_SMG) += ITEM_BOX_SMG_AMMO;
                        currentWeapon = TYPE_SMG;
                        updateWeaponInfo();
                    }
                    else if(r < 0.5f)
                    {
                        ammoCount.at(TYPE_RIFLE) += ITEM_BOX_RIFLE_AMMO;
                        currentWeapon = TYPE_RIFLE;
                        updateWeaponInfo();
                    }
                    else if(r < 0.75f)
                    {
                        ammoCount.at(TYPE_LASER) += ITEM_BOX_LASER_AMMO;
                        currentWeapon = TYPE_LASER;
                        updateWeaponInfo();
                    }
                    else
                    {
                        ammoCount.at(TYPE_SHOTGUN) += ITEM_BOX_SHOTGUN_AMMO;
                        currentWeapon = TYPE_SHOTGUN;
                        updateWeaponInfo();
                    }
                    weaponGetSound.play();
                }

                // check item box expire
                iter->lifetime -= dt;
                if(iter->lifetime < 0.0f)
                {
                    iter->alive = false;
                }
                else
                {
                    iter->sprite.setColor(sf::Color(255, 255, 255,
                        255 * iter->lifetime / ITEM_BOX_EXPIRE_TIME));
                }
            }


            // movement checks
            iter->sprite.update(dt);
            if(iter->type == TYPE_ZOMBIE)
            {
                bool hit = false;
                iter->sprite.move(
                    iter->velX * dt,
                    0);
                for(unsigned int i = 0; i < 4; ++i)
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
                        -iter->velX * dt / 2.0f,
                        0
                    );
                    hit = false;
                    /*
                    if(iter->velX > 0)
                    {
//                        iter->collideX -= 3;
                        ((int32_t*)&iter->currentDestination)[0] -= 1;
                        iter->destinationResetTime = ZOMBIE_PATH_RESET_TIME;
#ifndef NDEBUG
                        zombieTarget.setPosition(
                            ((int32_t*)&iter->currentDestination)[0] * 25,
                            ((int32_t*)&iter->currentDestination)[1] * 25
                        );
#endif
                    }
                    else
                    {
//                        iter->collideX += 3;
                        ((int32_t*)&iter->currentDestination)[0] += 1;
                        iter->destinationResetTime = ZOMBIE_PATH_RESET_TIME;
#ifndef NDEBUG
                        zombieTarget.setPosition(
                            ((int32_t*)&iter->currentDestination)[0] * 25,
                            ((int32_t*)&iter->currentDestination)[1] * 25
                        );
#endif
                    }
                    */
                }
                /*
                if(iter->collideX > 0)
                {
                    iter->collideX -= 1;
                }
                else if(iter->collideX < 0)
                {
                    iter->collideX += 1;
                }
                */

                iter->sprite.move(
                    0,
                    iter->velY * dt);
                for(unsigned int i = 0; i < 4; ++i)
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
                        -iter->velY * dt / 2.0f
                    );
                    /*
                    if(iter->velY > 0)
                    {
//                        iter->collideY -= 3;
                        ((int32_t*)&iter->currentDestination)[1] -= 1;
                        iter->destinationResetTime = ZOMBIE_PATH_RESET_TIME;
#ifndef NDEBUG
                        zombieTarget.setPosition(
                            ((int32_t*)&iter->currentDestination)[0] * 25,
                            ((int32_t*)&iter->currentDestination)[1] * 25
                        );
#endif
                    }
                    else
                    {
//                        iter->collideY += 3;
                        ((int32_t*)&iter->currentDestination)[1] += 1;
                        iter->destinationResetTime = ZOMBIE_PATH_RESET_TIME;
#ifndef NDEBUG
                        zombieTarget.setPosition(
                            ((int32_t*)&iter->currentDestination)[0] * 25,
                            ((int32_t*)&iter->currentDestination)[1] * 25
                        );
#endif
                    }
                    */
                }
                /*
                if(iter->collideY > 0)
                {
                    iter->collideY -= 1;
                }
                else if(iter->collideY < 0)
                {
                    iter->collideY += 1;
                }
                */
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
                    for(unsigned int i = 0; i < 4; ++i)
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
                    for(unsigned int i = 0; i < 4; ++i)
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
                    for(unsigned int i = 0; i < 4; ++i)
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
            for(unsigned int i = 0; i < 4; ++i)
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
            for(unsigned int i = 0; i < 4; ++i)
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
                        player.sprite.getPosition().y + 42.7f) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 16.0f,
                        player.sprite.getPosition().y) ||
                    GDT::isWithinPolygon(box, 8,
                        player.sprite.getPosition().x + 16.0f,
                        player.sprite.getPosition().y + 64.0f))
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
        player.lifetime -= dt;
        if(player.lifetime < 0.0f)
        {
            player.lifetime = 0.0f;
        }
        unsigned char playerHit = 255 * (1.0f - player.lifetime / PLAYER_HIT_FADE_TIME);
        player.sprite.setColor(sf::Color(255, playerHit, playerHit));

        // update hp bar
        float percentage = (float)player.health / (float)PLAYER_START_HEALTH;
        hpBar.setSize(sf::Vector2f(HP_BAR_WIDTH * percentage, HP_BAR_HEIGHT));
        hpBar.setFillColor(sf::Color(255 * (1.0f - percentage), 255 * percentage, 0));
        hpBar.setPosition(
            player.sprite.getPosition().x + 16.0f - (HP_BAR_WIDTH * percentage / 2.0f),
            player.sprite.getPosition().y - 5.0f - HP_BAR_HEIGHT);

        playerRegenTimer -= dt;
        if(playerRegenTimer < 0.0f)
        {
            playerRegenTimer = PLAYER_HP_REGEN_TIME;
            player.health += 3;
            if(player.health > PLAYER_START_HEALTH)
            {
                player.health = PLAYER_START_HEALTH;
            }
        }
    }
    else
    {
        player.sprite.setColor(sf::Color(200, 0, 0));
        hpBar.setFillColor(sf::Color::Transparent);
    }

    // update info text
    infoTimer -= dt;
    if(infoTimer < 1.0f)
    {
        if(infoTimer < 0.0f)
        {
            infoTimer = 0.0f;
            infoText.setFillColor(sf::Color::Transparent);
            infoText.setOutlineColor(sf::Color::Transparent);
            if(!flags.test(6))
            {
                flags.set(6);
                displayInfo("Spacebar/RightClick to change weapons\n(when you get them)");
            }
            else if(!flags.test(7))
            {
                flags.set(7);
                displayInfo("Escape to restart");
            }
        }
        else
        {
            infoText.setFillColor(sf::Color(255, 255, 255, 255 * infoTimer));
            infoText.setOutlineColor(sf::Color(0, 255, 0, 255 * infoTimer));
        }
    }

    // update reload meter
    updateReloadMeter();

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
        else if(event.key.code == sf::Keyboard::Space)
        {
            switchWeapon();
            updateWeaponInfo();
        }
#ifndef NDEBUG
        else if(event.key.code == sf::Keyboard::J)
        {
            std::cout << "J pressed" << std::endl;
        }
#endif
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
    else if(event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Right)
    {
        switchWeapon();
        updateWeaponInfo();
    }
}

void RoomScreen::draw(sf::RenderWindow& window)
{
#ifndef NDEBUG
//    std::cout << "Start draw...";
#endif
    for(unsigned int i = 0; i < 4; ++i)
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

    window.draw(hpBar);
    window.draw(currentWeaponIcon);
    window.draw(currentAmmoCount);
    window.draw(infoText);
    window.draw(reloadMeter);

#ifndef NDEBUG
//    std::cout << " End draw." << std::endl;
//    window.draw(zombieTarget);
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
        if(weaponSMG.lifetime > 0.0f || ammoCount.at(type) == 0)
        {
            break;
        }
        weaponSMG.lifetime = WEAPON_SMG_RELOAD_TIME;
        spawnBullet(TYPE_SMG_BULLET, firePos.x, firePos.y, angle);
        ammoCount.at(type) -= 1;
        smgSfxSound.play();
        updateWeaponInfo();
        break;
    case TYPE_RIFLE:
        if(weaponRifle.lifetime > 0.0f || ammoCount.at(type) == 0)
        {
            break;
        }
        weaponRifle.lifetime = WEAPON_RIFLE_RELOAD_TIME;
        spawnBullet(TYPE_RIFLE_BULLET, firePos.x, firePos.y, angle);
        ammoCount.at(type) -= 1;
        rifleSfxSound.play();
        updateWeaponInfo();
        break;
    case TYPE_LASER:
        if(weaponLaser.lifetime > 0.0f || ammoCount.at(type) == 0)
        {
            break;
        }
        weaponLaser.lifetime = WEAPON_LASER_RELOAD_TIME;
        spawnBullet(TYPE_LASER_BULLET, firePos.x, firePos.y, angle);
        ammoCount.at(type) -= 1;
        laserSfxSound.play();
        updateWeaponInfo();
        break;
    case TYPE_SHOTGUN:
        if(weaponShotgun.lifetime > 0.0f || ammoCount.at(type) == 0)
        {
            break;
        }
        weaponShotgun.lifetime = WEAPON_SHOTGUN_RELOAD_TIME;
        spawnBullet(TYPE_SHOTGUN_BULLET, firePos.x, firePos.y, angle);
        ammoCount.at(type) -= 1;
        shotgunSfxSound.play();
        updateWeaponInfo();
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

void RoomScreen::updateWeaponInfo()
{
    switch(currentWeapon)
    {
    case TYPE_SMG:
        currentWeaponIcon.setTextureRect(sf::IntRect(0, 0, 32, 32));
        break;
    case TYPE_RIFLE:
        currentWeaponIcon.setTextureRect(sf::IntRect(32, 0, 48, 32));
        break;
    case TYPE_LASER:
        currentWeaponIcon.setTextureRect(sf::IntRect(80, 0, 32, 32));
        break;
    case TYPE_SHOTGUN:
        currentWeaponIcon.setTextureRect(sf::IntRect(112, 0, 48, 32));
        break;
    default:
        break;
    }
    currentAmmoCount.setString(std::to_string(ammoCount.at(currentWeapon)));
}

void RoomScreen::switchWeapon()
{
    if(currentWeapon == TYPE_SMG)
    {
        if(ammoCount.at(TYPE_RIFLE) > 0)
        {
            currentWeapon = TYPE_RIFLE;
        }
        else if(ammoCount.at(TYPE_LASER) > 0)
        {
            currentWeapon = TYPE_LASER;
        }
        else if(ammoCount.at(TYPE_SHOTGUN) > 0)
        {
            currentWeapon = TYPE_SHOTGUN;
        }
    }
    else if(currentWeapon == TYPE_RIFLE)
    {
        if(ammoCount.at(TYPE_LASER) > 0)
        {
            currentWeapon = TYPE_LASER;
        }
        else if(ammoCount.at(TYPE_SHOTGUN) > 0)
        {
            currentWeapon = TYPE_SHOTGUN;
        }
        else if(ammoCount.at(TYPE_SMG) > 0)
        {
            currentWeapon = TYPE_SMG;
        }
    }
    else if(currentWeapon == TYPE_LASER)
    {
        if(ammoCount.at(TYPE_SHOTGUN) > 0)
        {
            currentWeapon = TYPE_SHOTGUN;
        }
        else if(ammoCount.at(TYPE_SMG) > 0)
        {
            currentWeapon = TYPE_SMG;
        }
        else if(ammoCount.at(TYPE_RIFLE) > 0)
        {
            currentWeapon = TYPE_RIFLE;
        }
    }
    else if(currentWeapon == TYPE_SHOTGUN)
    {
        if(ammoCount.at(TYPE_SMG) > 0)
        {
            currentWeapon = TYPE_SMG;
        }
        else if(ammoCount.at(TYPE_RIFLE) > 0)
        {
            currentWeapon = TYPE_RIFLE;
        }
        else if(ammoCount.at(TYPE_LASER) > 0)
        {
            currentWeapon = TYPE_LASER;
        }
    }
}

void RoomScreen::displayInfo(const sf::String& string)
{
    infoText.setString(string);
    infoText.setPosition(400.0f - infoText.getLocalBounds().width / 2.0f, 200.0f);
    infoTimer = INFO_TEXT_TIME;
    infoText.setFillColor(sf::Color::White);
    infoText.setOutlineColor(sf::Color::Green);
}

void RoomScreen::updateReloadMeter()
{
    switch(currentWeapon)
    {
    case TYPE_SMG:
        reloadMeter.setSize(sf::Vector2f(32.0f * (1.0f - weaponSMG.lifetime / WEAPON_SMG_RELOAD_TIME), 5.0f));
        if(weaponSMG.lifetime == 0.0f)
        {
            reloadMeter.setFillColor(sf::Color::Green);
        }
        else
        {
            reloadMeter.setFillColor(sf::Color::Red);
        }
        break;
    case TYPE_RIFLE:
        reloadMeter.setSize(sf::Vector2f(48.0f * (1.0f - weaponRifle.lifetime / WEAPON_RIFLE_RELOAD_TIME), 5.0f));
        if(weaponRifle.lifetime == 0.0f)
        {
            reloadMeter.setFillColor(sf::Color::Green);
        }
        else
        {
            reloadMeter.setFillColor(sf::Color::Red);
        }
        break;
    case TYPE_LASER:
        reloadMeter.setSize(sf::Vector2f(32.0f * (1.0f - weaponLaser.lifetime / WEAPON_LASER_RELOAD_TIME), 5.0f));
        if(weaponLaser.lifetime == 0.0f)
        {
            reloadMeter.setFillColor(sf::Color::Green);
        }
        else
        {
            reloadMeter.setFillColor(sf::Color::Red);
        }
        break;
    case TYPE_SHOTGUN:
        reloadMeter.setSize(sf::Vector2f(48.0f * (1.0f - weaponShotgun.lifetime / WEAPON_SHOTGUN_RELOAD_TIME), 5.0f));
        if(weaponShotgun.lifetime == 0.0f)
        {
            reloadMeter.setFillColor(sf::Color::Green);
        }
        else
        {
            reloadMeter.setFillColor(sf::Color::Red);
        }
        break;
    }
}

