#include "LinkedList.h"
#include "Board.h"
#include "Player.h"
#include "Settings.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <locale>
#include <algorithm>
#include <cctype>

using std::string;

#define EXIT_SUCCESS    0
#define FILE_LINES      28

#define NEW_GAME 1
#define MULTIPLAYER 1

#define LOAD_GAME 2
#define WORDCHECK 2

#define SETTINGS 3
#define HELP 3

#define CREDITS 4
#define LOAD_DICT 4

#define QUIT 5
#define RETURN 5

void mainMenu(Settings* settings);
int invalidMenu();
void MPgame(Player** players, Board* board, Player* playerTurn, Settings* settings);

void startNewGame(Settings* settings);
string invalidPlayerName();
void help();
void settingsMenu(Settings* settings);
string* wordListToArray(Settings* settings);
bool verifyWord(string wordToCheck, Settings* settings);
bool strToBool(string onOff);

void loadGame(Settings* settings);
string invalidFileName();
std::vector<string> invalidFileSize();
std::vector<string> invalidFormat();

void game(Player* player1, Player* player2, Board* board, Player* playerTurn, Settings* settings);

void credits();
void endGame();

int main(void) {
   Settings* settings = new Settings();
   std::cout << "Welcome to Scrabble!\n-------------------\n" << std::endl;
   mainMenu(settings);

   return EXIT_SUCCESS;
}

/**
 * Displays the main menu and takes input, allowing for selection of options.
 */
void mainMenu(Settings* settings) {
   std::cout << "Menu\n----\n1. New Game\n2. Load Game\n3. Settings\n4. Credits (Show student information)\n5. Quit\n\n> ";

   int input;
   std::cin >> input;

   // Ends game if EOF
   if (std::cin.eof()) {
      input = QUIT;
   }

   if (input > QUIT || input < NEW_GAME) {
      input = invalidMenu();
   }

   // For each of the potential inputs
   if (input == NEW_GAME) {
      startNewGame(settings);
   }
   else if (input == LOAD_GAME) {
      loadGame(settings);
   }
   else if (input == CREDITS) {
      credits();
      mainMenu(settings);
   }
   else if (input == SETTINGS) {
      settingsMenu(settings);
   }
   else if (input == QUIT) {
      endGame();
   }

}

/**
 * Determines if wordToCheck is a valid word within the words file.
 * Returns true if it is found in file, false otherwise.
 * @param wordToCheck The word to be verified.
 */
bool verifyWord(string wordToCheck, Settings* settings) {
   bool ret = false;
   //Have word for comparison
   string line;

   //Read in file for checking
   std::ifstream textFile;
   textFile.open(settings->DICT_NAME);

   //Loop through word file to find match
   if (textFile.is_open()) {
      while (!textFile.eof()) {

         std::getline(textFile, line);
         std::transform(line.begin(), line.end(),line.begin(), ::toupper);

         if (line == wordToCheck) {
            //If match found return true, else false
            textFile.close();
            ret = true;

         }
      }
   }
   if (ret == false){
      std::cout << "Word not found in dictionary" << std::endl;
   }
   textFile.close();
   return ret;
}

string* wordListToArray(Settings* settings){
   string line;
   int pos = 0;
   string* textArray = new string[TEXT_SIZE];

   //Open file
   std::ifstream textFile;
   textFile.open(settings->DICT_NAME);

   //Add each word encountered into the text array
   if (textFile.is_open())
   {
      while (!textFile.eof()){
         getline(textFile,line);
         textArray[pos] = line;
         pos++;
      }
    textFile.close();
   }
   else std::cout << "Can't open text file." << std::endl;
   
   //Returns a pointer to the text array
   return textArray;
}

