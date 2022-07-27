
#include "LinkedList.h"

/**
 * @brief Construct a new empty Linked List.
 */
LinkedList::LinkedList() {
   head = nullptr;
   tail = nullptr;
}

/**
 * @brief Destroy the Linked List object.
 */
LinkedList::~LinkedList() {
   delete head;
}

/**
 * @brief Adds tile to back of list and assumes memory ownership of it.
 * @param tile Tile to be appended to list.
 */
void LinkedList::add(Tile* tile) {
   if (head == nullptr) {
      // If no nodes in list
      head = new Node(tile);
      tail = head;
   }
   else {
      // If at least 1 node in list
      tail->next = new Node(tile);
      tail = tail->next;
   }
}

/**
 * @brief Prints the linked list in letter-value pairs.
 */
void LinkedList::printList() {
   Node* point = head;
   while (point != nullptr) {
      Tile* tempTile = point->getTileCopy();
      std::cout << tempTile->getLetter() << "-" << tempTile->getValue();
      std::cout << " ";
      point = point->next;
      delete tempTile;
   }
}

/**
 * @brief Return and remove first tile in list, nullptr if list empty.
 * @return Tile* Pointer to first tile.
 */
Tile* LinkedList::drawTile() {
   Tile* tile;
   if (head == nullptr) {
      // If list empty, return null pointer
      tile = nullptr;
   }

   if (head == tail) {
      // If 1 element, make empty and return head element
      Tile* tempTilePtr = head->getTile();
      delete head;
      head = tail = nullptr;

      tile = tempTilePtr;
   }
   else {
      // If more than 1 element, remove and return first element
      Tile* tempTilePtr = head->getTile();
      Node* tempNodePtr = head;

      head = head->next;   // Move head

      // Delete previous head
      tempNodePtr->next = nullptr;
      delete tempNodePtr;

      tile = tempTilePtr;
   }
   return tile;
}

/**
 * @brief Remove and return first tile with specified letter, nullptr if not found.
 * @param letter Letter of tile to remove from list.
 * @return Tile* Pointer to removed tile.
 */
Tile* LinkedList::removeLetter(Letter letter) {
   Tile* tile;
   if (head == nullptr) {
      // Check that list not empty
      tile = nullptr;
   }
   else if (head->getTileLetter() == letter) {
      // Check if head has letter
      tile = drawTile(); // Easy way to remove first node and return
   }
   else {
      tile = removeRecursive(letter, head);
   }
   return tile;
}

/**
 * @brief Recursively removes node with matching letter, returning tile.
 * @param letter Letter of tile to remove from list.
 * @param prevNode Previous node to the one which currently being checked.
 * @return Tile* Pointer to removed tile (nullptr if not found).
 */
Tile* LinkedList::removeRecursive(Letter letter, Node* prevNode) {
   Tile* tile;
   // prevNode is the previous node to the one being checked (so we can delete next)
   if (prevNode->next == nullptr) {
      // If at end of list, return not found
      tile = nullptr;
   }
   else if (prevNode->next->getTileLetter() == letter) {
      // If next node has letter, remove it and return copy of it
      Tile* tempTilePtr = prevNode->next->getTile();
      Node* tempNodePtr = prevNode->next;

      // Remove node from linkedlist
      prevNode->next = prevNode->next->next;
      if (prevNode->next == nullptr) {
         // If prevNode now at end, set tail
         tail = prevNode;
      }

      // Clear memory of node
      tempNodePtr->next = nullptr;
      delete tempNodePtr;

      // Return tile
      tile = tempTilePtr;
   }
   else {
      // Next node doesn't have letter, continue recursive
      tile = removeRecursive(letter, prevNode->next);
   }
   return tile;
}

/**
 * @brief Returns true if the letter is in the list, false otherwise.
 * @param letter Letter to check for.
 * @return true Letter in list.
 * @return false Letter not in list.
 */
bool LinkedList::inList(Letter letter) {
   return inListFromNode(letter, head);
}

/**
 * @brief Returns true if the letter is in the list starting from curNode, false otherwise.
 * @param letter Letter to check for.
 * @param curNode The node to start searching from.
 * @return true Letter in list from curNode.
 * @return false Letter not in list from curNode.
 */
bool LinkedList::inListFromNode(Letter letter, Node* curNode) {
   bool isInList;
   if (curNode == nullptr) {
      isInList = false;
   }
   else if (curNode->getTileLetter() == letter) {
      isInList = true;
   }
   else {
      isInList = inListFromNode(letter, curNode->next);
   }
   return isInList;
}

/**
 * @brief Gets a string of all of the letters in the linked list.
 * @return std::string String of nodes in list.
 */
std::string LinkedList::getNodeStrings() {
   std::string ret = "";
   Node* point = head;
   while (point != nullptr) {
      Tile* tempTile = point->getTileCopy();
      ret += (char)tempTile->getLetter();
      ret += "-";
      // std::cout << tempTile->getValue() << " " << std::to_string(tempTile->getValue()) << std::endl;
      ret += std::to_string(tempTile->getValue());
      ret += " ";
      point = point->next;
      delete tempTile;
   }
   return ret;
}