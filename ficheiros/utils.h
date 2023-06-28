//
// Created by Jorge on 25/10/2020.
//

#ifndef AEDAPROJ_UTILS_H
#define AEDAPROJ_UTILS_H

#include <vector>
#include <list>
#include <string>
#include <windows.h>
#include <functional>

/*! \file utils.h
    \brief Utils headers, some useful functions used troughout the code.
*/

/**
 * Procura numa lista
 * @tparam Comparable template
 * @param l lista
 * @param x valor a comparar
 * @return iterador da lista
 */
template <class Comparable>
typename std::list<Comparable>::const_iterator sequentialSearch(const std::list<Comparable> &l, Comparable x)
{
    for (auto it = l.begin(); it != l.end(); it++)
        if (*it == x)
            return it;   // encontrou
    return l.end();     // não encontrou
}

/**
 * Makes all letters lowercase
 * @return Copy of the string with all chars lowercase
 */
std::string lowercase(std::string st);
/**
 * Makes all lowercase, 1st uppercase
 * @return Copy of the string, all chars lowercase, 1st uppercase
 */
std::string firstUpper(std::string st);

/**
 * Changes the console print colour
 * @param color Int value for the selected colour
 */
void Color(int color);

#endif //AEDAPROJ_UTILS_H