void settingsMenu(Settings* settings) {
   int input;

   std::cout << "--- Game settings ---" << std::endl;
   std::cout << "Please enter the setting you wish to modify:" << std::endl;

   // Turn on and off 3-4 player mode
   std::cout << "1. 3-4 player mode: " << settings->onOff(settings->MP_STATUS) << std::endl;

   // Turn on and off verify words
   std::cout << "2. Word checking: " << settings->onOff(settings->WC_STATUS) << std::endl;

   // Turn on and off help hints function
   std::cout << "3. Help function: " << settings->onOff(settings->HELP_STATUS) << std::endl;

   // Load in new dictionary file
   std::cout << "4. Load in new dictionary file for gameplay" << std::endl;

   //Return to main
   std::cout << "5. Return to main menu" << std::endl;

   std::cin >> input;

   if (std::cin.eof()) {
      input = QUIT;
   }

   if (input > RETURN || input < MULTIPLAYER) {
      input = invalidMenu();
      mainMenu(settings);
   }

   // Each potential input
   if (input == MULTIPLAYER) {
      std::cout << "Scrabble can be played with 2-4 people. Please input the number of players:\n\n>";
      int playerCount;
      std::cin >> playerCount;
      if (((3 == playerCount) || (playerCount == 4)) && (settings->MP_STATUS == false)){
         settings->PLAYER_COUNT = playerCount;
         settings->updateSetting("MP_STATUS");
      }
      else if ((playerCount == 2) && (settings->MP_STATUS == true)){
         settings->PLAYER_COUNT = playerCount;
         settings->updateSetting("MP_STATUS");
      }
      else if (playerCount == (settings->PLAYER_COUNT)){
         std::cout << "You are already set to this many players" << std::endl;
      }
      else if ((playerCount < 2) || (playerCount > 4)){
         std::cout << "Invalid number of players, please input a number from 2 to 4" << std::endl;
      }
      settingsMenu(settings);
   }

   else if (input == WORDCHECK) {
      settings->updateSetting("WC_STATUS");
      settingsMenu(settings);
   }

   else if (input == HELP) {
      settings->updateSetting("HELP_STATUS");
      settingsMenu(settings);
   }

   else if (input == LOAD_DICT) {
      string dictName;
      std::cout << "Enter name of the dictionary file to load:\n> ";
      std::cin >> dictName;
      if (std::cin.eof()) {
         endGame();
      }
      else {
         std::ifstream file(dictName);
         if (!file){
            std::cout << "Invalid dictionary file\n" << std::endl;
            settingsMenu(settings);
         }
         settings->DICT_NAME = dictName;
         file.close();
         std::cout << "Successfully changed dictionary file\n" << std::endl;
         settingsMenu(settings);
      }
   }

   else if (input == RETURN) {
      mainMenu(settings);
   }
}

bool strToBool(string onOff){
   bool ret = false;
   if (onOff == "ON"){
      ret = true;
   }
   return ret;
}

void help() {
   std::cout << "--- HELP ---"  << std::endl;

   std::cout << "1. Loading a game"  << std::endl;
   std::cout << "To load a game from the main menu, input 2 and type the name of the save file.\n" << std::endl;

   std::cout << "2. Saving a game"  << std::endl;
   std::cout << "To save a game, when it is your turn type 'save' and press enter, you will be prompted to name the file, once done press enter again to save your game.\n" << std::endl;

   std::cout << "3. Placing a letter"  << std::endl;
   std::cout << "To place a letter during your turn, type 'place X at Y' with X being the letter you wish to place and Y being the board coordinate to place it at, the board coordinate must be letter first then column number (e.g. H7).\n" << std::endl;

   std::cout << "4. Replacing a letter in your hand"  << std::endl;
   std::cout << "To replace a letter in your hand, during your turn type 'replace X' with X being the letter you wish to replace, this will replace it with a random letter from the bag.\n" << std::endl;

   std::cout << "5. Passing your turn"  << std::endl;
   std::cout << "To pass your turn, during your turn type 'pass', this will pass the initiative to the other player, remember that you can't pass twice in a row.\n" << std::endl;

   std::cout << "6. Game rules"  << std::endl;
   std::cout << "The general game rules for scrabble are as follows:" << std::endl;
   std::cout << "- Each player starts the game with seven letters" << std::endl;
   std::cout << "- Players take turns placing tiles to make words" << std::endl;
   std::cout << "- Words are worth the cumulative total of their letter values which are found next to each one in your hand" << std::endl;
   std::cout << "- Whoever has the most points when the tilebag becomes empty will win, otherwise if a player passes twice in a row that player loses\n" << std::endl;

   std::cout << "7. Changing settings"  << std::endl;
   std::cout << "To change settings, from the main menu type 3 as your selection and press enter to enter the settings menu and change options.\n" << std::endl;

   std::cout << "8. Actions available during your turn"  << std::endl;
   std::cout << "During the player's turn the player has access to four options:" << std::endl;
   std::cout << "- Replace: Replace a letter in your hand with one from the bag" << std::endl;
   std::cout << "- Place: Place a letter from your hand onto the board" << std::endl;
   std::cout << "- Save: Save the current game state" << std::endl;
   std::cout << "- Pass: Pass your current turn\n" << std::endl;

}

