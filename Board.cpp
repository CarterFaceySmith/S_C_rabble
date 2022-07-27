#include "Board.h"
#include <string>
using std::string;

/**
 * @brief Construct a new empty Board object.
 */
Board::Board() {
   this->tilebag = new LinkedList();
   for (int i = 0; i < DIM; i++) {
      std::vector<Tile*> row;
      for (int j = 0; j < DIM; j++) {
         row.push_back(nullptr);
      }
      this->board.push_back(row);
   }
}

/**
 * @brief Construct a new Board object with tilebag and board already made.
 */
Board::Board(std::string tilebag, std::vector<std::string> board) {
   this->tilebag = new LinkedList();

   std::vector<std::string> words;
   std::string tilebagCopy = tilebag;
   size_t pos = 0;
   std::string delim = " ";
   while ((pos = tilebagCopy.find(delim)) != std::string::npos) {
      words.push_back(tilebagCopy.substr(0, pos));
      tilebagCopy.erase(0, pos + delim.length());
   }

   for (std::string str : words) {
      Tile* tile = new Tile(str[STRING_CHAR_LOCATION], std::stoi(str.substr(STRING_VALUE_LOCATION, (str.length() - 1))));
      this->tilebag->add(tile);
   }

   for (const auto& line : board) {
      std::string line_copy = line;
      std::vector<std::string> sep;
      std::vector<Tile*> boardTiles;
      size_t pos = 0;
      std::string delim = "|";
      while ((pos = line_copy.find(delim)) != std::string::npos) {
         sep.push_back(line_copy.substr(0, pos));
         line_copy.erase(0, pos + delim.length());
      }
      for (const auto& str : sep) {
         std::string space = " ";
         if (str == space) {
            boardTiles.push_back(nullptr);
         }
         else {
            // Score of the tiles is put to 0 on placed tiles for simplicity
            boardTiles.push_back(new Tile(str[STRING_CHAR_LOCATION], 0));
         }
      }
      this->board.push_back(boardTiles);
   }
}

/**
 * @brief Destroy the Board object
 */
Board::~Board() {
   delete this->tilebag;
   for (int i = 0; i < DIM; i++) {
      for (int j = 0; j < DIM; j++) {
         delete this->board[i][j];
      }
   }
}

/**
 * @brief Reads tiles from ScrabbleTiles.txt and randomizes their order, putting them into tilebag.
 */
void Board::buildBag() {
   //Seed set as 0 for new game diff check to function
   srand(time(0));
   char letter[MAX_ARRAY_SIZE];
   int value[MAX_ARRAY_SIZE];
   int random = 0;
   std::ifstream tiles("ScrabbleTiles.txt");
   for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
      tiles >> letter[i] >> value[i];
   }
   for (int j = 0; j < MAX_ARRAY_SIZE - 1; j++) {
      random = j + (rand() % (MAX_ARRAY_SIZE - j));
      Letter swapLetter = letter[j];
      letter[j] = letter[random];
      letter[random] = swapLetter;
      int swapValue = value[j];
      value[j] = value[random];
      value[random] = swapValue;
   }
   for (int k = 0; k < MAX_ARRAY_SIZE; k++) {
      Tile* tile = new Tile(letter[k], value[k]);
      if ((tile->getLetter()) && tile->getValue() != 0) {
         this->tilebag->add(tile);
      }
      else {
         delete tile;
      }

   }
   tiles.close();
}

/**
 * @brief Make player draw 7 cards, for starting game.
 */
void Board::buildHand(Player* player) {
   for (int i = 0; i < HAND_SIZE; i++) {
      player->draw(tilebag);
   }
}

/**
 * @brief Swaps a tile in the player's hand with one in the tile bag.
 * @return true Tile replaced succesfully.
 * @return false No tiles in tile bag.
 */
