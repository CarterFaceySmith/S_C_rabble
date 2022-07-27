
#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include <memory>
#include "Tile.h"

using std::shared_ptr;
using std::make_shared;

class Node {
public:

   Node(Tile* tile);
   Node(Tile* tile, Node* next);
   ~Node();
   Node(const Node& other);
   Node* next;

   Tile* getTile();
   Tile* getTileCopy();
   Letter getTileLetter();
private:
   Tile* tile;
};

#endif // ASSIGN2_NODE_H
