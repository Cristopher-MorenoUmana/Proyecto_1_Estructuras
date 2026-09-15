#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Button
{
public:
    Button(
        const std::string& pNormalImageURL,
        const std::string& pHoverImageURL,
        sf::Vector2f pCoordinates
    );

    bool isOver(sf::Vector2f pMousePosition) const;

    void update(const sf::RenderWindow& pWindow);

    void setCoordinates(sf::Vector2f pCoordinates);
    void setScale(sf::Vector2f pScale);

    void draw(sf::RenderWindow& pWindow) const;

private:
    sf::Texture normalTexture;
    sf::Texture hoverTexture;

    sf::Sprite buttonSprite;
};

#endif