/**
 * Requests menu input from user, and loops until valid value inputted.
 * Returns input as an Integer.
 */
int invalidMenu() {
   int input;
   std::cout << "Invalid input" << std::endl;
   std::cout << "> ";
   std::cin >> input;
   std::cout << std::endl;

   if (std::cin.eof()) {
      input = QUIT;
   }
   else {
      if (input > QUIT || input < NEW_GAME) {
         input = invalidMenu();
      }

   }
   return input;
}

/**
 * Gets input for player names and starts a new game, generating the board.
 * Runs game function with inputted values.
 */
void startNewGame(Settings* settings) {
   if (settings->MP_STATUS == false){
      string name1;
      string name2;
      std::cout << "\nStarting a New Game" << std::endl;
      std::cout << "Enter a name for player 1 (uppercase characters only)\n> ";
      std::cin >> name1;

      if (std::cin.eof()) {
         endGame();
      }
      else {
         if (!std::regex_match(name1, std::regex("^[A-Z]+$"))) {
            name1 = invalidPlayerName();
         }

         std::cout << "Enter a name for player 2 (uppercase characters only)\n> ";
         std::cin >> name2;
         if (std::cin.eof() || name1 == "end") {
            endGame();
         }
         else {
            if (!std::regex_match(name2, std::regex("^[A-Z]+$"))) {
               name2 = invalidPlayerName();
            }

            if (name2 != "end") {
               std::cout << "\nLet's Play!" << std::endl;
               std::cout << std::endl;

               Board* board = new Board();
               Player* player1 = new Player(name1);
               Player* player2 = new Player(name2);
               board->buildBag();
               board->buildHand(player1);
               board->buildHand(player2);

               Player* playerTurn = player1;

               game(player1, player2, board, playerTurn, settings);
            }
         }
      }
   }
   else if (settings->MP_STATUS == true){
      std::cout << "\nStarting a New Game" << std::endl;
      string playerName = "";
      Player** players = new Player*[settings->PLAYER_COUNT];
      Board* board = new Board();
      board->buildBag();

      for (int i=1; i<=(settings->PLAYER_COUNT) && (playerName != "end"); ++i) {
         std::cout << "Please enter a name for Player " << i << " (names must be uppercase)\n> ";
         std::cin >> playerName;
         if (std::cin.eof()) {
            playerName = "end";
            endGame();
         }
         else {
            if (!std::regex_match(playerName, std::regex("^[A-Z]+$"))) {
               playerName = invalidPlayerName();
            }
            Player* newPlayerObj = new Player(playerName);
            players[i-1] = newPlayerObj;
            board->buildHand(newPlayerObj);
         }
      }

      std::cout << "\nAll players initialised, let's Scrabble!" << std::endl;
      std::cout << std::endl;

      MPgame(players, board, players[0], settings);
   }
}

/**
 * Requests player input from user, and loops until valid value inputted.
 * Returns input as a string.
 */
string invalidPlayerName() {
   string input;
   std::cout << "Invalid input" << std::endl;
   std::cout << "> ";
   std::cin >> input;
   if (std::cin.eof()) {
      input = "end";
   }
   else {
      std::cout << std::endl;
      if (!std::regex_match(input, std::regex("^[A-Z]+$"))) {
         input = invalidPlayerName();
      }
   }
   return input;

}

