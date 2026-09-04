#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(const std::string& pImageURL, sf::Vector2f pCoordinates);

    bool onClick(sf::RenderWindow& pWindow);
    bool isOver(sf::RenderWindow& pWindow);

    std::string getImageURL() const;
    void setImageURL(const std::string& pImageURL);

    void setCoordinates(sf::Vector2f pCoordinates);
    void setScale(sf::Vector2f pScale);

    void draw(sf::RenderWindow& pWindow);

private:
    sf::Texture buttonTexture;
    sf::Sprite buttonSprite;
    std::string imageURL;
};

#endif