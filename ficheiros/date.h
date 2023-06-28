//
// Created by Duarte on 13/11/2020.
//

#ifndef AEDAPROJ_DATE_H
#define AEDAPROJ_DATE_H

#include <ctime>

/*! \file date.h
    \brief Date header. Implements a simple date to use for birth dates and age calculation
*/

/**
 * Simple date class, with day/month/years and age calculations
 */
class Date {
public:
    int day;
    int month;
    int year;
    /**
     * Default Date constructor ( defaults to current day)
     */
    Date();
    /**
     * Age constructor
     * @param y year
     * @param m month
     * @param d day
     */
    Date(int y, int m, int d);
    /**
     * Gets current date and returns age from a given birthdate
     * @return Int for age
     */
    int age() const;
    friend bool operator<=(const Date& d1, const Date& d2);
};

#endif //AEDAPROJ_DATE_H
