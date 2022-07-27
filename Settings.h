#include <string>
#include "Tile.h"
#include "LinkedList.h"
#include "Player.h"
#include "Board.h"

#define DIM 15
#define TEXT_SIZE 235886
#define MAX_ARRAY_SIZE 99
#define BOARD_CENTER DIM/2
#define BINGO 7
#define CHAR_TO_INT_OFFSET 65
#define BOARD_LINE_SIZE 61

class Settings {
public:
    Settings();
    ~Settings();
    void updateSetting(std::string boolName);
    void settingsMenu();
    std::string onOff(bool setting);
    bool MP_STATUS;
    bool WC_STATUS;
    bool HELP_STATUS;
    std::string DICT_NAME;
    int PLAYER_COUNT;
   

private:
    
};