/**
 * Takes fileName from cin and loads from the file.
 * Validates file formatting before reading.
 * Runs game function with loaded values.
 */
void loadGame(Settings* settings) {
   int pCount = 2;

   std::cout << "Enter the save game name to load:\n> ";
   string filename;
   std::cin >> filename;

   // Standard end of file character check
   if (std::cin.eof()) {
      endGame();
   }

   else {
      // Check for file validity
      std::ifstream file(filename);

      if (!file) {
         file.close();
         filename = invalidFileName();
         file.open(filename);
      }

      if (filename != "end") {
         std::vector<string> lines;
         if (file.is_open()) {
            while (!file.eof()) {
               string tmp;
               std::getline(file, tmp);
               // // Just double checks that something isn't going wrong here
               // // Some OSes may add \r to the end of lines
               if (!tmp.empty() && tmp[tmp.size() - 1] == '\r')
                  tmp.erase(tmp.size() - 1);

               lines.push_back(tmp);
            }
         }

         if (!lines.empty()) {
            pCount = std::stoi(lines.back());
            bool boardCheck = true;
            bool nameCheck = false;
            bool playerScoreCheck = true;
            bool playerTileCheck = true;        

            // Retrieves the board structure
            std::vector<string> boardStrings;
            for (int i = (3*pCount); i < 15+(3*pCount); ++i) {
               boardStrings.push_back(lines[i]);
            }

            //Checks players have all valid data
            for (int i=0; i<pCount; ++i) {
               if (lines[3*i] == lines[16+(3*pCount)]) {
                  nameCheck = true;
               }
               if (!std::isdigit(lines[1+(3*i)][0])) {
                  playerScoreCheck = false;
               }
               if (!lines[2+(3*i)].find('-')) {
                  playerTileCheck = false;
               }
            }

            for (auto line : boardStrings) {
               boardCheck = boardCheck && (line.find('|') != std::string::npos);
            }

            //Calls invalid if data is improper
            if (!(nameCheck && playerScoreCheck && playerTileCheck && boardCheck)) {
               lines = invalidFormat();
               if (!lines.empty()) {
                  std::vector<string> tmpBoardStrings;
                  for (int i = (3*pCount); i < 15+(3*pCount); ++i) {
                     tmpBoardStrings.push_back(lines[i]);
                  }
                  boardStrings = tmpBoardStrings;
               }
            }

            //Initialises the game and settings with the saved values
            if (!lines.empty()) {
               Settings* settings = new Settings();
               settings->MP_STATUS = strToBool(lines[23 + (3*(pCount-2))]);
               settings->HELP_STATUS = strToBool(lines[24 + (3*(pCount-2))]);
               settings->WC_STATUS = strToBool(lines[25 + (3*(pCount-2))]);
               settings->DICT_NAME = lines[26 + (3*(pCount-2))];
               settings->PLAYER_COUNT = pCount;
               string currPlayerName = lines[16+(3*settings->PLAYER_COUNT)];
               std::cout << currPlayerName << "\n";
               
               Player** players = new Player*[settings->PLAYER_COUNT];
               Player* playerTurn = nullptr;
               for (int i=0; i<settings->PLAYER_COUNT; ++i) {
                  int playerScore = std::stoi(lines[1+(3*i)]);
                  Player* currPlayer = new Player(lines[0+(3*i)], playerScore, lines[2+(3*i)]);
                  players[i] = currPlayer;

                  if (currPlayer->getName() == currPlayerName) {
                     playerTurn = currPlayer;
                  }
                  else{
                     playerTurn = players[0];
                  }
               }

               Board* board = new Board(lines[15+(3*settings->PLAYER_COUNT)], boardStrings);
               std::cout << "Game loaded successfully" << std::endl;
               MPgame(players, board, playerTurn, settings);
            }
         }
      }
      file.close();
   }
}

/**
 * Requests fileName from user, and loops until file found matching name.
 * Returns input as a string.
 */
