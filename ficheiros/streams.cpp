//
// Created by Duarte on 13/11/2020.
//

#include "users.h"
#include "streams.h"

/*! \file streams.cpp
    \brief Streams body.
*/

#define PURPLE 5
#define RED 4
#define GREEN 2
#define WHITE 7


Stream::Stream(std::string t, std::string lang, int age, Streamer* st) {
    title = t;
    minAge = age;
    language = lang;
    streamer = st;
    finalViewers = -1; //set to -1 while the stream is active, used in checks
    likes = 0;
    dislikes = 0;
    startDate = Date(); //start date will default to the current pc date (when the stream starts)
}
Stream::Stream(std::string t, std::string lang, int age, Streamer* st, int y, int m, int d, int l, int dl) {
    title = t;
    minAge = age;
    language = lang;
    streamer = st;
    finalViewers = -1; //set to -1 while the stream is active, used in checks
    likes = l;
    dislikes = dl;
    Date date(y, m, d);
    startDate = date;
}
Stream::Stream(std::string t, std::string lang, int age, Streamer* st, int y, int m, int d, int l, int dl, int views) {
    title = t;
    minAge = age;
    language = lang;
    streamer = st;
    finalViewers = views;
    likes = l;
    dislikes = dl;
    Date date(y, m, d);
    startDate = date;
}
void Stream::close() {
    finalViewers = viewers.size(); //stream is over, we can set final viewers
    while(!viewers.empty()) { //well remove viewers until theres nothing to remove
        viewers.front()->leaveStream(); //viewer at the top leaves
    }
}
void Stream::info() const{
    infoHeader();
    if(finalViewers == -1) { //this lets us check if a stream is still running
        std::cout << "\n" << viewers.size() << " viewers\n";
    } else { //and print the type of viewers accordingly
        std::cout << "\n" << finalViewers << " final viewers\n";
    }
    likesDisplay();
}
void Stream::likesDisplay() const {
    if(likes == 0 && dislikes == 0) { //if theres nothing yet, we print 0 .......... 0
        Color(PURPLE);std::cout << "Likes: ";Color(WHITE);
        std::cout << "0 .......... 0";
        Color(PURPLE);std::cout << " :Dislikes\n";Color(WHITE);
        return;
    }
    double likePercent = (double)likes/(double)(likes+dislikes); //we print like 7 +++++++--- 3 with colours
    double dislikePercent = (double)dislikes/(double)(likes+dislikes);
    int plusAmount = round(likePercent*10);
    int minusAmount = round(dislikePercent*10); //how many +'s and -'s to print
    Color(PURPLE);std:: cout << "Likes: ";Color(WHITE);std::cout << likes << " ";
    Color(GREEN);
    while(plusAmount != 0) {
        std::cout << "+";
        plusAmount--;
    }
    Color(RED);
    while(minusAmount != 0) {
        std::cout << "-";
        minusAmount--;
    }
    Color(WHITE);std::cout << " " << dislikes;Color(PURPLE);std::cout << " :Dislikes\n";Color(WHITE);
}
void Stream::infoHeader() const {
    Color(PURPLE);std::cout << "Stream: \"" ;
    Color(WHITE);std::cout << title;
    Color(PURPLE);std::cout << "\" Streamer: " ;
    Color(WHITE);std::cout << streamer->getNick();
    Color(PURPLE);std::cout << "\nLanguage: ";
    Color(WHITE);std::cout << language;
    Color(PURPLE);std::cout << " Minimum age: ";
    Color(WHITE);std::cout << minAge;
}
int Stream::ageLimit() const {
    return minAge; //simply return minAge
}
std::string Stream::getLanguage() const {
    return language; //just return lang
}
void Stream::addViewer(Viewer *viewer) {
    viewers.push_back(viewer); //simply add the viewer pointer to the list
}
void Stream::removeViewer(Viewer *viewer) { //removing a viewer by pointer
    auto pos = sequentialSearch(viewers, viewer); //search
    if(pos == viewers.end()) { //not found
        throw UserNotFound();
    } else { //found, remove
        viewers.erase(pos);
    }
}
int Stream::getViewerCount() const{
    if(finalViewers != -1) { //finalviewers == -1 if stream still active
        return finalViewers;
    } else { //if its not != -1, its active, return the size of the viewer vector
        return viewers.size();
    }
}
int Stream::getLikes() const {
    return likes; //return likes
}
bool Stream::isPrivate() const { return false; } //for Stream, returns false
void Stream::feedback(bool positive) {
    if(positive) { //add like or dislike depending on bool
        likes++;
    } else {
        dislikes++;
    }
}
void Stream::printFeedback() const {
    Color(PURPLE);std::cout << "Stream Feedback:\n";Color(WHITE);
    likesDisplay();
}
std::string Stream::streamerName() const {
    return streamer->getNick(); //get the nickname of the streamer whos streaming this
}
Date Stream::getDate() const {
    return startDate; //just return it
}
std::string Stream::save() const {
    std::string out; //were gonna build a string that will be saved to the file
    out += "stream\n" + title + "\n" + language + "\n" + std::to_string(minAge) + " " + std::to_string(startDate.year) +
           " " + std::to_string(startDate.month) + " " + std::to_string(startDate.day) + " " +
           std::to_string(likes) + " " + std::to_string(dislikes);
    if(finalViewers != -1) { //if its over, we attach "old" to the "stream" and add the viewers
        out = "old" + out + " " + std::to_string(finalViewers);
    }
    out += "\n";
    return out;
}

