#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <stdio.h>
//#include <ArduinoSTL.h>

#define FRAME_HEADER "KBUS"
#define  FRAME_FUNCTION_SEND_SINGLE_POT_VALUE "SEQID0;"
#define FRAME_END "ENDK;"


class Utils{
public:
    Utils();
    static void print_hex(unsigned char * buffer_in, int buffer_size, char * buffer_out);
    static void timeToString(char* string, size_t size);
    static std::vector<String> stringSplit(String data, char separator);

};

#endif // UTILS_H
