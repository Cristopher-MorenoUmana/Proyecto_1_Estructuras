#include "Button.h"

Button::Button(const std::string& pImageURL, sf::Vector2f pCoordinates)
    : buttonTexture(pImageURL),
    buttonSprite(buttonTexture),
    imageURL(pImageURL)
{
    buttonSprite.setPosition(pCoordinates);
}

bool Button::isOver(sf::RenderWindow& pWindow)
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(pWindow);

    sf::Vector2f mousePositionFloat(
        static_cast<float>(mousePosition.x),
        static_cast<float>(mousePosition.y)
    );

    return buttonSprite.getGlobalBounds().contains(mousePositionFloat);
}

bool Button::onClick(sf::RenderWindow& pWindow)
{
    return isOver(pWindow) &&
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

std::string Button::getImageURL() const
{
    return imageURL;
}

void Button::setImageURL(const std::string& pImageURL)
{
    imageURL = pImageURL;
    buttonTexture.loadFromFile(imageURL);
    buttonSprite.setTexture(buttonTexture, true);
}

void Button::setCoordinates(sf::Vector2f pCoordinates)
{
    buttonSprite.setPosition(pCoordinates);
}

void Button::setScale(sf::Vector2f pScale)
{
    buttonSprite.setScale(pScale);
}

void Button::draw(sf::RenderWindow& pWindow)
{
    pWindow.draw(buttonSprite);
}