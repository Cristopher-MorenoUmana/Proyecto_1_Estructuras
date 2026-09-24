#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

class TextField {
private:
    sf::RectangleShape box;
    std::optional<sf::Text> text; // std::optional permite crearlo antes de tener la fuente
    std::string inputString;
    bool isFocused;
    std::size_t maxLength;

public:
    TextField();

    void init(const sf::Font& font, sf::Vector2f pos, sf::Vector2f size);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    std::string getText() const;
    void setText(const std::string& str);
};