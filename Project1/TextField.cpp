#include "TextField.h"

TextField::TextField()
    : isFocused(false), maxLength(12) {}

void TextField::init(const sf::Font& font, sf::Vector2f pos, sf::Vector2f size) {
    box.setPosition(pos);
    box.setSize(size);
    box.setFillColor(sf::Color(30, 30, 45, 230));
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color::White);

    // Instanciamos el sf::Text pasándole la fuente obligatoria de SFML 3
    text.emplace(font, "", 18);
    text->setFillColor(sf::Color::Yellow);
    text->setPosition({ pos.x + 10.f, pos.y + 5.f });
}

void TextField::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButton->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            isFocused = box.getGlobalBounds().contains(mousePos);

            if (isFocused) {
                box.setOutlineColor(sf::Color::Yellow);
            }
            else {
                box.setOutlineColor(sf::Color::White);
            }
        }
    }

    if (isFocused) {
        if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
            uint32_t unicode = textEntered->unicode;

            // Tecla Backspace (Borrar)
            if (unicode == 8) {
                if (!inputString.empty()) {
                    inputString.pop_back();
                }
            }
            // Caracteres imprimibles sin espacios
            else if (unicode >= 33 && unicode <= 126) {
                if (inputString.length() < maxLength) {
                    inputString += static_cast<char>(unicode);
                }
            }

            if (text.has_value()) {
                text->setString(inputString);
            }
        }
    }
}

void TextField::draw(sf::RenderWindow& window) const {
    window.draw(box);
    if (text.has_value()) {
        window.draw(*text);
    }
}

std::string TextField::getText() const {
    return inputString;
}

void TextField::setText(const std::string& str) {
    inputString = str;
    if (text.has_value()) {
        text->setString(inputString);
    }
}