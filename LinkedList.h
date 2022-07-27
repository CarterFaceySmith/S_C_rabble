
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include "Tile.h"
#include <string>
#include <iostream>

class LinkedList
{
public:
   LinkedList();
   ~LinkedList();
   void add(Tile* tile);
   void printList();

   Tile* drawTile();

   bool inList(Letter letter);

   Tile* removeLetter(Letter letter);

   std::string getNodeStrings();
private:
   Tile* removeRecursive(Letter letter, Node* prevNode);
   bool inListFromNode(Letter letter, Node* curNode);

   Node* head;
   Node* tail;
};

#endif // ASSIGN2_LINKEDLIST_H
