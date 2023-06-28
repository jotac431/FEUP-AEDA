//
// Created by Duarte on 13/11/2020.
//

#include "date.h"

/*! \file date.cpp
    \brief Date body.
*/

Date::Date() { //default date = defaults to pc local time
    time_t t = time(nullptr);
    struct tm tm = *localtime(&t); //time is created and set to the pc local time
    year = tm.tm_year + 1900; //2020 in tm is 120, adjusted
    month = tm.tm_mon + 1; //months in tm are 0-11, adjusted
    day = tm.tm_mday;
}

Date::Date(int y, int m, int d) { //non default date, set by user
    year = y;
    month = m;
    day = d;
}

int Date::age() const{ //calcs age from a given birthdate
    time_t t = time(nullptr);
    struct tm tm = *localtime(&t); //gets localtime
    int age = tm.tm_year + 1900 - year; //subtracts the years
    if (tm.tm_mon+1 < month || (tm.tm_mon+1 == month && tm.tm_mday < day)) { //check the birthday
        age--; //if this years birthday is yet to happen, age is reduced by 1
    }
    return age;
}