bool Board::replaceWithTileFromBag(Player* player, Letter letter) {
   bool ret = true;
   Tile* tileFromBag = this->tilebag->drawTile();
   if (tileFromBag == nullptr) {
      std::cout << "There exists no tiles for replacement" << std::endl;
      ret = false;
   }
   else {
      Tile* tileFromPlayer = player->getHand()->removeLetter(letter);
      player->getHand()->add(tileFromBag);
      this->tilebag->add(tileFromPlayer);
   }
   return ret;
}

/**
 * @brief Prints the board.
 */
void Board::print() {
   std::cout << "   ";
   for (int i = 0; i <= 9; ++i) {
      std::cout << "  " << i << " ";
   }
   std::cout << "  10  11  12  13  14";
   std::cout << std::endl;
   std::string line(BOARD_LINE_SIZE, '-');
   std::cout << "   " << line << std::endl;

   for (int row = 0; row < DIM; row++) {
      std::cout << " " << (char)('A' + row) << " |";
      for (int col = 0; col < DIM; col++) {
         if (this->board[row][col] != nullptr) {
            std::cout << " " << board[row][col]->getLetter() << " |";
         }
         else {
            std::cout << "   |";
         }
      }
      std::cout << std::endl;
   }
}

/**
 * @brief Returns a string representing the board.
 */
std::string Board::getBoard() {
   std::string ret = "";
   for (int row = 0; row < DIM; row++) {
      for (int col = 0; col < DIM; col++) {
         if (this->board[row][col] != nullptr) {
            ret += board[row][col]->getLetter();
            ret += "|";
         }
         else {
            ret += " |";
         }
      }
      ret += "\n";
   }

   return ret;
}

/**
 * @brief Pushes tile (letter, row and column) to tileQueue.
 * @param letter Letter of tile.
 * @param coords Coordinate string of tile. E.g. "A12".
 */
void Board::placeBuffer(Letter letter, std::string coords) {
   int row = coords[0] - CHAR_TO_INT_OFFSET; // hash define
   int col = -1;


   // Makes sure that the input is correct
   if (coords.length() > 1) {
      // this uses string to int and substrings to convert from string to int
      col = std::stoi(coords.substr(1, coords.length() - 1));
   }

   // If out of range, do not place the tile.
   if (!(row > DIM || col >= DIM || row < 0 || col <= 0)) {
      std::tuple<Letter, int, int> tile = std::make_tuple(letter, row, col);
      this->tileQueue.push_back(tile);
   }
   else {
      std::cout << "Invalid board location, try again..." << std::endl;
   }
}

/**
 * @brief Places all tiles in tileQueue onto the board, ensuring placement is legal.
 * @return true Legal move.
 * @return false Illegal move.
 */
bool Board::placeAll(Player* player) {
   bool ret = false;
   if (this->tileQueue.empty() == false) {
      Letter letter = 'A';
      int row = std::get<1>(this->tileQueue.front());
      int col = std::get<2>(this->tileQueue.front());
      bool hor = true;
      bool ver = true;
      bool legal = true;
      int countBefore = 0;
      int countAfter = 0;

      // invoke int grid
      for (int i = 0; i < DIM; i++) {
         for (int j = 0; j < DIM; j++) {
            if (this->board[i][j] != nullptr) {
               this->intBoard[i][j] = 1;
               countBefore++;
            }
         }
      }

      // check if all tiles exist on same axis
      for (std::tuple<Letter, int, int> tile : this->tileQueue) {
         if (std::get<1>(tile) != row) {
            hor = false;
         }
         if (std::get<2>(tile) != col) {
            ver = false;
         }
         this->intBoard[std::get<1>(tile)][std::get<2>(tile)] = 1;
      }

      // checks if tiles are placed in a connected line
      // this method is considerate of players using preexisting tiles to build their word
      if (hor == true) {
         for (int i = col; i < std::get<2>(this->tileQueue.back()); i++) {
            if (this->intBoard[row][i] == 0) {
               legal = false;
            }
         }
      }
      if (ver == true) {
         for (int i = row; i < std::get<1>(this->tileQueue.back()); i++) {
            if (this->intBoard[i][col] == 0) {
               legal = false;
            }
         }
      }

      // penultimate condition check to determine move legality
      if ((hor == true || ver == true) && legal == true) {
         // sweeper modifies countAfter using references
         // sweep takes (7, 7) as argument to enforce first turn must be in centre of board
         this->sweep(BOARD_CENTER, BOARD_CENTER, countAfter);
      }

      // if there exists at least one tile that cannot be reached from (7, 7)
      // it is an invalid move
      if (countBefore + (int)this->tileQueue.size() == countAfter) {
         ret = true;
         if (this->tileQueue.size() == BINGO) {
            std::cout << "BINGO!" << std::endl;
            player->addScore(50);
         }

         // execute tile placement
         while (this->tileQueue.empty() == false) {
            std::tie(letter, row, col) = this->tileQueue.front();
            Tile* tileToPlace = player->getHand()->removeLetter(letter);
            this->board[row][col] = tileToPlace;
            player->addScore(tileToPlace->getValue());
            this->tileQueue.pop_front();
            player->draw(this->tilebag);
         }
      }
      else {
         std::cout << "Invalid move, try again..." << std::endl;
         this->tileQueue.clear();
      }
   }
   else {
      std::cout << "No letters were queued for placement, try again..." << std::endl;
   }
   return ret;
}

