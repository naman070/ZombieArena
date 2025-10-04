#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include <set>
#include <iostream>

using namespace std;

// The Game will always be in 1 of the 4 states.
enum class State{
    PAUSED, LEVELING_UP, GAME_OVER, PLAYING
};

const set<sf::Keyboard::Key> NUM_KEYS = {
    sf::Keyboard::Key::Num1,
    sf::Keyboard::Key::Num2,
    sf::Keyboard::Key::Num3,
    sf::Keyboard::Key::Num4,
    sf::Keyboard::Key::Num5,
    sf::Keyboard::Key::Num6
};

int main(){
    // Start with GAME_OVER state
    State state = State::GAME_OVER;

    sf::Vector2u desktopSize = sf::VideoMode::getDesktopMode().size;
    sf::Vector2f resolution (desktopSize);
    sf::RenderWindow window(sf::VideoMode(desktopSize), "ZombieArena");

    // Create a SFML view for the main action
    sf::View mainView(sf::Vector2f ({0, 0}), resolution);

    sf::Clock clock;

    // Track the mouse position coordinates in relation to world coordinates
    sf::Vector2f mouseWorldPosition;

    // Track the mouse position coordinates in relation to screen coordinates
    sf::Vector2i mouseScreenPosition;

    // Create a player
    Player player;

    // Create the background and then load the texture in the background VA
    sf::VertexArray background;
    sf::Texture textureBackground;
    (void)textureBackground.loadFromFile("graphics/background_sheet.png");

    sf::Time gameTimeTotal;

    // Arena boundary
    sf::IntRect arena;

    // The main game loop
    while(window.isOpen()){
        /*
        ------------------------------------------
            Handle Input
        ------------------------------------------
        */
        while(auto event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()) window.close();
            if(auto keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if(keyPressed->code == sf::Keyboard::Key::Escape) window.close();
                else if(keyPressed->code == sf::Keyboard::Key::Enter){
                    if(state == State::PLAYING) state = State::PAUSED;
                    else if(state == State::PAUSED){
                        state = State::PLAYING;
                        // Reset the clock so there isn't a frame jump
                        clock.restart();
                    }
                    // Start a new game while in game over state
                    else if(state == State::GAME_OVER) state = State::LEVELING_UP;
                }
                if(state == State::PLAYING){
                    if(keyPressed->code == sf::Keyboard::Key::W) player.moveUp();
                    else if(keyPressed->code == sf::Keyboard::Key::S) player.moveDown();
                    else if(keyPressed->code == sf::Keyboard::Key::A) player.moveLeft();
                    else if(keyPressed->code == sf::Keyboard::Key::D) player.moveRight();
                            
                }
                if(state == State::LEVELING_UP && NUM_KEYS.find(keyPressed->code) != NUM_KEYS.end()){
                    state = State::PLAYING;
                    // Prepare the level
                    // arena.size = sf::Vector2i(desktopSize);
                    arena.size = {500, 500};
                    arena.position = {0, 0};

                    // Create our background vertex array
                    int tileSize = createBackground(background, arena);
                    // Spawn the player in the middle of the arena
                    player.spawn(arena, resolution, tileSize);
                    mainView.setCenter(player.getCenter());

                    // restart the clock to avoid the frame jump
                    clock.restart();
                }
            }
            if(auto keyReleased = event->getIf<sf::Event::KeyReleased>()){
                if(state == State::PLAYING){
                    if(keyReleased->code == sf::Keyboard::Key::W) player.stopUp();
                    else if(keyReleased->code == sf::Keyboard::Key::S) player.stopDown();
                    else if(keyReleased->code == sf::Keyboard::Key::A) player.stopLeft();
                    else if(keyReleased->code == sf::Keyboard::Key::D) player.stopRight();
                }
            }
        }
        
        window.clear();
        if(state == State::PLAYING){
            sf::Time dt = clock.restart();
            // Reset the clock so that there isn't a frame jump
            gameTimeTotal += dt;

            mouseScreenPosition = sf::Mouse::getPosition();
            // Convert mouse position to world coordinates of world view
            mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);
            player.update(dt, mouseScreenPosition);

            window.setView(mainView);
            window.draw(background, &textureBackground);
            window.draw(player.getSprite());
        }
        window.display();
    }
}