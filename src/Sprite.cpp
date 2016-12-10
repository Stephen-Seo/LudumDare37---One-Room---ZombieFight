
#include "Sprite.hpp"

#include <cassert>

SpriteData::SpriteData(sf::IntRect subRect, float duration) :
subRect(subRect),
duration(duration)
{
}

Sprite::Sprite(SpriteMap* externalMap) :
sprite(),
currentPhase(0),
currentIndex(0),
timer(0.0f),
spriteDirty(true),
spriteMap(),
externalSpriteMap(externalMap)
{
}

Sprite::Sprite(sf::Texture& texture, SpriteMap* externalMap) :
sprite(texture),
currentPhase(0),
currentIndex(0),
timer(0.0f),
spriteDirty(true),
spriteMap(),
externalSpriteMap(externalMap)
{
}

void Sprite::setTexture(sf::Texture& texture)
{
    sprite.setTexture(texture);
}

void Sprite::setExternalMap(SpriteMap* externalMap)
{
    externalSpriteMap = externalMap;
}

void Sprite::registerMapping(unsigned int phase, SpriteData data)
{
    auto found = spriteMap.find(phase);
    if(found == spriteMap.end())
    {
        spriteMap.insert(std::make_pair(phase, std::vector<SpriteData>{data}));
    }
    else
    {
        found->second.push_back(data);
    }
}

void Sprite::clearMapping(unsigned int phase)
{
    spriteMap.erase(phase);
}

void Sprite::clearAllMappings()
{
    spriteMap.clear();
}

void Sprite::setPhase(unsigned int phase)
{
    currentPhase = phase;

    auto currentPhaseVector = (externalSpriteMap ?
        externalSpriteMap->find(currentPhase)->second :
        spriteMap.find(currentPhase)->second);
    currentIndex = currentIndex % currentPhaseVector.size();

    spriteDirty = true;
}

unsigned int Sprite::getPhase()
{
    return currentPhase;
}

void Sprite::update(float dt)
{
    timer += dt;

#ifndef NDEBUG
    if(externalSpriteMap)
    {
        assert(currentPhase < externalSpriteMap->size());
    }
    else
    {
        assert(currentPhase < spriteMap.size());
    }
#endif

    auto currentPhaseVector = (externalSpriteMap ?
        externalSpriteMap->find(currentPhase)->second :
        spriteMap.find(currentPhase)->second);
    if(currentPhaseVector.at(currentIndex).duration <= timer)
    {
        currentIndex = (currentIndex + 1) % currentPhaseVector.size();
        timer = 0.0f;
        spriteDirty = true;
    }
}

void Sprite::draw(sf::RenderWindow& window)
{
    if(spriteDirty)
    {
        sprite.setTextureRect(externalSpriteMap ?
            externalSpriteMap->find(currentPhase)->second.at(currentIndex).subRect :
            spriteMap.find(currentPhase)->second.at(currentIndex).subRect);
        spriteDirty = false;
    }

    window.draw(sprite, sf::RenderStates(getTransform()));
}

void Sprite::setColor(sf::Color color)
{
    sprite.setColor(color);
}

