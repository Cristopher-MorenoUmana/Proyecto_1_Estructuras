#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "BlockColor.h"

class TextureManager {
private:
    sf::Texture spriteSheet;
    std::vector<sf::IntRect> rectangleBlocks;

public:
    TextureManager();
    bool load(const std::string& rutaArchivo);
    sf::IntRect getRectangle(BlockColor color) const;

    const sf::Texture& getTexture() const;
};

#endif 

