
#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

// Define a Letter type
typedef char Letter;

// Define a Value type
typedef int Value;

#define STRING_CHAR_LOCATION 0
#define STRING_VALUE_LOCATION 2

class Tile {
public:
   Tile(Letter letter, Value value);
   Tile(const Tile& other);
   Letter getLetter();
   Value getValue();

private:
   Letter letter;
   Value  value;
};

#endif // ASSIGN2_TILE_H
