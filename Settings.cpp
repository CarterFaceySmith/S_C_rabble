#include "Board.h"
#include "Settings.h"
#include <string>
using std::string;

Settings::Settings() {
   this->MP_STATUS = false;
   this->HELP_STATUS = false;
   this->WC_STATUS = false;
   this->DICT_NAME = "words";
   this->PLAYER_COUNT = 2;
}

Settings::~Settings(){

}

string Settings::onOff(bool setting){
   string ret;
   if (setting == true){
      ret = "ON";
   }
   else{
      ret = "OFF";
   }
   return ret;
}

void Settings::updateSetting(string boolName){
    if (boolName == "MP_STATUS"){
        this->MP_STATUS = !MP_STATUS;
    }

    else if (boolName == "WC_STATUS"){
        this->WC_STATUS = !WC_STATUS;
    }

    else if (boolName == "HELP_STATUS"){
        this->HELP_STATUS = !HELP_STATUS;
    }

    else{
        std::cout << "Invalid setting name for switching." << std::endl;
    }
}