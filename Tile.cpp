
#include "Tile.h"

/**
 * @brief Construct a new Tile object from letter and value.
 */
Tile::Tile(Letter letter, Value value) :
    letter(letter),
    value(value) {
}

/**
 * @brief Construct a new Tile object, copying other Tile.
 * @param other Tile to be copied.
 */
Tile::Tile(const Tile& other) :
    letter(other.letter),
    value(other.value) {
}

/**
 * @brief Gets tile letter.
 */
Letter Tile::getLetter() {
    return letter;
}

/**
 * @brief Gets tile value.
 */
Value Tile::getValue() {
    return value;
}