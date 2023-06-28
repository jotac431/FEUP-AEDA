//
// Created by Duarte on 13/11/2020.
//

#ifndef AEDAPROJ_USERS_H
#define AEDAPROJ_USERS_H

/*! \file users.h
    \brief Users header: contains the User and derived Viewer, Streamer and Admin classes.
*/

#include "streams.h"

/**
 * Base class for users(viewers, streamers and admins)
 */
class User {
protected:
    std::string name;
    Date bd;
    std::string nickname;
public:
    /**
     * User constructor
     * @param n Real name
     * @param nick Nickname
     * @param y Year of birth
     * @param m Month of birth
     * @param d Day of birth
     */
    User(std::string n, std::string nick, int y, int m, int d);
    /**
     * Gets user age
     * @return Int for the age of the user
     */
    int getAge() const;
    /**
     * Gets nickname
     * @return String corresponding to nickname
     */
    std::string getNick() const;
    /**
    * Pure virtual, used by inheritors
    */
    virtual std::string save() const = 0;
    /**
     * Pure virtual, used by inheritors
     */
    virtual std::string type() const = 0;
};

/**
 * Class for viewers and their actions
 */
class Viewer: public User{
private:
    Stream* watchingStream;
public:
    /**
     * Viewer constructor
     * @param n Real name
     * @param nick Nickname
     * @param y Year of birth
     * @param m Month of birth
     * @param d Day of birth
     */
    Viewer(std::string n, std::string nick, int y, int m, int d);
    /**
     * Sets watchingStream to given pointer
     */
    void watchStream(Stream* s);
    /**
     * Leaves current stream, exception ViewerNotWatching if not in one
     */
    void leaveStream();
    /**
     * Checks if viewer is watching a stream
     * @return Bool, True if watching;
     */
    bool watching() const;
    /**
     * Adds a like or dislike to the currently watched stream
     * @param positive If true, gives like, otherwise dislike
     */
    void publicFeedback(bool positive);
    /**
     * Adds message feedback on a private stream
     * @param message Message to be sent
     */
    void privateFeedback(const std::string& message);
    /**
     * << Overload used to print user info -> name and what theyre watching
     */
    friend std::ostream &operator<<(std::ostream &output, const Viewer& vw);
    /**
    * Used to save to files
    * @return String with viewer info
    */
    std::string save() const;
    /**
    * Used to check for what type of user were dealing with when we have a User* pointer
    * @return "viewer"
    */
    std::string type() const;
};

/**
 * Class for streamers and their actions
 */
class Streamer: public User{
private:
    Stream* stream;
    std::vector<Stream*> streamHistory;
public:
    /**
     * Constructor for streamers
     * @param n Real name
     * @param nick Nickname
     * @param y Year of birth
     * @param m Month of birth
     * @param d Day of birth
     */
    Streamer(std::string n, std::string nick, int y, int m, int d);
    /**
     * Gets public stream
     * @return Stream pointer
     */
    Stream* getStream() const;
    /**
     * Returns stream history
     * @return Stream history (vector of pointers)
     */
    std::vector<Stream*> getStreamHistory() const;
    /**
     * Creates a stream, throws StreamerAlreadyStreaming if streamer is already streaming
     * @param n Stream name
     * @param lang Stream language
     * @param age Stream's minimum age
     */
    void createStream(std::string n, std::string lang, int age);
    /**
     * Creates a private stream, throws StreamerAlreadyStreaming if streamer is already streaming
     * @param n Stream name
     * @param lang Stream language
     * @param age Stream min age
     * @param capacity Stream capacity (max users)
     * @param wl List of allowed users (by nickname)
     */
    void createPrivateStream(std::string n, std::string lang, int age, int capacity);
    /**
     * Ends the stream, making pointer nullptr and adding it to history. Throws StreamerNotStreaming if not streaming;
     */
    void endStream();
    /**
     * Checks if the streamer has a stream on
     * @return Bool, True if streaming
     */
    bool streaming() const;
    /**
     * Prints the current active viewers
     */
    void checkViewers();
    /**
     * If streaming privately, adds a nick to the whitelist
     * @param nick Allowed nickname
     */
    void allowPrivate(std::string nick);
    /**
     * If streaming privately, removes a nick from the whitelist, and kick the person out if theyre watching
     * @param nick Removed nickname
     */
    void removePrivate(std::string nick);
    /**
     * Prints likes/dislikes and if private, messages
     */
    void viewFeedback() const;
    /**
    * << Overload used to print user info -> name and if theyre streaming
    */
    friend std::ostream &operator<<(std::ostream &output, const Streamer& st);
    /**
    * Used to save to files
    * @return String with streamer info
    */
    std::string save() const;
    /**
    * Used to check for what type of user were dealing with when we have a User* pointer
    * @return "streamer"
    */
    std::string type() const;
    /**
     * Sets current active stream to a pointer, used in file loading
     * @param s Stream pointer
     */
    void setStream(Stream* s);
    /**
     * Adds a stream pointer to the history
     * @param s Stream pointer
     */
    void addToHistory(Stream* s);
    /**
     * Prints whitelist if streaming privately
     */
    void showWhiteList() const;
};

/**
 * Class for admins and their actions
 */
class Admin: public User {
public:
    /**
     * Admin constructor
     * @param n Name
     * @param nick Nickname
     * @param y Birth year
     * @param m Birth month
     * @param d Birth day
     */
    Admin(std::string n, std::string nick, int y, int m, int d);
    /**
     * << Overload for admin
     * @param output Ostream
     * @param ad Admin
     * @return returns ostream with admin nickname
     */
    friend std::ostream &operator<<(std::ostream &output, const Admin& ad);
    /**
    * Used to save to files
    * @return String with admin info
    */
    std::string save() const;
    /**
    * Used to check for what type of user were dealing with when we have a User* pointer
    * @return "admin"
    */
    std::string type() const;
};

#endif //AEDAPROJ_USERS_H
