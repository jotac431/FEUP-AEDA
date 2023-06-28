#include "utils.h"

/*! \file utils.cpp
    \brief Utils body.
*/

std::string lowercase(std::string st) {
    std::string newString;
    for(auto ch : st) {
        newString.push_back(tolower(ch));
    }
    return newString;
}

std::string firstUpper(std::string st) {
    std::string newString = lowercase(st);
    newString[0] = toupper(newString[0]);
    return newString;
}

//COLORS LIST
//1: Blue
//2: Green
//3: Cyan
//4: Red
//5: Purple
//6: Yellow (Dark)
//7: Default white
//8: Gray/Grey
//9: Bright blue
//10: Brigth green
//11: Bright cyan
//12: Bright red
//13: Pink/Magenta
//14: Yellow
//15: Bright white
//Numbers after 15 include background colors

void Color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
