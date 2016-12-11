
#include "Game.hpp"

#include "RoomScreen.hpp"

#include <GDT/GameLoop.hpp>

Game::Game() :
runFlag(true),
window(sf::VideoMode(800, 450), GAME_TITLE_NAME, sf::Style::Close | sf::Style::Titlebar)
{
//    window.setFramerateLimit(60);
    currentScreen = std::make_unique<RoomScreen>();
}

Game::~Game()
{
}

void Game::run()
{
    auto update = [this] (float dt) {
        currentScreen->update(dt, window);

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                runFlag = false;
            }
            else
            {
                currentScreen->handleEvent(event);
            }
        }

        switch(currentScreen->switchScreen())
        {
        case 1:
            currentScreen = std::make_unique<RoomScreen>();
            break;
        default:
            break;
        }
    };

    auto draw = [this] () {
        window.clear(sf::Color(30, 30, 30));

        currentScreen->draw(window);

        window.display();
    };

    GDT::IntervalBasedGameLoop(&runFlag,
        update,
        draw,
        60);

    window.close();
}

