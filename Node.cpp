
#include "Node.h"

/**
 * @brief Construct a new Node object.
 * @param tile Pointer to tile to be placed in node.
 */
Node::Node(Tile* tile) {
   this->tile = tile;
   this->next = nullptr;
}

/**
 * @brief Construct a new Node object.
 * @param tile Pointer to tile to be placed in node.
 * @param next Pointer to next node in list.
 */
Node::Node(Tile* tile, Node* next) {
   this->tile = tile;
   this->next = next;
}

/**
 * @brief Destroy the Node object.
 */
Node::~Node() {
   delete tile;
   delete next;
}

/**
 * @brief Construct a new Node object by copying other.
 * @param other Node to be copied.
 */
Node::Node(const Node& other) {
   this->tile = new Tile(*(other.tile));
   this->next = new Node(*(other.next));
}

/**
 * @brief Returns and removes the node's tile.
 * Passes memory ownership as well.
 * @return Tile* The node's tile.
 */
Tile* Node::getTile() {
   Tile* tempPtr = this->tile;
   this->tile = nullptr;
   return tempPtr;
}

/**
 * @brief Returns copy of the node's tile.
 * @return Tile* Copy of node's tile.
 */
Tile* Node::getTileCopy() {
   return new Tile(*(this->tile));
}

/**
 * @brief Returns the letter of the nodes tile.
 * @return Letter The tile's letter.
 */
Letter Node::getTileLetter() {
   return tile->getLetter();
}
