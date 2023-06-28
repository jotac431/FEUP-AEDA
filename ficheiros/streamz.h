//
// Created by Duarte on 19/10/2020.
//


#ifndef AEDAPROJ_STREAMZ_H
#define AEDAPROJ_STREAMZ_H

/*! \file streamz.h
    \brief Streamz header. Contains the StreamZ class which serves as a platform for the user-streams interaction.
*/

#include <map>
#include <fstream>
#include "users.h"

/**
 * The main platform class, has lists for the 3 types of users
 */
class StreamZ {
private:
    std::list<Viewer *> viewerList;
    std::list<Streamer *> streamerList;
    std::list<Admin *> adminList;
public:
    /**
     * Checks if a given nickname has been used before
     * @return False if previously used, else true
     */
    bool uniqueNickname(std::string nick) const;
    /**
     * Constructs and adds a viewer to the list, checks if nickname is used, returns pointer
     * @param name Viewer real name
     * @param nick Viewer nickname
     * @param y Birth year
     * @param m Birth month
     * @param d Birth day
     * @ret Viewer pointer
     */
    Viewer* addViewer(std::string name, std::string nick, int y, int m, int d);
    /**
    * Constructs and adds a streamer to the list, checks if nickname is used, returns pointer
    * @param name Streamer real name
    * @param nick Streamer nickname
    * @param y Birth year
    * @param m Birth month
    * @param d Birth day
     * @ret Streamer pointer
    */
    Streamer* addStreamer(std::string name, std::string nick, int y, int m, int d);
    /**
    * Constructs and adds a admin to the list, checks if nickname is used, returns pointer
    * @param name Admin real name
    * @param nick Admin nickname
    * @param y Birth year
    * @param m Birth month
    * @param d Birth day
    * @ret Admin pointer
    */
    Admin* addAdmin(std::string name, std::string nick, int y, int m, int d);
    /**
    * Cycles trough all streamers to get all active streams
    * @return Vector with pointers to all active streams
    */
    std::vector<Stream *> activeStreams() const;
    /**
    * Cycles trough all streamers to get all inactive streams
    * @return Vector with pointers to all inactive streams
    */
    std::vector<Stream *> inactiveStreams() const;
    /**
    * Calls activeStreams() and then adds the stream archive streams too
    * @return Vector with all active and inactive streams
    */
    std::vector<Stream *> allStreams() const;
    /**
    * Prints a vector of streams
    * @param vec Vector to print
    */
    void printVector(const std::vector<Stream *> &vec) const;
    /**
    * Gets a pointer from a nickname
    * @param nick Nickname to find
    */
    User *select(std::string nick) const;
    /**
    * Gets a vector of stream pointers, removes streams that dont have language == lang
    * @param vec Vector to filter
    * @param lang Language to compare
    */
    void filterStreams(std::vector<Stream *> &vec, std::string lang) const;
    /**
    * Gets a vector of stream pointers, removes streams whose minAge > age;
    * @param vec Vector to filter
    * @param age Age to compare
    */
    void filterStreams(std::vector<Stream *> &vec, int age) const;
    /**
    * Sorts a vector of stream pointers based on number of likes
    * @param vec Vector to sort
    */
    static void sortLikes(std::vector<Stream *> &vec) ;
    /**
    * Sorts a vector of stream pointer based on number of final viewers
    * @param vec Vector to sort
    */
    static void sortViews(std::vector<Stream *> &vec) ;
    /**
    * Saves current state into a file that can be loaded
    * @param filepath File path to save in
    */
    void saveData(std::string filepath) const;
    /**
    * Loads current state from a file
    * @param filepath File path to load from
    */
    void loadData(std::string filepath);
    /**
    * Lists all streams that have the current language, active first, used by all users
    * @param language Language to search for
    */
    void search(std::string language) const;
    /**
     * Lists all streams that have a minimum age equal or below the current age, active first, used by all users
     * @param age Age parameter to search for
     */
    void search(int age) const;
    /**
     * Lists all streams that have a language and a minimum age equal or below the given age, used by all users,
     * @param language Language to search for
     * @param age Age parameter to search for
     */
    void search(std::string language, int age) const;
    /**
     * Lists all active streams, used by all users
     */
    void listActiveStreams() const;
    /**
     * Lists all active users, used by all users
     */
    void listUsers() const;
    /**
     * Lists the top 10 stream history by likes and views, used by all users
     */
    void listTop() const;
    /**
    * Displays total created streams, used by admins
    */
    void streamQuantity() const;
    /**
     * Displays average views, used by admins
     */
    void averageViews() const;
    /**
     * Counts number of private and public streams in a range, used by admins
     * @param d1 min date
     * @param d2 max age
     */
    void privatePublic(Date d1, Date d2) const;
    /**
     * Displays most common streams by type and language, used by admins
     */
    void mostCreated() const;
    /**
     * Displays most viewed streamer, used by admins
     */
    void mostViewed() const;
    /**
     * Tries to make a viewer watch a stream and handles exceptions
     * @param v Viewer to join stream
     * @param streamer Nick Streamer nickname that viewer will watch
     */
    void viewerJoinStream(Viewer* v, std::string streamerNick);
    /**
    * Deletes a user, ie removes them from the list, closes streams/leaves them and delete()
    * @param nick Nick of user to remove
    */
    void deleteUser(std::string nick);
};


#endif //AEDAPROJ_STREAMZ_H