PrivateStream::PrivateStream(std::string t, std::string lang, int age, Streamer *st, int capacity)
        : Stream(t, lang, age, st) {
    this->capacity = capacity;
}
PrivateStream::PrivateStream(std::string t, std::string lang, int age, Streamer *st, int capacity, int y, int m, int d,
                             int l, int dl)
        : Stream(t, lang, age, st, y, m, d, l, dl) {
    this->capacity = capacity;
}
PrivateStream::PrivateStream(std::string t, std::string lang, int age, Streamer *st, int capacity, int y, int m, int d,
                             int l, int dl, int views)
        : Stream(t, lang, age, st, y, m, d, l, dl, views) {
    this->capacity = capacity;
}
bool PrivateStream::userAllowed(std::string n) const {
    n = lowercase(n); //search normalized to lowercase
    if(sequentialSearch(whitelist, n) != whitelist.end()) { //is it there
        return true; //yay
    }
    return false; //its not there
}
bool PrivateStream::isFull() const {
    return (capacity == viewers.size()); //will be true if capacity is reached
}
void PrivateStream::info() const { //has priority, stream::info is virtual
    Color(PURPLE);std::cout<<"Private ";
    infoHeader();
    if(finalViewers == -1) { //this lets us check if a stream is still running
        std::cout << "\n" << viewers.size() << " out of " << capacity << " viewers\n";
    } else { //and print the type of viewers accordingly
        std::cout << "\n" << finalViewers << " final viewers\n";
    }
    likesDisplay();
}

void PrivateStream::addToList(std::string nick) {
    whitelist.push_back(lowercase(nick)); //add a nick to the whitelist (its lowercased)
}
void PrivateStream::privateFeedback(std::string message) {
    feedback.push_back(message); //add a feedback message to feedback
}
void PrivateStream::removeFromList(std::string nick) {
    nick = lowercase(nick);
    auto pos = sequentialSearch(whitelist, nick); //find the nick
    if(pos == whitelist.end()) { //if its not found throw
        throw UserNotFound();
    } else { //otherwise get it out
        whitelist.erase(pos);
        //if theyre in the stream they gotta leave too
        for(auto viewer: viewers) {
            if(lowercase(viewer->getNick()) == nick) {
                viewer->leaveStream();
                return;
            }
        }
    }
}
bool PrivateStream::isPrivate() const { return true; } //for PrivateStream always true
void PrivateStream::printFeedback() const {
    Color(PURPLE);std::cout << "Stream Feedback:\n";Color(WHITE);
    likesDisplay(); //not only print likes
    for(const auto& message: feedback) { //but also the messages
        std::cout << message << "\n";
    }
}
std::string PrivateStream::save() const {
    std::string out;
    out += "privatestream\n" + title + "\n" + language + "\n" + std::to_string(minAge) + " "
           + std::to_string(startDate.year) + " " + std::to_string(startDate.month) + " " +
           std::to_string(startDate.day) + " " + std::to_string(likes) + " " + std::to_string(dislikes) +
           " " + std::to_string(capacity);
    if(finalViewers != -1) {
        out = "old" + out + " " + std::to_string(finalViewers);
    } //to this point its equal to Stream::save();
    out += "\n";
    if(!feedback.empty()) {
        out += "feedback";
        for (auto message: feedback) {
            out += "\n" + message;
        }
        out += "\nfeedbackend\n";
    } //but we to store the feedback messages
    if(!whitelist.empty()) {
        out += "whitelist";
        for (auto nick: whitelist) {
            out += "\n" + nick;
        }
        out += "\nwhitelistend\n";
    } //as well as the nicknames in the whitelist
    return out;
}

std::list<std::string> PrivateStream::getWhiteList() const{
    return whitelist;
}

