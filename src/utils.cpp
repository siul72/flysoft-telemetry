#include "utils.h"


Utils::Utils(){

}

void Utils::print_hex(unsigned char *buffer_in, int buffer_size, char * msg){

    for(int j = 0; j < buffer_size; j++)
        sprintf(&msg[5*j], "0x%02X,", buffer_in[j]);

}

void Utils::timeToString(char* string, size_t size) {
  unsigned long nowMillis = millis();
  unsigned long seconds = nowMillis / 1000;
  uint8_t mil = nowMillis % 1000;
  int days = seconds / 86400;
  seconds %= 86400;
  byte hours = seconds / 3600;
  seconds %= 3600;
  byte minutes = seconds / 60;
  seconds %= 60;
  snprintf(string, size, "%04d:%02d:%02d:%02d.%03d", days, hours, minutes, static_cast<uint8_t>(seconds), mil);
}

std::vector<String> Utils::stringSplit(String data, char separator){

    std::vector<String> myList;

    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex; i++) {
        if (data.charAt(i) == separator) {

            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
            myList.push_back(data.substring(strIndex[0], strIndex[1]));
        }
    }
    return myList;
}