string Board::returnWord(Player* player) {
   //Define word to be returned for verification
   string retWord = "";

   //Get player word coordinates
   if (this->tileQueue.empty() == false) {
      int row = std::get<1>(this->tileQueue.front());
      int col = std::get<2>(this->tileQueue.front());
      bool hor = true;
      bool ver = true;

      // Create int and char grids
      for (int i = 0; i < DIM; i++) {
         for (int j = 0; j < DIM; j++) {
            if (this->board[i][j] != nullptr) {
               this->charBoard[i][j] = this->board[i][j]->getLetter();
            }
         }
      }

      for (int i = 0; i < DIM; i++) {
         for (int j = 0; j < DIM; j++) {
            if (this->board[i][j] != nullptr) {
               this->intBoard[i][j] = 1;
            }
         }
      }
      
      for (std::tuple<Letter, int, int> tile : this->tileQueue) {
         if (std::get<1>(tile) != row) {
            hor = false;
         }
         if (std::get<2>(tile) != col) {
            ver = false;
         }
         this->charBoard[std::get<1>(tile)][std::get<2>(tile)] = std::get<0>(tile);
         this->intBoard[std::get<1>(tile)][std::get<2>(tile)] = 1;
      }     

      // Orientation check for word then push back to retWord
      if (hor == true) {
         for (int i = col; i <= std::get<2>(this->tileQueue.back()); i++) {
            retWord.push_back(this->charBoard[row][i]);
         }
      }
      if (ver == true) {
         for (int i = row; i <= std::get<1>(this->tileQueue.back()); i++) {
            retWord.push_back(this->charBoard[i][col]);
         }
      }
   }
   return retWord;
}

/**
 * @brief Gets string representation of tileBag.
 */
std::string Board::getTiles() {
   return this->tilebag->getNodeStrings();
}

/**
 * @brief Starting from row, col, counts adjacent tiles recursively.
 * @param row Current row.
 * @param col Current col.
 * @param count Value that is incremented when counting.
 */
void Board::sweep(int row, int col, int& count) {
   // precondition sweep is always invoked with row = 7 and col = 7

   // if sweeper is currently standing on a tile
   if (this->intBoard[row][col] == 1) {
      count++;
   }

   // to prevent backtracking
   this->intBoard[row][col] = 2;

   // check up
   if (row - 1 >= 0 && this->intBoard[row - 1][col] == 1) {
      sweep(row - 1, col, count);
   }

   // check right
   if (col + 1 < DIM && this->intBoard[row][col + 1] == 1) {
      sweep(row, col + 1, count);
   }

   // check down
   if (row + 1 < DIM && this->intBoard[row + 1][col] == 1) {
      sweep(row + 1, col, count);
   }

   // check left
   if (col - 1 >= 0 && this->intBoard[row][col - 1] == 1) {
      sweep(row, col - 1, count);
   }
}