string invalidFileName() {
   string input;
   std::cout << "File does not exist." << std::endl;
   std::cout << "> ";
   std::cin >> input;
   if (std::cin.eof()) {
      input = "end";
      endGame();
   }
   else {
      std::cout << std::endl;
      std::ifstream file(input);
      if (!file) {
         input = invalidFileName();
      }
      file.close();
   }
   return input;
}

/**
 * Requests new file input from user when file size is incorrect.
 * Loops until file size is correct.
 * @return std::vector<string> Vector of lines of correctly sized file.
 */
std::vector<string> invalidFileSize() {
   string input;
   std::vector<string> lines;

   std::cout << "File is not formatted correctly." << std::endl;
   std::cout << "> ";
   std::cin >> input;
   std::cout << std::endl;
   if (std::cin.eof()) {
      endGame();
   }
   else {
      // Check for file validity (exists and correct format)
      std::ifstream file(input);
      if (!file) {
         input = invalidFileName();
      }

      if (file.is_open()) {
         while (file) {
            string tmp;
            std::getline(file, tmp);
            lines.push_back(tmp);
         }
      }
      file.close();

      if (lines.size() != FILE_LINES + 1) {
         lines = invalidFileSize();
      }
   }
   return lines;
}

/**
 * Requests new file input from user when file is formatted incorrectly.
 * Loops until file has correct formatting.
 * @return std::vector<string> Vector of lines of correctly formatted file.
 */
std::vector<string> invalidFormat() {
   std::vector<string> lines = invalidFileSize();

   if (!lines.empty()) {
      // Get vector just of the board
      std::vector<string> boardStrings;
      for (int i = 6; i < 21; ++i) {
         boardStrings.push_back(lines[i]);
      }

      bool names = (lines[0] == lines[22]) || (lines[3] == lines[22]);

      std::locale loc;
      bool nums = (isdigit(lines[1], loc)) && (isdigit(lines[4], loc));

      bool tiles = (lines[2].find('-') != std::string::npos) && (lines[5].find('-') != std::string::npos) && (lines[21].find('-') != std::string::npos);

      bool boardBool = true;
      for (auto line : boardStrings) {
         boardBool = boardBool && (line.find('|') != std::string::npos);
      }

      if (!(names && nums && tiles && boardBool)) {
         lines = invalidFormat();
      }
   }

   return lines;
}

