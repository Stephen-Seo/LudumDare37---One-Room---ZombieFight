
#ifndef LD37_GAME_HPP
#define LD37_GAME_HPP

#define GAME_TITLE_NAME "LudumDare 37 - One Room - Zombie Fight!!"

#include <SFML/Graphics.hpp>

#include "Screen.hpp"

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    Screen::Ptr currentScreen;
    bool runFlag;
    sf::RenderWindow window;

};

#endif

