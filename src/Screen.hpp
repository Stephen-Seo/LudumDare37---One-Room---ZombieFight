
#ifndef LD37_SCREEN_HPP
#define LD37_SCREEN_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Screen
{
public:
    typedef std::unique_ptr<Screen> Ptr;

    Screen() {};
    virtual ~Screen() {};

    virtual void update(float dt, sf::RenderWindow& window) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual unsigned int switchScreen() = 0;

private:

};

#endif

