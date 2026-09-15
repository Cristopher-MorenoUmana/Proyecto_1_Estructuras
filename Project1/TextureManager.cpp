#include "TextureManager.h"

TextureManager::TextureManager() {
    
    rectangleBlocks = {
        sf::IntRect({  0, 0 }, {19, 16}), // 0 - ROJO
        sf::IntRect({ 18, 0 }, {19, 16}), // 1 - VERDE
        sf::IntRect({ 36, 0 }, {19, 16}), // 2 - AZUL (Celeste)
        sf::IntRect({ 54, 0 }, {19, 16}), // 3 - MORADO
        sf::IntRect({ 72, 0 }, {19, 16}), // 4 - ROSA
        sf::IntRect({ 90, 0 }, {19, 16})  // 5 - AMARILLO
    };
}

bool TextureManager::load(const std::string& rutaArchivo) {
    return spriteSheet.loadFromFile(rutaArchivo);
}

sf::IntRect TextureManager::getRectangle(BlockColor color) const {
    return rectangleBlocks[static_cast<int>(color)];
}

const sf::Texture& TextureManager::getTexture() const {
    return spriteSheet;
}