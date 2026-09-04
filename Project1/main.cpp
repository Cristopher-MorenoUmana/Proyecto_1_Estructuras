#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML works!");
    
    sf::RectangleShape shape(sf::Vector2f(100, 50));
    shape.setFillColor(sf::Color::Cyan);
    shape.setOutlineColor(sf::Color::White);
    sf::Texture texture;

    if (!texture.loadFromFile("assets/bloques_De_Colores.png")) {
        return -1;
    }

    sf::Sprite sprite(texture);
    sprite.setScale(sf::Vector2f({2,2}));
    

    
    const std::vector<sf::IntRect> bloques = {
    sf::IntRect({  0, 0 }, {19, 16}), // 0 - rojo
    sf::IntRect({ 18, 0 }, {19, 16}), // 1 - verde
    sf::IntRect({ 36, 0 }, {19, 16}), // 2 - celeste
    sf::IntRect({ 54, 0 }, {19, 16}), // 3 - morado
    sf::IntRect({ 72, 0 }, {19, 16}), // 4 - rosa
    sf::IntRect({ 90, 0 }, {19, 16}), // 5 - amarillo
    };

    sprite.setTextureRect(sf::IntRect(bloques[5]));
   

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(128, 128, 128));
        window.draw(sprite);
        window.display();
    }
}