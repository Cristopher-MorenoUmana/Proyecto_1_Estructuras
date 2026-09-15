#include "Block.h"

Block::Block(const sf::Texture& pTexture, const sf::IntRect& pRectangle, BlockColor pColor)
    : sprite(pTexture), color(pColor)
{
    sprite.setTextureRect(pRectangle);
}

void Block::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

void Block::setScale(sf::Vector2f factor) {
    sprite.setScale(factor);
}

void Block::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}