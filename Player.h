#pragma once
#include<SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <iostream>
#include<cmath>
 
using namespace std;

class Player{
    private:
        const float START_SPEED = 200;
        const float START_HEALTH = 100;

        // Player speed in pixels per second
        float m_Speed;
        // How big is each tile of the screen
        int m_TileSize, m_Health, m_MaxHealth;

        // Players position, screen resolution
        sf::Vector2f m_Position, m_Resolution;

        sf::Texture m_Texture;
        sf::Sprite m_Sprite;

        // What size is the current arena
        sf::IntRect m_Arena;

        // Which direction the player is currently moving in
        bool m_UpPressed = false, m_DownPressed = false, m_RightPressed = false, m_LeftPressed = false;

        // When was player last hit
        sf::Time m_LastHit;

    public:
        Player() :
            m_Speed(START_SPEED),
            m_Health(START_HEALTH),
            m_MaxHealth(START_HEALTH),
            m_Texture("graphics/player.png"),
            m_Sprite(m_Texture)
            {
                m_Sprite.setOrigin({25, 25});
            }

        // Spawn the player in deadly zombie arena
        void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize){
            // Place the player in the center of the arena
            m_Position.x = (arena.position.x + arena.size.x)/2.0f;
            m_Position.y = (arena.position.y + arena.size.y)/2.0f;

            m_Sprite.setPosition(m_Position);

            m_Arena.position = arena.position;
            m_Arena.size = arena.size;

            // Remember how big the tiles are in this arena
            m_TileSize = tileSize;

            m_Resolution.x = resolution.x;
            m_Resolution.y = resolution.y;
        }

        // Call this at the end of the game
        void resetPlayerStats(){
            m_Speed = START_SPEED;
            m_Health = START_HEALTH;
            m_MaxHealth = START_HEALTH;
        }

        // Handle the player get hit by zombie
        bool hit(sf::Time timeHit){
            if(timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200){
                m_LastHit = timeHit;
                m_Health -= 10;
                return true;
            }
            return false;
        }

        // How long ago was the player hit
        sf::Time getLastHitTime(){
            return m_LastHit;
        }

        sf::FloatRect getPosition(){
            return m_Sprite.getGlobalBounds();
        }

        sf::Vector2f getCenter(){
            return m_Position;
        }

        // What angle player is facing
        sf::Angle getRotation(){
            return m_Sprite.getRotation();
        }

        // Send a copy of the sprite to the main function
        sf::Sprite getSprite(){
            return m_Sprite;
        }

        void moveLeft(){
            m_LeftPressed = true;
        }

        void moveRight(){
            m_RightPressed = true;
        }

        void moveDown(){
            m_DownPressed = true;
        }

        void moveUp(){
            m_UpPressed = true;
        }

        void stopLeft(){
            m_LeftPressed = false;
        }

        void stopRight(){
            m_RightPressed = false;
        }

        void stopUp(){
            m_UpPressed = false;
        }

        void stopDown(){
            m_DownPressed = false;
        }

        // We will call this function once every frame
        void update(sf::Time dt, sf::Vector2i mousePosition){
            if(m_UpPressed){
                m_Position.y -= m_Speed * dt.asSeconds();
            }
            if(m_DownPressed){
                m_Position.y += m_Speed * dt.asSeconds();
            }
            if(m_LeftPressed){
                m_Position.x -= m_Speed * dt.asSeconds();
            }
            if(m_RightPressed){
                m_Position.x += m_Speed * dt.asSeconds();
            }
            // Update the player's position according to the speed and dt
            if(m_Position.x > (m_Arena.position.x + m_Arena.size.x) - m_TileSize){
                // Right Bound
                m_Position.x = m_Arena.position.x + m_Arena.size.x - m_TileSize;
            }
            if(m_Position.x < m_Arena.position.x + m_TileSize){
                // Left Bound
                m_Position.x = m_Arena.position.x + m_TileSize;
            }
            if(m_Position.y > (m_Arena.position.y + m_Arena.size.y) - m_TileSize){
                // Down Bound
                m_Position.y = m_Arena.position.y + m_Arena.size.y - m_TileSize;
            }
            if(m_Position.y < m_Arena.position.y + m_TileSize){
                // Upper Bound
                m_Position.y = m_Arena.position.y + m_TileSize;
            }
            m_Sprite.setPosition(m_Position);
            float radians = std::atan2(
                static_cast<float>(mousePosition.y) - m_Resolution.y / 2.0f,
                static_cast<float>(mousePosition.x) - m_Resolution.x / 2.0f
            );
            sf::Angle angle = sf::radians(radians);
            m_Sprite.setRotation(angle);
        }

        // Give the player a boost
        void upgradeSpeed(){
            // Increase the speed by 20%
            m_Speed += (0.2*START_SPEED);
        }

        // Give the player some health
        void upgradeHealth(){
            // Increase the health by 20%
            m_MaxHealth += (0.2*START_HEALTH);
        }

        // Increase the amount of health the player can have
        void increaseHealthLevel(int amount){
            m_Health = min(m_MaxHealth, m_Health + amount);
        }

        // How much health the player has currently got
        int getHealth(){
            return m_Health;
        }

};