void MPgame(Player** players, Board* board, Player* playerTurn, Settings* settings) {
   string opt;
   string input[4];
   std::stringstream stream;
   bool validMove;
   bool terminateGame = false;
   bool correctWord = false;
   //string* wordList = wordListToArray(settings);

   std::getline(std::cin, opt);
   while (!std::cin.eof() && terminateGame == false) {
      validMove = false;
      if (board->getTiles() == "") {
         // tile bag is empty, terminate game
         terminateGame = true;
      }

      /*
       * Prints out board state in this format:
       * Player's turn
       * Scores
       * Board
       * Player's hand
       * Options
       */
       // Declares whose turn it is
      playerTurn->declareTurn();

      // Prints scores
      for(int i = 0; i < settings->PLAYER_COUNT; ++i){
         players[i]->printScore();
      }
      std::cout << std::endl;

      // Prints board
      board->print();
      std::cout << std::endl;

      // Prints out the hand of the current player
      std::cout << "Your hand is" << std::endl;
      playerTurn->printHand();
      std::cout << std::endl << std::endl;

      // Options
      std::cout << "Please choose an option:\nreplace\nplace\nsave\npass\nhelp\n\n> ";

      // Gets input
      std::getline(std::cin, opt);
      if (!std::cin.eof()) {
         stream.str(opt);

         // input is a string array of size 4
         // where if you type in:
         //    "place A at A0"
         // then 
         //    input[0] = "place"
         //    input[1] = "A"
         //    input[2] = "at"
         //    input[3] = "A0"

         // in input[x][y], the [y] is the character index
         // e.g.
         //    input[0][0] = p
         //    input[0][1] = l
         //    input[0][2] = a
         //    input[0][3] = c
         //    input[0][4] = e

         for (int i = 0; i < 4; i++) {
            stream >> input[i];
         }

         // I moved this up here because the player is able to take action if they so choose afterwards
         if (input[0] == "save") {
            /*
             * FORMAT:
               <player i name>
               <player i score>
               <player i hand>
               <Board State>
               <tile bag contents>
               <current player name>
               <multiplayer setting status>
               <help setting status>
               <wordcheck setting status>
               <current dictionary file name>
               <current player count>
             */
            std::cout << "Please input a name for the save file:\n\n>";
            string filename;
            std::cin >> filename;
            std::ofstream myfile;

            myfile.open(filename);

            for (int i = 0; i < settings->PLAYER_COUNT; i++){
               myfile << players[i]->getName() << "\n" << players[i]->getScore() << "\n" << players[i]->getTiles() << "\n";
            }
            myfile << board->getBoard() << board->getTiles() << "\n" << playerTurn->getName() << "\n";
            myfile << settings->onOff(settings->MP_STATUS) << "\n";
            myfile << settings->onOff(settings->HELP_STATUS) << "\n";
            myfile << settings->onOff(settings->WC_STATUS) << "\n";
            myfile << settings->DICT_NAME << "\n";
            myfile << settings->PLAYER_COUNT;
            myfile.close();

            std::cout << "\nGame saved successfully" << std::endl;

            std::cout << "\n> ";
            std::getline(std::cin, opt);
            stream.str(opt);
            for (int i = 0; i < 4; i++) {
               stream >> input[i];
            }
         }

         if (input[0] == "replace") {
            validMove = board->replaceWithTileFromBag(playerTurn, input[1][0]);
         }
         else if (input[0] == "place" && input[2] == "at") {
            // input[3][0]-65
            // input[3][1]-48
            // these are literal int conversions from ascii characters

            board->placeBuffer(input[1][0], input[3]);
            stream.clear();
            bool cont = true;
            while (cont) {
               std::cout << "> ";
               std::getline(std::cin, opt);
               if (!std::cin.eof()) {
                  if (opt != "place Done") {
                     stream.str(opt);
                     for (int i = 0; i < 4; i++) {
                        stream >> input[i];
                     }
                     if (input[0] == "place" && input[2] == "at") {
                        board->placeBuffer(input[1][0], input[3]);
                     }
                     else {
                        std::cout << "Invalid command, try again..." << std::endl;
                     }
                     stream.clear();
                  }
                  else {
                     cont = false;
                  }
               }
               else {
                  cont = false;
                  terminateGame = true;
               }

            }
            if (!terminateGame && settings->WC_STATUS == true){
               correctWord = verifyWord(board->returnWord(playerTurn), settings);
            
               if (correctWord){
                  std::cout << "Word is valid" << std::endl;
                  validMove = board->placeAll(playerTurn);
               }

               else{
                  std::cout << "Invalid word" << std::endl;
               }
            }
            else if (!terminateGame && settings->WC_STATUS == false){
               validMove = board->placeAll(playerTurn);
            }
         }

         else if ((input[0] == "pass") && (playerTurn->getPass() < 1)) {
            std::cout << "Passing turn..." << std::endl;
            playerTurn->incrementPass();
            validMove = true;
         }

         else if ((input[0] == "help") && (settings->HELP_STATUS == true)){
            help();
         }

         else if ((input[0] == "help") && (settings->HELP_STATUS == false)){
            std::cout << "You have help disabled in the settings, please change the settings from the main menu to access this feature." << std::endl;
         }

         else if ((input[0] == "pass") && (playerTurn->getPass() >= 1)) {
            std::cout << "You cannot pass your turn twice in a row." << std::endl;
            terminateGame = true;
         }

         else {
            std::cout << "Option error, try again..." << std::endl;
         }
         std::cout << std::endl;
         stream.clear();

         // transfer initiative to other player (relative)
         if (validMove == true) {
            bool retPlayer = false;
            for (int i=0; i<(settings->PLAYER_COUNT) && (retPlayer == false); ++i) {
               if (playerTurn == players[settings->PLAYER_COUNT-1]) {
                  playerTurn = players[0];
                  retPlayer = true;
               }
               else if (playerTurn == players[i]) {
                  playerTurn = players[i+1];
                  retPlayer = true;
               }
            }
         }
      }
      // termination case if terminateGame == true
   }

   std::cout << "Game over" << std::endl;
   int winnerPos = 0;
   int winnerScore = -1;

   for (int i = 0; i <= settings->PLAYER_COUNT; i++){
      players[i]->printScore();
   }

   for (int i = 0; i < settings->PLAYER_COUNT; i++){
      if (players[i]->getScore() > winnerScore){
         winnerScore = players[i]->getScore();
         winnerPos = i;
      }
   }

   std::cout << "Player " << players[winnerPos]->getName() << " won!" <<std::endl;

   //delete board object
   delete board;
   
   //deallocate the players array
   for(int i = 0; i <= settings->PLAYER_COUNT ; i++)
      delete[] players[i];
   delete[] players;

   delete settings;
}


