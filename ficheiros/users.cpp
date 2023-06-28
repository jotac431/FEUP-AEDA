//
// Created by Duarte on 13/11/2020.
//

/*! \file users.cpp
    \brief Users body.
*/

#include "users.h"

User::User(std::string n, std::string nick, int y, int m, int d): bd(y, m, d) {
    name = n;
    nickname = nick;
}

int User::getAge() const{
    return bd.age(); //get the user age, call age() on his birthday
}
bool operator<=(const Date& d1, const Date& d2) {
    if(d1.year > d2.year) {
        return false;
    } else if (d1.year == d2.year && d1.month > d2.month) {
        return false;
    } else if (d1.year == d2.year && d1.month == d2.month && d1.day > d2.day) {
        return false;
    }
    return true;
}

std::string User::getNick() const{
    return nickname; //get the user nickname, simply return it
}

Viewer::Viewer(std::string n, std::string nick, int y, int m, int d): User(n, nick, y, m, d) {
    if(this->getAge() <= 12) { //user must be above 12 to join
        throw ViewerTooYoung(this->getAge());
    }
    watchingStream = nullptr; //if hes not watching, the stream pointer is set to nullptr
}
void Viewer::watchStream(Stream* s) {
    watchingStream = s;
}
void Viewer::leaveStream() {
    if(watching()) { //are we even in a stream?
        watchingStream->removeViewer(this); //ok lets dropout
        watchingStream = nullptr; //reset our pointer
        return;
    } else {
        throw ViewerNotWatching();
    }
}
bool Viewer::watching() const {return(watchingStream!= nullptr);}
void Viewer::publicFeedback(bool positive) {
    if(watching()) {
        watchingStream->feedback(positive);
    } else {
        throw(ViewerNotWatching());
    }
}
void Viewer::privateFeedback(const std::string& message) {
    if(watching() && watchingStream->isPrivate()) {
        std::string newMessage; //new message is created, corresponds to "Nick: message";
        newMessage += this->getNick();
        newMessage += ": ";
        newMessage += message;
        PrivateStream* privPointer = dynamic_cast<PrivateStream *>(watchingStream);
        privPointer->privateFeedback(newMessage);
    } else {
        throw(ViewerNotWatching());
    }
}
std::ostream &operator<<(std::ostream &output, const Viewer& vw) {
    output << vw.nickname;
    if(vw.watching()) {
        output << " - watching " << vw.watchingStream->streamerName() << "\n";
    } else {
        output << "\n";
    }
    return output;
}
std::string Viewer::save() const {
    std::string out;
    out += "viewer\n" + name + "\n" + nickname + "\n";
    out += std::to_string(bd.year) + " " + std::to_string(bd.month) + " " + std::to_string(bd.day);
    out += "\n";
    if(watching()) {
        out += "watching\n" + watchingStream->streamerName() + "\n";
    }
    return out;
}
std::string Viewer::type() const {return "viewer";}

Streamer::Streamer(std::string n, std::string nick, int y, int m, int d): User(n, nick, y, m , d) {
    if(this->getAge() <= 15) { //streamer must be above 16
        throw StreamerTooYoung(this->getAge());
    }
    stream = nullptr; //stream is nullptr when theyre not streaming
}
Stream* Streamer::getStream() const {
    return stream; //just return it
}
std::vector<Stream*> Streamer::getStreamHistory() const {
    return streamHistory;
}
void Streamer::createStream(std::string n, std::string lang, int age) {
    if(streaming()) { //first check if theyre already streaming
        throw StreamerAlreadyStreaming();
    } else { //ok we can create
        stream = new Stream(n, firstUpper(lang), age, this);
    }
}
void Streamer::createPrivateStream(std::string n, std::string lang, int age, int capacity){
    if(streaming()) { //see above
        throw StreamerAlreadyStreaming();
    } else {
        stream = new PrivateStream(n, firstUpper(lang), age, this, capacity);
    }
}
void Streamer::endStream() {
    if(!streaming()) { //cant end a stream if hes not streaming
        throw StreamerNotStreaming();
    } else {
        stream->close(); //close it
        addToHistory(stream); //add it to history
        stream = nullptr; //reset the pointer
    }
}
void Streamer::checkViewers() {
    if(!streaming()) {
        throw StreamerNotStreaming();
    } else {
        std::cout << stream->getViewerCount();
    }
    std::cout << " viewers currently watching.\n";
}
void Streamer::allowPrivate(std::string nick) {
    if(streaming() && stream->isPrivate()) {
        PrivateStream* privPointer = dynamic_cast<PrivateStream *>(stream);
        privPointer->addToList(nick);
    } else {
        throw StreamerNotStreaming();
    }
}
void Streamer::removePrivate(std::string nick) {
    if(streaming() && stream->isPrivate()) {
        PrivateStream* privPointer = dynamic_cast<PrivateStream *>(stream);
        privPointer->removeFromList(nick);
    } else {
        throw StreamerNotStreaming();
    }
}
bool Streamer::streaming() const {return(stream != nullptr);}
void Streamer::viewFeedback() const {
    if(streaming()) {
        stream->printFeedback();
    } else {
        throw(StreamerNotStreaming());
    }
}
std::ostream &operator<<(std::ostream &output, const Streamer& st) {
    output << st.nickname;
    if(st.streaming()) {
        output << " - streaming\n";
    } else {
        output << "\n";
    }
    return output;
}
std::string Streamer::save() const {
    std::string out;
    out += "streamer\n" + name + "\n" + nickname + "\n";
    out += std::to_string(bd.year) + " " + std::to_string(bd.month) + " " + std::to_string(bd.day);
    out += "\n";
    if(streaming()) {
        out += stream->save();
    }
    for(auto oldStream: streamHistory) {
        out += oldStream->save();
    }
    return out;
}
std::string Streamer::type() const { return "streamer";}
void Streamer::setStream(Stream *s) {stream = s;}
void Streamer::addToHistory(Stream *s) {streamHistory.push_back(s);}

void Streamer::showWhiteList() const{
    if(!streaming()) {
        throw StreamerNotStreaming();
    }
    else if (!stream->isPrivate()){
        throw StreamNotPrivate();
        }
    else {
        PrivateStream* s = dynamic_cast<PrivateStream *>(stream);
        std::cout << "\nWhitelist:\n";
        if(s->getWhiteList().empty()) {
            std::cout << "empty\n";
        }
        for (auto nick : s->getWhiteList()) {
            std::cout << nick << std::endl;
        }
    }
}

Admin::Admin(std::string n, std::string nick, int y, int m, int d): User(n, nick, y, m , d) {
}
std::string Admin::save() const {
    std::string out;
    out += "admin\n" + name + "\n" + nickname + "\n";
    out += std::to_string(bd.year) + " " + std::to_string(bd.month) + " " + std::to_string(bd.day);
    out += "\n";
    return out;
}
std::ostream &operator<<(std::ostream &output, const Admin& ad) {
    output << ad.nickname << "\n";
    return output;
}
std::string Admin::type() const { return "admin";}