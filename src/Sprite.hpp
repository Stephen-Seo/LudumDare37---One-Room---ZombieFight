
#ifndef LD37_SPRITE_HPP
#define LD37_SPRITE_HPP

#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

struct SpriteData
{
    SpriteData() = default;
    SpriteData(sf::IntRect subRect, float duration);

    // copy
    SpriteData(const SpriteData& other) = default;
    SpriteData& operator = (const SpriteData& other) = default;

    sf::IntRect subRect;
    float duration;
};

class Sprite : public sf::Transformable
{
public:
    typedef std::unordered_map<unsigned int, std::vector<SpriteData> > SpriteMap;

    Sprite(SpriteMap* externalMap = nullptr);
    Sprite(sf::Texture& texture, SpriteMap* externalMap = nullptr);

    // copy
    Sprite(const Sprite& other) = default;
    Sprite& operator = (const Sprite& other) = default;

    void setTexture(sf::Texture& texture);
    void setExternalMap(SpriteMap* externalMap);

    void registerMapping(unsigned int phase, SpriteData data);
    void clearMapping(unsigned int phase);
    void clearAllMappings();

    void setPhase(unsigned int phase);
    unsigned int getPhase();

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void setColor(sf::Color color = sf::Color::White);

private:
    sf::Sprite sprite;
    unsigned int currentPhase;
    unsigned int currentIndex;
    float timer;
    bool spriteDirty;

    SpriteMap spriteMap;
    SpriteMap* externalSpriteMap;

};

#endif

