#include "ZombieArena.h"
#include "RandomGenerator.h"
#include<SFML/Graphics.hpp>

int createBackground(sf::VertexArray &rVA, sf::IntRect arena){
    // How big is each tile/texture
    const int TILE_SIZE = 50;
    const int TILE_TYPES = 3;
    const int VERTS_IN_QUAD = 6;

    int worldWidth = arena.size.x / TILE_SIZE;
    int worldHeight = arena.size.y / TILE_SIZE;

    rVA.setPrimitiveType(sf::PrimitiveType::Triangles);
    rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

    // Start at the beginning of the vertex array
    int currentVertex = 0;

    for(int w = 0; w < worldWidth; w++){
        for(int h = 0; h < worldHeight; h++){
            // Define the 4 corners
            sf::Vector2f topLeft (w * TILE_SIZE, h * TILE_SIZE);
            sf::Vector2f bottomLeft (w * TILE_SIZE, TILE_SIZE + (h * TILE_SIZE));
            sf::Vector2f topRight (TILE_SIZE + (w * TILE_SIZE), h * TILE_SIZE);
            sf::Vector2f bottomRight (TILE_SIZE + (w * TILE_SIZE), TILE_SIZE + (h * TILE_SIZE));

            // First triangle (top-left, bottom-left, bottom-right)
            rVA[currentVertex].position = topLeft;
            rVA[currentVertex + 1].position = bottomLeft;
            rVA[currentVertex + 2].position = bottomRight;

            // Second triangle (top-left, top-right, bottom-right)
            rVA[currentVertex + 3].position = topLeft;
            rVA[currentVertex + 4].position = topRight;
            rVA[currentVertex + 5].position = bottomRight;

            // Texture coordinates
            sf::Vector2f texTopLeft, texTopRight, texBottomRight, texBottomLeft;

            // For the boundary use the wall texture
            if(h == 0 || w == 0 || h == worldHeight - 1 || w == worldWidth - 1){
                int verticalOffset = TILE_TYPES * TILE_SIZE;
                texTopLeft = sf::Vector2f(0, verticalOffset);
                texTopRight = sf::Vector2f(TILE_SIZE, verticalOffset);
                texBottomRight = sf::Vector2f(TILE_SIZE, verticalOffset + TILE_SIZE);
                texBottomLeft = sf::Vector2f(0, verticalOffset + TILE_SIZE);
            }
            else{
                // Use a random floor texture: grass, stone or bush
                int num = RandomGenerator::getInstance().getInt(0, TILE_TYPES-1);
                int verticalOffset = num * TILE_SIZE;
                texTopLeft = sf::Vector2f(0, verticalOffset);
                texTopRight = sf::Vector2f(TILE_SIZE, verticalOffset);
                texBottomRight = sf::Vector2f(TILE_SIZE, verticalOffset + TILE_SIZE);
                texBottomLeft = sf::Vector2f(0, verticalOffset + TILE_SIZE);
            }
            // Apply texture coords to both triangles
            // First triangle
            rVA[currentVertex].texCoords = texTopLeft;
            rVA[currentVertex + 1].texCoords = texBottomLeft;
            rVA[currentVertex + 2].texCoords = texBottomRight;

            // Second triangle
            rVA[currentVertex + 3].texCoords = texTopLeft;
            rVA[currentVertex + 4].texCoords = texTopRight;
            rVA[currentVertex + 5].texCoords = texBottomRight;

            currentVertex += VERTS_IN_QUAD;
        }
    }
    return TILE_SIZE;
}