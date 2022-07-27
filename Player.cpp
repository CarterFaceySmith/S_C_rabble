
#include "Player.h"

/**
 * @brief Construct a new Player object with name inputted.
 * @param name Player name.
 */
Player::Player(std::string name) {
   this->name = name;
   this->score = 0;
   this->hand = new LinkedList();
   this->passCount = 0;
}

/**
 * @brief Construct a new Player object from a saved state.
 * @param name Player name.
 * @param score Player's score.
 * @param tiles Tiles to be added to the player's linked list.
 */
Player::Player(std::string name, int score, std::string tiles) {
   this->name = name;
   this->score = score;
   this->hand = new LinkedList();

   std::vector<std::string> words;

   // Failsafe, just make sure nothing important gets deleted
   std::string tilesCopy = tiles;

   size_t pos = 0;
   std::string delim = " ";

   while ((pos = tilesCopy.find(delim)) != std::string::npos) {
      words.push_back(tilesCopy.substr(0, pos));
      tilesCopy.erase(0, pos + delim.length());
   }
   for (const auto& str : words) {
      Tile* tile = new Tile(str[STRING_CHAR_LOCATION], std::stoi(str.substr(STRING_VALUE_LOCATION, str.length() - 1)));
      this->hand->add(tile);
   }
}

/**
 * @brief Destroy the Player object.
 */
Player::~Player() {
   delete this->hand;
}

/**
 * @brief Prints turn declaration.
 */
void Player::declareTurn() {
   std::cout << this->name << ", it's your turn" << std::endl;
}

/**
 * @brief Prints the player's hand.
 */
void Player::printHand() {
   this->hand->printList();
}

/**
 * @brief Draws a card from tilebag and adds it to player's hand.
 * @param tilebag Tile bag to draw from.
 */
void Player::draw(LinkedList* tilebag) {
   this->hand->add(tilebag->drawTile());
}

/**
 * @brief Increments passCount by 1.
 */
void Player::incrementPass() {
   this->passCount += 1;
}

/**
 * @brief Getter for passCount.
 */
int Player::getPass() {
   return this->passCount;
}

/**
 * @brief Set passCount to 0.
 */
void Player::clearPass() {
   this->passCount = 0;
}

/**
 * @brief Returns pointer to player's hand linked list.
 */
LinkedList* Player::getHand() {
   return this->hand;
}

/**
 * @brief Prints player's score.
 */
void Player::printScore() {
   std::cout << "Score for " << this->name << ": " << this->score << std::endl;
}

/**
 * @brief Add points to score.
 */
void Player::addScore(int points) {
   this->score += points;
}

/**
 * @brief Getter for score. 
 */
int Player::getScore() {
   return this->score;
}

/**
 * @brief Getter for name. 
 */
std::string Player::getName() {
   return this->name;
}

/**
 * @brief Gets string representing all tiles in hand.
 */
std::string Player::getTiles() {
   return this->hand->getNodeStrings();
}