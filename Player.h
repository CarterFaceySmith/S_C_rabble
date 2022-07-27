#ifndef PLAYER_H
#define PLAYER_H

#include "LinkedList.h"
#include "Tile.h"
#include <string>
#include <vector>

#define HAND_SIZE 7

class Player {
public:
   Player(std::string name);
   Player(std::string name, int score, std::string tiles);
   ~Player();
   void draw(LinkedList* tilebag);
   void printHand();
   LinkedList* getHand();
   void printScore();
   void declareTurn();
   void addScore(int points);
   int getScore();
   std::string getName();
   std::string getTiles();
   void incrementPass();
   int getPass();
   void clearPass();

private:
   std::string name;
   int score;
   LinkedList* hand;
   int passCount;
};

#endif // PLAYER_H
