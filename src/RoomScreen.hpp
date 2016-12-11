
#ifndef LD37_ROOM_SCREEN_HPP
#define LD37_ROOM_SCREEN_HPP

#define ENTITY_VECTOR_START_SIZE 256
#define ZOMBIE_START_HEALTH 100
#define ZOMBIE_WALK_FRAME_TIME 0.35f
#define ZOMBIE_DAMAGE 18
#define ZOMBIE_WALK_SPEED 110.0f

#define PLAYER_START_HEALTH 100
#define PLAYER_WALK_FRAME_TIME 0.27f
#define PLAYER_WALK_SPEED 180.0f

#define WEAPON_SMG_DAMAGE 10
#define WEAPON_RIFLE_DAMAGE 35
#define WEAPON_LASER_DAMAGE 25
#define WEAPON_SHOTGUN_DAMAGE 7

#define WEAPON_SMG_BULLET_SPEED 500.0f
#define WEAPON_RIFLE_BULLET_SPEED 1200.0f
#define WEAPON_LASER_BULLET_SPEED 700.0f
#define WEAPON_SHOTGUN_BULLET_SPEED 400.0f

#define WEAPON_SMG_BULLET_LIFETIME 1.5f
#define WEAPON_RIFLE_BULLET_LIFETIME 1.0f
#define WEAPON_LASER_BULLET_LIFETIME 1.2F
#define WEAPON_SHOTGUN_BULLET_LIFETIME 1.5f

#define WEAPON_SMG_RELOAD_TIME 0.25f
#define WEAPON_RIFLE_RELOAD_TIME 0.8f
#define WEAPON_LASER_RELOAD_TIME 0.5f
#define WEAPON_SHOTGUN_RELOAD_TIME 0.6f

#define TYPE_ZOMBIE 0
#define TYPE_PLAYER 1
#define TYPE_SMG 2
#define TYPE_RIFLE 3
#define TYPE_LASER 4
#define TYPE_SHOTGUN 5
#define TYPE_SMG_BULLET 6
#define TYPE_RIFLE_BULLET 7
#define TYPE_LASER_BULLET 8
#define TYPE_SHOTGUN_BULLET 9

#define WEAPON_SMG_SPREAD 3.1415f/9.0f

#define WEAPON_SHOTGUN_NUMBER_OF_BULLETS 7
#define WEAPON_SHOTGUN_SPREAD 3.1415f/7.0f

#define ZOMBIE_HIT_FADE_TIME 2.0f
#define PLAYER_HIT_FADE_TIME 1.0f

#define PATH_FINDING_BLOCK_SIZE 25.0f

#include <bitset>
#include <random>
#include <unordered_map>

#include <SFML/Audio.hpp>

#include "Screen.hpp"
#include "Sprite.hpp"

struct Entity
{
    Entity();
    Entity(Sprite sprite);

    Sprite sprite;
    int health;
    bool alive;

    float velX;
    float velY;

    float lifetime;
    char type;

    uint64_t destination;
    std::unordered_map<uint64_t, uint64_t> pathMap;
    uint64_t currentDestination;
};

class RoomScreen : public Screen
{
public:
    RoomScreen();
    virtual ~RoomScreen() override;

    virtual void update(float dt, sf::RenderWindow& window) override;
    virtual void handleEvent(const sf::Event& event) override;
    virtual void draw(sf::RenderWindow& window) override;

    virtual unsigned int switchScreen() override;

    void createZombie(float x, float y);
    void fireWeapon(char type, float angle, const sf::Vector2f& pos);
    void spawnBullet(char type, float x, float y, float angle);

private:
    sf::Texture zombieTexture;
    Sprite baseZombieSprite;
    Sprite::SpriteMap zombieSpriteMap;
    std::vector<Entity> entityVector;

    sf::Texture playerTexture;
    Entity player;

    /**
        0 - Left pressed
        1 - Right pressed
        2 - Up pressed
        3 - Down pressed
        4 - Mouse button is down
        5 - reset screen
    */
    std::bitset<32> flags;

    const float playerDiag;

    sf::Texture weaponsTexture;
    Entity weaponSMG;
    Entity weaponSMGBullet;
    Entity weaponRifle;
    Entity weaponRifleBullet;
    Entity weaponLaser;
    Entity weaponLaserBullet;
    Entity weaponShotgun;
    Entity weaponShotgunBullet;

    std::mt19937 gen;
    std::uniform_real_distribution<float> zeroToOneDist;

    std::unordered_map<unsigned int, sf::Vector2f> playerWeaponOffsets;

    unsigned int currentWeapon;

    sf::Music zombieFightMusic;

    sf::RectangleShape wall;
    sf::FloatRect walls[6];
    float wallsF[6][8];

};

#endif

