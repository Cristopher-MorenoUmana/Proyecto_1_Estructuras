#include "Button.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Button::Button(
    const std::string& pNormalImageURL,
    const std::string& pHoverImageURL,
    sf::Vector2f pCoordinates
)
    : buttonSprite(normalTexture)
{
    bool normalLoaded = normalTexture.loadFromFile(pNormalImageURL);
    bool hoverLoaded = hoverTexture.loadFromFile(pHoverImageURL);

    if (!normalLoaded)
    {
        std::cerr << "No se pudo cargar la imagen normal: "
            << pNormalImageURL
            << std::endl;
    }

    if (!hoverLoaded)
    {
        std::cerr << "No se pudo cargar la imagen hover: "
            << pHoverImageURL
            << std::endl;
    }

    if (normalLoaded)
    {
        buttonSprite.setTexture(normalTexture, true);
    }

    buttonSprite.setPosition(pCoordinates);
}

bool Button::isOver(sf::Vector2f pMousePosition) const
{
    return buttonSprite.getGlobalBounds().contains(pMousePosition);
}

void Button::update(const sf::RenderWindow& pWindow)
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(pWindow);

    sf::Vector2f mousePositionFloat(
        static_cast<float>(mousePosition.x),
        static_cast<float>(mousePosition.y)
    );

    if (isOver(mousePositionFloat))
    {
        buttonSprite.setTexture(hoverTexture, true);
    }
    else
    {
        buttonSprite.setTexture(normalTexture, true);
    }
}

void Button::setCoordinates(sf::Vector2f pCoordinates)
{
    buttonSprite.setPosition(pCoordinates);
}

void Button::setScale(sf::Vector2f pScale)
{
    buttonSprite.setScale(pScale);
}

void Button::draw(sf::RenderWindow& pWindow) const
{
    pWindow.draw(buttonSprite);
}