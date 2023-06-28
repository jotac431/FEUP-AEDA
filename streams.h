//
// Created by Duarte on 13/11/2020.
//

#ifndef AEDAPROJ_STREAMS_H
#define AEDAPROJ_STREAMS_H

/*! \file streams.h
    \brief Streams header. Contains the Stream and PrivateStream classes.
*/

#include <string>
#include "date.h"
#include "utils.h"
#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include "exceptions.h"

class Streamer; //forward declarations
class Viewer;

/**
 * Stream class and it's associated functions
 */
class Stream {
protected:
    std::string title;
    Date startDate;
    std::string language;
    int minAge;
    std::list<Viewer*> viewers;
    int finalViewers;
    int likes;
    int dislikes;
    Streamer* streamer;
public:
    /**
     * Stream constructor
     * @param t Stream title
     * @param lang Stream language
     * @param age Stream's minimum age
     * @param st Pointer to stream creator
     */
    Stream(std::string t, std::string lang, int age, Streamer* st);
    /**
     * Stream constructor with date and (dis)likes, used when loading from files
     * @param t Stream title
     * @param lang Stream language
     * @param age Stream's minimum age
     * @param st Pointer to stream creator
     * @param y int year
     * @param m int month
     * @param d int day
     * @param l Likes
     * @param dl Dislikes
     */
    Stream(std::string t, std::string lang, int age, Streamer* st, int y, int m, int d, int l, int dl);
    /**
    * Stream constructor with date, (dis)likes and viewers, used when loading finalized streams from files
    * @param t Stream title
    * @param lang Stream language
    * @param age Stream's minimum age
    * @param st Pointer to stream creator
    * @param y int year
    * @param m int month
    * @param d int day
    * @param l Likes
    * @param dl Dislikes
    * @param views int for finalViewers
    */
    Stream(std::string t, std::string lang, int age, Streamer* st, int y, int m, int d, int l, int dl, int views);
    /**
     * Sets finalviewers, clears from watching and vector
     */
    void close();
    /**
     * Lists stream info
     */
    virtual void info() const;
    /**
     * Displays likes and dislikes by absolute value and percentage
     */
    void likesDisplay() const;
    /**
     * Displays the first bit of info, which is the same for public and private
     */
    void infoHeader() const;
    /**
     * Gives age limit to join
     * @return Returns int minAge;
     */
    int ageLimit() const;
    /**
     * Gives stream language
     * @return std::string language;
     */
    std::string getLanguage() const;
    /**
     * Adds a certain viewer
     * @param viewer Viewer pointer
     */
    void addViewer(Viewer* viewer);
    /**
     * Removes a certain viewer
     * @param viewer Viewer pointer
     */
    void removeViewer(Viewer* viewer);
    /**
     * Viewer Count
     * @return Returns viewers.size() if active, finalviewers if inactive
     */
    int getViewerCount() const;
    /**
     * Number of likes
     * @return Returns likes
     */
    int getLikes() const;
    /**
     * For stream, always false
     * @return False
     */
    virtual bool isPrivate() const;
    /**
     * Gives feedback, on likes or dislikes
     * @param positive Selects whether to increase likes or dislikes;
     */
    void feedback(bool positive, int count = 1);
    /**
     * Calls likesDisplay(), since theres no private feedback messages
     */
    virtual void printFeedback() const;
    /**
     * Returns nickname of streamer whose stream this belongs to
     * @return std::string nickname
     */
    std::string streamerName() const;
    /**
     * Returns creation date
     * @return Date class object, startDate
     */
    Date getDate() const;
    /**
    * Used to save to files
    * @return String with stream info
    */
    virtual std::string save() const;
};

/**
 * Private stream class, derives from stream class
 */
class PrivateStream: public Stream {
private:
    std::list<std::string> whitelist;
    int capacity;
    std::list<std::string> feedback;
public:
    /**
     * Private stream constructor
     * @param t Title
     * @param lang Language
     * @param age Minimum age
     * @param st Pointer to streamer
     * @param capacity Max capacity
     */
    PrivateStream(std::string t, std::string lang, int age, Streamer* st,
                  int capacity);
    /**
    * Private stream constructor with date and likes, used to load from files
    * @param t Title
    * @param lang Language
    * @param age Minimum age
    * @param st Pointer to streamer
    * @param capacity Max capacity
    * @param y Year
    * @param m Month
    * @param d Day
    * @param l Likes
    * @param dl Dislikes
    */
    PrivateStream(std::string t, std::string lang, int age, Streamer* st,
                  int capacity, int y, int m, int d, int l, int dl);
    /**
    * Private stream constructor with date, likes and viewers, used to load finalized streams from files
    * @param t Title
    * @param lang Language
    * @param age Minimum age
    * @param st Pointer to streamer
    * @param capacity Max capacity
    * @param y Year
    * @param m Month
    * @param d Day
    * @param l Likes
    * @param dl Dislikes
    * @param views Final viewers
    */
    PrivateStream(std::string t, std::string lang, int age, Streamer* st,
                  int capacity, int y, int m, int d, int l, int dl, int views);
    /**
     * Function to get the whitelist list
     * @return Whitelist list
     */
    std::list<std::string> getWhiteList() const;
    /**
     * Function to check whether a user is allowed in a private stream
     * @param n User nickname
     * @return Bool, true if allowed
     */
    bool userAllowed(std::string n) const;
    /**
     * Checks if private stream hit capacity
     * @return Bool, true if full;
     */
    bool isFull() const;
    /**
     * Lists private stream info
     */
    void info() const;
    /**
     * Add nickname to whitelist
     * @param nick
     */
    void addToList(std::string nick);
    /**
     * Remove nickname from whitelist
     * @param nick
     */
    void removeFromList(std::string nick);
    /**
     * For private stream, just returns true
     * @return True
     */
    bool isPrivate() const;
    /**
     * Adds a message to the list of private messages
     * @param message Message to be kept
     */
    void privateFeedback(std::string message);
    /**
     * Prints likes/dislikes + feedback messages;
     */
    void printFeedback() const;
    /**
     * Used to save to files
     * @return String with stream info
     */
    std::string save() const;
};

#endif //AEDAPROJ_STREAMS_H
