#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>
#include "BlockColor.h"

class Block {
private:
    sf::Sprite sprite;
    BlockColor color;

public:
    Block(const sf::Texture& pTexture, const sf::IntRect& pRectangle, BlockColor pColor);

    void setPosition(sf::Vector2f pos);
    void setScale(sf::Vector2f factor);
    void draw(sf::RenderWindow& window);
};

#endif // !BLOCK_H
