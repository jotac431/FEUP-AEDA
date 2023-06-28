//
// Created by Duarte on 13/11/2020.
//

#ifndef AEDAPROJ_USERS_H
#define AEDAPROJ_USERS_H

enum status {
    ACTIVE,
    INACTIVE,
    REACTIVATED,
};

/*! \file users.h
    \brief Users header: contains the User and derived Viewer, Streamer and Admin classes.
*/

#include "streams.h"
#include <queue>
#include <sstream>
#include "BST.h"

/**
 * Base class for users(viewers, streamers and admins)
 */
class User {
protected:
    std::string name;
    Date bd;
    std::string nickname;
    status state;
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
    /**
    * Alternates between states
    */
    void updateState(status st);
    /**
     * Check activity
     * @return True if state = inactive
     */
    bool inactive() const;
    /**
     * Returns the proper status
     * @return Current state value of the users
     */
    status getState() const;
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

enum class streamerEvaluation{
    ONE, TWO, THREE, FOUR, FIVE
};

/**
 * Class for Donate to Streamers
 */
class Donate{
private:
    std::string streamer;
    float ammount;
    unsigned evaluation;
    unsigned donationID;
public:
    static unsigned lastId;
    /**
     * Donate constructor
     * @param s Streamer pointer
     * @param a Donation ammount
     * @param e Global evaluation
     */
    Donate(std::string streamer, float ammount, unsigned evaluation);
    /**
     *  Returns the streamer nickname
     * @return streamer std::string streamer
     */
    std::string getStreamer() const;
    /**
     * Returns ammount
     * @return float amount
     */
    float getAmmount() const;
    /**
     * Returns evaluation
     * @return streamerEvaluation (1 to 5);
     */
    unsigned getEvaluation() const;
    /**
     * Comparator for donations, for use with BST
     * @param d Donation
     * @return Orders first by amount, then evaluation, then id
     */
    bool operator<(const Donate &d) const;
    /**
     * << Overload used to print donation info -> name and amount and evaluation
     */
    friend std::ostream &operator<<(std::ostream &output, const Donate& dono);
    /**
     * Returns string with info to save for files
     * @return string, "donation \n streamer \n ammount evaluation"
     */
    std::string save() const;
};

/**
 * Class for Purchase orders from Viewers to Streamers
 */
class PurchaseOrder{
private:
    std::string viewer;
    int products;
    int disp;
public:
    /**
     * Purchase Order constructor
     * @param v Viewer pointer
     * @param p Number of products in order
     * @param d Level of disponibility
     */
    PurchaseOrder(std::string v, int p, int d): viewer(v), products(p), disp(d) {};
    /**
     * Operator used to compare Purchase Orders to use in priority queue
     */
    bool operator<(const PurchaseOrder& p) const;
    /**
     * Define the values of a purchase order
     * @param q quantity of products
     * @param d disponibility
     */
    void setCompra(int q, int d);
    /**
     * Print values of PurchaseOrder
     * @return string with values
     */
    std::string printCompra() const;
    /**
     *
     * @return products
     */
    int getProducts() const;
    /**
     *
     * @return disp
     */
    int getDisp() const;
    /**
     *
     * @return viewer
     */
    std::string getViewer() const;
    /**
     * Saves info for files
     * @return nick, then qnt and disp on next line
     */
    std::string save() const;
};

typedef std::priority_queue<PurchaseOrder> ViewerPQ;

/**
 * Class for streamers and their actions
 */
class Streamer: public User{
private:
    Stream* stream;
    std::vector<Stream*> streamHistory;
    ViewerPQ viewerMerch;
    int stockUsed = 0;
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
    /**
     *
     * @return viewerMerch priority_queue
     */
    ViewerPQ getViewerMerch();
    /**
     * Pushes Purchase Order to priority queue viewerMerch
     * @param p Purchase Order from viewer
     */
    void pushPurch(PurchaseOrder p);
    /**
     * Deletes Purchases Orders from priority queue based on suficient stock
     * @param stock Stock limit
     */
    void deletePurch(int stock);
    /**
     * Deletes Purchase Orders from priority queue from Viewer v
     * @param v Viewer
     */
    void deletePurch(std::string v);
    /**
     * Print a list of all Purchase Orders from a Streamer
     */
    void printPurches();
    /**
     * Returns stucked used in Purchase orders
     * @return stockUsed
     */
    int getStockUsed();
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
