
#ifndef LD37_ROOM_SCREEN_HPP
#define LD37_ROOM_SCREEN_HPP

#include "Screen.hpp"

class RoomScreen : public Screen
{
public:
    RoomScreen();
    virtual ~RoomScreen() override;

    virtual void update(float dt) override;
    virtual void handleEvent(const sf::Event& event) override;
    virtual void draw(sf::RenderWindow& window) override;

    virtual unsigned int switchScreen() override;

private:

};

#endif