/**
 * Runs a game of scrabble with both players and the inputted board as a starting point.
 * Game ends when tile bag is empty or a player passes their turn twice in a row.
 *
 * @param player1 The first player.
 * @param player2 The second player.
 * @param board The game's board object.
 * @param playerTurn Pointer to player whose turn it is.
 */
void game(Player* player1, Player* player2, Board* board, Player* playerTurn, Settings* settings) {
   string opt;
   string input[4];
   std::stringstream stream;
   bool validMove;
   bool terminateGame = false;
   bool correctWord = false;
   //string* wordList = wordListToArray(settings);

   // bug fix: prevents skipping of player1 first turn
   std::getline(std::cin, opt);
   while (!std::cin.eof() && terminateGame == false) {
      validMove = false;
      if (board->getTiles() == "") {
         // tile bag is empty, terminate game
         terminateGame = true;
      }

      /*
       * Prints out board state in this format:
       * Player's turn
       * Scores
       * Board
       * Player's hand
       * Options
       */
       // Declares whose turn it is
      playerTurn->declareTurn();

      // Prints scores
      player1->printScore();
      player2->printScore();
      std::cout << std::endl;

      // Prints board
      board->print();
      std::cout << std::endl;

      // Prints out the hand of the current player
      std::cout << "Your hand is" << std::endl;
      playerTurn->printHand();
      std::cout << std::endl << std::endl;

      // Options
      std::cout << "Please choose an option:\nreplace\nplace\nsave\npass\nhelp\n\n> ";

      // Gets input
      std::getline(std::cin, opt);
      if (!std::cin.eof()) {
         stream.str(opt);

         // input is a string array of size 4
         // where if you type in:
         //    "place A at A0"
         // then 
         //    input[0] = "place"
         //    input[1] = "A"
         //    input[2] = "at"
         //    input[3] = "A0"

         // in input[x][y], the [y] is the character index
         // e.g.
         //    input[0][0] = p
         //    input[0][1] = l
         //    input[0][2] = a
         //    input[0][3] = c
         //    input[0][4] = e

         for (int i = 0; i < 4; i++) {
            stream >> input[i];
         }

         // I moved this up here because the player is able to take action if they so choose afterwards
         if (input[0] == "save") {
            /*
             * FORMAT:
               <player 1 name>
               <player 1 score>
               <player 1 hand>
               <player 2 name>
               <player 2 score>
               <player 2 hand>
               <Board State>
               <tile bag contents>
               <current player name>
             */
            std::cout << "Please input a name for the save file:\n\n>";
            string filename;
            std::cin >> filename;
            std::ofstream myfile;

            myfile.open(filename);

            myfile << player1->getName() << "\n" << player1->getScore() << "\n" << player1->getTiles() << "\n"
               << player2->getName() << "\n" << player2->getScore() << "\n" << player2->getTiles() << "\n"
               << board->getBoard() << board->getTiles() << "\n" << playerTurn->getName() << "\n";
            myfile << settings->onOff(settings->MP_STATUS) << "\n";
            myfile << settings->onOff(settings->HELP_STATUS) << "\n";
            myfile << settings->onOff(settings->WC_STATUS) << "\n";
            myfile << settings->DICT_NAME << "\n";
            myfile << settings->PLAYER_COUNT;
            myfile.close();

            std::cout << "\nGame saved successfully" << std::endl;

            std::cout << "\n> ";
            std::getline(std::cin, opt);
            stream.str(opt);
            for (int i = 0; i < 4; i++) {
               stream >> input[i];
            }
         }

         if (input[0] == "replace") {
            validMove = board->replaceWithTileFromBag(playerTurn, input[1][0]);
         }
         else if (input[0] == "place" && input[2] == "at") {
            // input[3][0]-65
            // input[3][1]-48
            // these are literal int conversions from ascii characters

            board->placeBuffer(input[1][0], input[3]);
            stream.clear();
            bool cont = true;
            while (cont) {
               std::cout << "> ";
               std::getline(std::cin, opt);
               if (!std::cin.eof()) {
                  if (opt != "place Done") {
                     stream.str(opt);
                     for (int i = 0; i < 4; i++) {
                        stream >> input[i];
                     }
                     if (input[0] == "place" && input[2] == "at") {
                        board->placeBuffer(input[1][0], input[3]);
                     }
                     else {
                        std::cout << "Invalid command, try again..." << std::endl;
                     }
                     stream.clear();
                  }
                  else {
                     cont = false;
                  }
               }
               else {
                  cont = false;
                  terminateGame = true;
               }

            }
            if (!terminateGame && settings->WC_STATUS == true){
               correctWord = verifyWord(board->returnWord(playerTurn), settings);
            
               if (correctWord){
                  std::cout << "Word is valid." << std::endl;
                  validMove = board->placeAll(playerTurn);
               }

               else{
                  std::cout << "Invalid word." << std::endl;
               }
            }
            else if (!terminateGame && settings->WC_STATUS == false){
               validMove = board->placeAll(playerTurn);
            }
         }

         else if ((input[0] == "pass") && (playerTurn->getPass() < 1)) {
            std::cout << "Passing turn..." << std::endl;
            playerTurn->incrementPass();
            validMove = true;
         }

         else if ((input[0] == "help") && (settings->HELP_STATUS == true)){
            help();
         }

         else if ((input[0] == "help") && (settings->HELP_STATUS == false)){
            std::cout << "You have help disabled in the settings, please change the settings from the main menu to access this feature." << std::endl;
         }

         else if ((input[0] == "pass") && (playerTurn->getPass() >= 1)) {
            std::cout << "You cannot pass your turn twice in a row." << std::endl;
            terminateGame = true;
         }

         else {
            std::cout << "Option error, try again..." << std::endl;
         }
         std::cout << std::endl;
         stream.clear();

         // transfer initiative to other player (relative)
         if (validMove == true) {
            playerTurn = (playerTurn == player1) ? player2 : player1;
         }
      }
   }

   std::cout << "Game over" << std::endl;
   player1->printScore();
   player2->printScore();
   if (player2->getScore() > player1->getScore()) {
      std::cout << "Player " << player2->getName() << " won!" << std::endl;
   }
   else {
      std::cout << "Player " << player1->getName() << " won!" << std::endl;
   }

   delete board;
   delete player1;
   delete player2;
   delete settings;
}

/**
 * Prints credits of game.
 */
void credits() {
   std::cout << "----------------------------------";
   const int memberLen = 4;
   string members[memberLen][2] = { {"Alli Toth", "s3902333"}, {"Carter Facey-Smith", "s3907874"}, {"Antoni Giannakopoulos", "s3895923"}, {"Kelvin Young", "s3899733"} };
   for (int i = 0; i < memberLen; ++i) {
      std::cout << "\nName: " << members[i][0] << "\nStudent ID: " << members[i][1] << "\nEmail: " << members[i][1] << "@student.rmit.edu.au" << std::endl;
   }
   std::cout << "----------------------------------";
   std::cout << "\n\n";
}

/**
 * Says goodbye when game is ended.
 */
void endGame() {
   std::cout << "Goodbye" << std::endl;
}
