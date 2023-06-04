#include <SFML/Graphics.hpp>

#include "Monster.h"
#include "Game.h"
#include "Menu.h"
#include "Button.h"
#include "worldCreator.h"
#include "Settings.h"
#include "Info.h"
#include "SubMenu.h"
#include <ctime>
int main(){
sf::RenderWindow window(sf::VideoMode(1600,1000),"Retro-Game");
Menu menu("font.ttf","texture.png");
SubMenu gameOverMenu("font.ttf", "texture.png", "game_over");  // Menu to be presented upon the player's death
SubMenu escKeyMenu("font.ttf", "texture.png", "escape_key");   // Menu to be presented upon pressing escape
sf::Clock clock;
sf::Time deltaTime;
sf::View viewGame;
sf::View viewUI;
Game game("texture.png", "font.ttf", &window, &viewUI);
int mode=1;
sf::Font font;
if(!font.loadFromFile("font.ttf")){}
Info info(font,"texture.png");
WorldCreator wCreator(font);
Settings settings(font);
window.setFramerateLimit(60);
sf::Vector2u windowSize=window.getSize();
   float idk=(float)windowSize.y/windowSize.x;
            float idk2=(1.f-idk)*0.5;
            viewUI.setViewport(sf::FloatRect(idk2, 0.f, idk, 1));
            viewGame.setViewport(sf::FloatRect(idk2, 0.f, idk, 1));
window.setView(viewUI);

bool escapeKeyPressed = false; // This will allow the event loop to update the escape menu screen

while(window.isOpen()){
    window.clear();
     sf::Vector2i windowPosition=sf::Mouse::getPosition(window);
     sf::Vector2f mousePos=window.mapPixelToCoords(windowPosition);
    sf::Event event;
    while(window.pollEvent(event)){
        if(event.type==sf::Event::Closed){
            window.close();
        }
        else if(event.type==sf::Event::Resized){
            sf::Vector2u windowSize=window.getSize();
            float idk=(float)windowSize.y/windowSize.x;
            float idk2=(1.f-idk)*0.5;
            viewUI.setViewport(sf::FloatRect(idk2, 0.f, idk, 1));
            viewGame.setViewport(sf::FloatRect(idk2, 0.f, idk, 1));
            window.setView(viewUI);
        }
        switch(mode){
        case 1:
                 menu.update(mousePos);
        if(event.type==sf::Event::MouseButtonPressed){
            if(menu.bExitGame.click(mousePos)){
                window.close();
                return 0;
            }
            if(menu.bNewGame.click(mousePos)) mode=2;
            if(menu.bDescription.click(mousePos)) mode=5;
            if(menu.bSettings.click(mousePos)) mode=4;
        }
        break;
        case 2:
              wCreator.update(mousePos);
            if(event.type==sf::Event::MouseButtonPressed){
                    wCreator.checkClick(mousePos);
                    if(wCreator.bStartGame.click(mousePos)){
                    game.changePlayerClass(wCreator.playerClass);
                    game.player->movementWSAD(settings.WSAD);
                    game.changeMap(wCreator.seed,wCreator.octaves, wCreator.bias);
                    mode=3;
                    wCreator.clearVectors();
                    }
                    if(wCreator.bGoBack.click(mousePos)){
                        mode=1;
                    }
            }
        break;
        case 3:
            if (event.type == sf::Event::KeyPressed) {

                // Sets the flag to present the ESC key menu if the player is not dead.
                if (event.key.code == sf::Keyboard::Escape && !game.player->isDead()) {
                    
                    if (!game.paused)
                        escapeKeyPressed = true;
                    else
                        escapeKeyPressed = false;

                    game.paused = !game.paused;
                }
            }
             if(event.type==sf::Event::TextEntered){
                }
            break;
        case 4:
            settings.update(mousePos);
            settings.checkClick(mousePos);

            if(settings.bGoBack.click(mousePos) && !escapeKeyPressed)
                mode = 1; // If the escape key had not been pressed, we were in the main menu
            if(settings.bGoBack.click(mousePos) && escapeKeyPressed)
                mode = 3; // If it had, we were in the escape key menu

        break;
        case 5:
            info.update(mousePos);
            info.checkClick(mousePos);
            if(info.goBack.click(mousePos)){
                mode=1;
            }
        break;
        }
    }
    switch (mode) {
    case 1:
        window.draw(menu);
        break;
    case 2:
        window.draw(wCreator);
        break;
    case 3:
        window.setView(viewGame);
        mousePos = window.mapPixelToCoords(windowPosition);
           
        game.update(deltaTime, mousePos);
        viewGame.setCenter(game.player->getCenter());
        window.draw(game);
        
        //  Menu to be presented upon the player's death
        if (game.player->isDead()) {

            game.paused = true;
            mousePos = window.mapPixelToCoords(windowPosition);
            gameOverMenu.update(mousePos);
            window.draw(gameOverMenu);

            if(event.type == sf::Event::MouseButtonPressed) {

                if (gameOverMenu.bExitGame.click(mousePos)) {
                    window.close();
                    return 0;
                }

                if (gameOverMenu.bMainMenu.click(mousePos)) {
                    mode = 1;
                    game.paused = false;

                    // Recreating the game object was the only way I found to reset the game.
                    Game game("texture.png", "font.ttf", &window, &viewUI);
                 }
            }
        }
        break;
    case 4:
        window.draw(settings);
    break;
    case 5:
        window.draw(info);
    break;
    }

    // Menu to be presented upon pressing escape
    if (escapeKeyPressed && mode == 3){

        game.paused = true;
        mousePos = window.mapPixelToCoords(windowPosition);
        escKeyMenu.update(mousePos);
        window.draw(escKeyMenu);

        if (event.type == sf::Event::MouseButtonPressed) {

            if (escKeyMenu.bResume.click(mousePos)) {
                game.paused = false;
                escapeKeyPressed = false;
            }
            if (escKeyMenu.bSettings.click(mousePos))
                mode = 4;

            if (escKeyMenu.bExitGame.click(mousePos)) {
                window.close();
                return 0;
            }

            if (escKeyMenu.bMainMenu.click(mousePos)) {
                mode = 1;
                game.paused = false;
                escapeKeyPressed = false;

                // Recreating the game object was the only way I found to reset the game.
                Game game("texture.png", "font.ttf", &window, &viewUI);
            }
        }
    }

    window.display();
    deltaTime=clock.restart();
}
return 0;
}
