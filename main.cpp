#include <iostream>
#include <cstdlib>
#include "streamz.h"

/*! \file main.cpp
    \brief Main project file, contains functions to run the menus.
*/

/**
 * Command enum to be used in switches
 */
enum commands {
    cload,
    csave,
    cstreamer,
    cviewer,
    cadmin,
    cselection,
    cremove,
    cclose,
    cback,
    cusers, //common
    ctop,
    csearch,
    clist,
    cjoin, //viewer
    cleave,
    cfeedback,
    cdonate,
    ccreate, //streamer
    cendstr,
    cviewers,
    cwhitelist,
    cviewfeed,
    cstrdonoview,
    cquant, //admin
    cavg,
    cprivatepublic,
    cmostcreated,
    cmostviewed,
    cshowwt,
    cstreamhistory,
    deact,
    react,
    cpurch,
    cpurchdisplay,
    climit,
    cdeletpurch,
    crating,
    crange,
    camount,
    cnull
};

/**
 * Turns strings into enums that can be used in switch statements
 * @param string Received command
 * @return Returned enum command
 */
commands hashit(std::string const& string) {
    if(lowercase(string) == "load") return cload;
    if(lowercase(string) == "save") return csave;
    if(lowercase(string) == "streamer") return cstreamer;
    if(lowercase(string) == "viewer") return cviewer;
    if(lowercase(string) == "admin") return cadmin;
    if(lowercase(string) == "selection") return cselection;
    if(lowercase(string) == "remove") return cremove;
    if(lowercase(string) == "close") return cclose;
    if(lowercase(string) == "back") return cback;
    if(lowercase(string) == "users") return cusers;
    if(lowercase(string) == "top") return ctop;
    if(lowercase(string) == "search") return csearch;
    if(lowercase(string) == "list") return clist;
    if(lowercase(string) == "join") return cjoin;
    if(lowercase(string) == "leave") return cleave;
    if(lowercase(string) == "feedback") return cfeedback;
    if(lowercase(string) == "create") return ccreate;
    if(lowercase(string) == "end") return cendstr;
    if(lowercase(string) == "viewers") return cviewers;
    if(lowercase(string) == "whitelist") return cwhitelist;
    if(lowercase(string) == "viewfeed") return cviewfeed;
    if(lowercase(string) == "quantity") return cquant;
    if(lowercase(string) == "average") return cavg;
    if(lowercase(string) == "type") return cprivatepublic;
    if(lowercase(string) == "created") return cmostcreated;
    if(lowercase(string) == "viewed") return cmostviewed;
    if(lowercase(string) == "showwt") return cshowwt;
    if(lowercase(string) == "streamhistory") return cstreamhistory;
    if(lowercase(string) == "deactivate") return deact;
    if(lowercase(string) == "reactivate") return react;
    if(lowercase(string) == "purch") return cpurch;
    if(lowercase(string) == "purchdisplay") return cpurchdisplay;
    if(lowercase(string) == "limit") return climit;
    if(lowercase(string) == "deletpurch") return cdeletpurch;
    if(lowercase(string) == "donate") return cdonate;
    if(lowercase(string) == "donations") return cstrdonoview;
    if(lowercase(string) == "range") return crange;
    if(lowercase(string) == "amount") return camount;
    if(lowercase(string) == "rating") return crating;
    return cnull;
}

/**
 * Gets and processes a date
 * @return Date class
 */
Date dateParser() {
    std::string data, ys, ms, ds;
    char pull;
    int y, m, d, pos = 0, count = 0;
    fflush(stdin);
    getline(std::cin, data);
    if(data.empty()) {
        throw BadDate();
    }
    for(auto chr: data) {
        if(chr == '/') {
            count++;
        }
    }
    if(count != 2) {
        throw BadDate();
    }
    pull = data[pos];
    while(pull != '/') {
        ys.push_back(pull);
        pos++;
        pull = data[pos];
    }
    pos++;
    pull = data[pos];
    while(pull != '/') {
        ms.push_back(pull);
        pos++;
        pull = data[pos];
    }
    pos++;
    pull = data[pos];
    while(pos < data.size()) {
        ds.push_back(pull);
        pos++;
        pull = data[pos];
    }
    y = std::stoi(ys);m = std::stoi(ms);d = std::stoi(ds);
    return Date(y, m, d);
}

/**
 * Halts execution until enter is entered
 */
void wait() {
    std::cout << "\nPress enter twice to continue";
    fflush(stdin);
    std::cin.get();
}

/**
 * Waits 1 second, then prints many newlines to clear the console
 */
void clear() {
    _sleep(1000);
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

/**
 * Handles receiving a path and loading from it
 * @param sz Streamz Platform
 */
void load(StreamZ &sz) {
    clear();
    std::string path;
    std::cout << "Filepath: ";
    std::cin >> path;
    if(std::cin.fail()) {
        std::cout << "Bad input";
        return;
    } else {
        try {
            sz.loadData(path);
            std::cout << "Loaded";
        } catch(BadFile& e) {
            std::cout << "Bad file";
            return;
        } catch(NickIsTaken &e) {
            std::cout << "Conflict: tried to load user with nick " << e.getConflict() << " but they're already defined";
        }
    }
}

/**
 * Handles receiving a path and saving to it
 * @param sz StreamZ platform
 */
void save(StreamZ &sz) {
    clear();
    std::string path;
    std::cout << "Filepath: ";
    std::cin >> path;
    if(std::cin.fail()) {
        std::cout << "Bad input";
        return;
    } else {
        sz.saveData(path);
        std::cout << "Saved";
    }
}

/**
 * Handles getting parameters to call a search function
 * @param sz StreamZ platform
 */
void search(StreamZ &sz) {
    std::string type, lang;
    int age;
    std::cout << "Search by language - lang\nSearch by age - age\nSearch by both - both\n";
    if(std::cin >> type) {
        type = lowercase(type);
        if(type != "lang" && type != "age" && type != "both") {
            std::cout << "Bad command";
            return;
        }
        if(type == "both" || type == "lang") {
            fflush(stdin);
            std::cout << "What language to search for?\n";
            std::getline(std::cin, lang);
        }
        if(type == "both" || type == "age") {
            std::cout << "What maximum age limit to search for?\n";
            std::cin >> age;
            if(std::cin.fail()) {
                std::cout << "Bad input";
                return;
            }
        }
    } else {
        std::cout << "Bad input";
        return;
    }
    try {
        if (type == "lang") {
            sz.search(lang);
        } else if (type == "age") {
            sz.search(age);
        } else if (type == "both") {
            sz.search(lang, age);
        }
        wait();
    } catch (NoStreamsFound &e) {
        std::cout << "Couldn't find any streams";
    }
}

/**
 * Handles getting the streamer nickname which the viewer wants to join
 * @param sz StreamZ platform
 * @param v Viewer pointer
 */
void join(StreamZ &sz, Viewer* v) {
    std::string nick;
    std::cout << "Which streamer to watch (by nickname)?\n";
    fflush(stdin);
    std::getline(std::cin, nick);
    try {
        sz.viewerJoinStream(v, nick);
    } catch(UserNotFound &e) {
        std::cout << "Could not find " << nick;
        return;
    } catch(AccountNotActive &e) {
        std::cout << "Selected streamer is not active";
        return;
    } catch(TooYoungForStream &e) {
        std::cout << "Too young: Viewer is " << v->getAge() << ", stream minimum is " << e.ageLimit();
        return;
    } catch(PrivateStreamFull &e) {
        std::cout << "The stream is private and it has reached capacity";
        return;
    } catch(NotInWhitelist &e) {
        std::cout << "The stream is private and viewer is not in whitelist";
        return;
    } catch(StreamerNotStreaming &e) {
        std::cout << nick << " is not streaming";
        return;
    } catch(ViewerAlreadyWatching &e) {
        std::cout << "Viewer is already in a stream";
        return;
    }
    std::cout << "Joined stream";
}

/**
 * Handles getting parameters and calling feedback functions
 * @param v Viewer pointer
 */
void feedback(Viewer* v) {
    std::string command;
    std::cout << "Public feedback(like or dislike) - public\nPrivate feedback(messages) - private\n";
    if(std::cin >> command) {
        command = lowercase(command);
        if(command == "public") {
            std::cout << "Give a like - like\nGive a dislike - dislike\n";
            if(std::cin >> command) {
                command = lowercase(command);
                bool like;
                if(command == "like") {
                    like = true;
                } else if (command == "dislike") {
                    like = false;
                } else {
                    std::cout << "Bad command";
                    return;
                }
                try {
                    v->publicFeedback(like);
                } catch(ViewerNotWatching &e) {
                    std::cout << "Couldn't give feedback: Not in a stream";
                    return;
                }
                std::cout << "Gave feedback";
                return;
            } else {
                std::cout << "Bad input";
                return;
            }
        } else if(command == "private") {
            std::string message;
            std::cout << "Input message to send\n";
            fflush(stdin);
            std::getline(std::cin, message);
            try {
                v->privateFeedback(message);
            } catch(ViewerNotWatching &e) {
                std::cout << "Couldn't give feedback: Not in a private stream";
                return;
            }
            std::cout << "Gave feedback";
            return;
        } else {
            std::cout << "Bad command";
            return;
        }
    } else {
        std::cout << "Bad input";
        return;
    }
}

/**
 * Handles getting args for purchasing requests/deletions
 * @param sz StreamZ container
 * @param v Viewer pointer
 * @param delet wether to delete or create
 */
void purchHandler(StreamZ &sz, Viewer* v, bool delet) {
    std::string streamerNick;
    std::cout << "\nStreamer Nickname: ";
    fflush(stdin);
    std::getline(std::cin, streamerNick);
    if(delet) {
        sz.deletOrder(v->getNick(), streamerNick);
        return;
    }
    int qnt, disp;
    std::cout << "\nQuantity of products to buy: ";
    std::cin >> qnt;
    std::cout << "\nNumber your disponibility from 1 to 5: ";
    std::cin >> disp;
    sz.purchOrder(v->getNick(), streamerNick, qnt, disp);
}

/**
 * Viewer menu handling their display and commands
 * @return If it returns true, the returns to main, otherwise, it loops and restarts
 */
bool viewerMenu(StreamZ &sz, Viewer* v) {
    std::cout << "Viewer " << v->getNick() << "\n\n";
    std::cout << "Commands:\nList users - users\nList top 10 streams in history - top\nSearch streams - search"
                 "\nList active streams - list\nJoin stream - join\n"
                 "Leave stream - leave\nGive feedback - feedback\n"
                 "Make a Purchase Order - purch\nDelete Purchases Orders from Streamer - deletpurch\n"
                 "Give a streamer a donation - donate\nReturn to main menu - back\n\n";
    std::string command;
    if(std::cin >> command) {
        switch (hashit(command)) {
            case cusers:
                sz.listUsers(true);
                wait();
                clear();
                return false;
            case ctop:
                try {
                    sz.listTop();
                    wait();
                } catch (NoStreamsFound &e) {
                    std::cout << "Can't list, no streams found";
                }
                clear();
                return false;
            case csearch:
                search(sz);
                clear();
                return false;
            case clist:
                try {
                    sz.listActiveStreams();
                } catch (NoStreamsFound &e) {
                    std::cout << "There are no active streams";
                }
                wait();
                clear();
                return false;
            case cjoin:
                join(sz, v);
                clear();
                return false;
            case cleave:
                try {
                    v->leaveStream();
                } catch (ViewerNotWatching &e) {
                    std::cout << "Can't leave stream: Not watching any";
                }
                clear();
                return false;
            case cfeedback:
                feedback(v);
                clear();
                return false;
            case cback:
                return true;
            case cpurch:
                try {
                    purchHandler(sz, v, false);
                } catch (UserNotFound &e) {
                    std::cout << "Can´t place purchase order: Streamer not found";
                } catch (StockNotEnough &e) {
                    std::cout << "Can´t place purchase order: Stock is not enough";
                } catch (NotInRange &e) {
                    std::cout << "Can´t place purchase order: Disponibility level is not in given range";
                }
                clear();
                return false;
            case cdeletpurch:
                try {
                    purchHandler(sz, v, true);
                } catch (UserNotFound &e) {
                    std::cout << "Can´t delete purchase order: Streamer not found";
                }
                clear();
                return false;
            case cdonate: {
                std::string streamerNick;
                float am;
                unsigned eval;
                std::cout << "\nStreamer Nickname: ";
                fflush(stdin);
                std::getline(std::cin, streamerNick);
                std::cout << "\nHow much to donate: ";
                std::cin >> am;
                std::cout << "\nHow do you rate the streamer (1-5): ";
                std::cin >> eval;
                try {
                    sz.addDonation(streamerNick, am, eval);
                    std::cout << "Donated :)";
                } catch (InvalidEvaluation &e) {
                    std::cout << "Invalid evaluation, must be 1 to 5";
                } catch (InvalidDonationValue &e) {
                    std::cout << "Invalid donation, must be positive";
                } catch (UserNotFound &e) {
                    std::cout << "Couldn't find active streamer with that nickname";
                }
                clear();
                return false;
            }
            default:
                std::cout << "Bad command";
                clear();
                return false;
        }
    } else {
        std::cout << "Bad input";
        clear();
        return false;
    }
}

/**
 * Handles stream creation
 * @param s Streamer pointer
 */
void createStream(Streamer *s) {
    std::string command;
    std::cout << "Public stream - public\nPrivate stream - private\n";
    if(std::cin >> command) {
        command = lowercase(command);
        if(command == "public" || command == "private") {
            std::string title, language;
            int age;
            std::cout << "Stream title: ";
            fflush(stdin);
            std::getline(std::cin, title);
            std::cout << "Stream language: ";
            fflush(stdin);
            std::getline(std::cin, language);
            std::cout << "Stream minimum age: ";
            if(std::cin >> age) {
                if(command == "private") {
                    int capacity;
                    std::cout << "Stream max capacity: ";
                    if(std::cin >> capacity) {
                        try {
                            s->createPrivateStream(title, language, age, capacity);
                        } catch (StreamerAlreadyStreaming &e) {
                            std::cout << "Can't create private stream: already streaming";
                            return;
                        }
                    } else {
                        std::cout << "Bad input";
                        return;
                    }
                } else {
                    try {
                        s->createStream(title, language, age);
                    } catch (StreamerAlreadyStreaming &e) {
                        std::cout << "Can't create stream: Already streaming";
                        return;
                    }
                }
                std::cout << "Created";
                return;
            } else {
                std::cout << "Bad input";
                return;
            }
        } else {
            std::cout << "Bad command";
            return;
        }
    } else {
        std::cout << "Bad input";
        return;
    }
}

/**
 * Handles adding or removing from whitelist
 * @param s Streamer pointer
 */
void whitelist(Streamer* s) {
    std::string command;
    std::cout << "Add to the whitelist - add\nRemove from whitelist - remove\n";
    if(std::cin >> command) {
        command = lowercase(command);
        if(command == "add" || command == "remove") {
            std::string nickname;
            std::cout << "Who to add/remove: ";
            fflush(stdin);
            std::getline(std::cin, nickname);
            try {
                if(command == "add") {
                    s->allowPrivate(nickname);
                } else {
                    s->removePrivate(nickname);
                }
                std::cout << "Done";
                return;
            } catch (StreamerNotStreaming &e) {
                std::cout << "Can't update whitelist: Streamer not streaming privately";
                return;
            } catch(UserNotFound &e) {
                std::cout << "Can't remove nickname from whitelist: It's not in it.";
                return;
            }
        } else {
            std::cout << "Bad command";
            return;
        }
    } else {
        std::cout << "Bad input";
        return;
    }
}

/**
 * Streamer menu handling their display and commands
 * @param sz StreamZ platform
 * @param s Streamer pointer
 * @return If it returns true, the returns to main, otherwise, it loops and restarts
 */
bool streamerMenu(StreamZ &sz, Streamer* s) {
    std::cout << "Streamer " << s->getNick() << "\n\n";
    std::cout << "Commands:\nList users - users\nList top 10 streams in history - top"
                 "\nSearch streams - search\nList active streams - list\nCreate stream - create\n"
                 "End stream - end\nCheck viewer count - viewers\nManage whitelist - whitelist\n"
                 "Show whitelist - showwt\nShow stream history - streamhistory\n"
                 "View feedback - viewfeed\nList of Purchase Orders - purchdisplay"
                 "\nList donations received - donations\nReturn to main menu - back\n\n";
    if(s->getState() == REACTIVATED) {
        Color(5);
        std::cout << "You have recently reactivated your account! Start a stream for a bonus!\n\n";
        Color(7);
    }
    std::string command;
    if(std::cin >> command) {
        switch (hashit(command)) {
            case cusers:
                sz.listUsers(true);
                wait();
                clear();
                return false;
            case ctop:
                try {
                    sz.listTop();
                    wait();
                } catch (NoStreamsFound &e) {
                    std::cout << "Can't list, no streams found";
                }
                clear();
                return false;
            case csearch:
                search(sz);
                clear();
                return false;
            case clist:
                try {
                    sz.listActiveStreams();
                    wait();
                } catch (NoStreamsFound &e) {
                    std::cout << "There are no active streams";
                }
                clear();
                return false;
            case ccreate:
                createStream(s);
                clear();
                return false;
            case cendstr:
                try {
                    s->endStream();
                } catch (StreamerNotStreaming &e) {
                    std::cout << "Can't end stream: Not streaming";
                }
                clear();
                return false;
            case cviewers:
                try {
                    s->checkViewers();
                    wait();
                } catch (StreamerNotStreaming &e) {
                    std::cout << "Can't check viewers: Not streaming";
                }
                clear();
                return false;
            case cwhitelist:
                whitelist(s);
                clear();
                return false;
            case cshowwt:
                try {
                    s->showWhiteList();
                    wait();
                } catch (StreamerNotStreaming &e) {
                    std::cout << "Can't check whitelist: Not streaming";
                } catch (StreamNotPrivate &e) {
                    std::cout << "Can't show whitelist: Stream is not private";
                }
                clear();
                return false;
            case cstreamhistory:
                try {
                    sz.printVector(s->getStreamHistory());
                    wait();
                } catch (NoStreamsFound &e) {
                    std::cout << "Can't show stream history list: No streams found";
                }
                clear();
                return false;
            case cviewfeed:
                try {
                    s->viewFeedback();
                    wait();
                } catch (StreamerNotStreaming &e) {
                    std::cout << "Can't view feedback: Not streaming";
                }
                clear();
                return false;
            case cpurchdisplay:
                s->printPurches();
                wait();
                clear();
                return false;
            case cback:
                return true;
            case cstrdonoview:
                sz.listDonoStreamer(s->getNick());
                wait();
                clear();
                return false;
            default:
                std::cout << "Bad command";
                clear();
                return false;
        }
    } else {
        std::cout << "Bad input";
        clear();
        return false;
    }
}

/**
 * Gets the dates to do a private-public in a given range search
 * @param zs StreamZ platform
 */
void privatepublic(StreamZ &zs) {
    try {
        std::cout << "Date 1 (YYYY/MM/DD): ";
        Date d1 = dateParser();
        std::cout << "Date 2 (YYYY/MM/DD): ";
        Date d2 = dateParser();
        zs.privatePublic(d1, d2);
        wait();
    } catch (BadDate &e) {
        std::cout << "Bad date";
    }
}

/**
 * Admin menu handling their display and commands
 * @param sz StreamZ
 * @param a Admin pointer
 * @return If it returns true, the returns to main, otherwise, it loops and restarts
 */
bool adminMenu(StreamZ &sz, Admin* a) {
    std::cout << "Admin " << a->getNick() << "\n\n";
    std::cout << "Commands:\nList users - users\nList top 10 streams in history - top"
                 "\nSearch streams - search\nList active streams - list\nSee viewer average - average\n"
                 "See quantity of created streams - quantity\n"
                 "See quantity of public/private streams in a date range - type\n"
                 "See most created types of stream - created\nSee most viewed streamer - viewed\n"
                 "Set stock limit - limit\nSee donations in range - range\n"
                 "See donations listed by amount - amount\n"
                 "See donations listed by rating - rating\nReturn to main menu - back\n\n";
    std::string command;
    if(std::cin >> command) {
        switch (hashit(command)) {
            case cusers:
                sz.listUsers(false);
                wait();
                clear();
                return false;
            case ctop:
                try {
                    sz.listTop();
                    wait();
                } catch (NoStreamsFound &e) {
                    std::cout << "Can't list, no streams found";
                }
                clear();
                return false;
            case csearch:
                search(sz);
                wait();
                clear();
                return false;
            case clist:
                try {
                    sz.listActiveStreams();
                    wait();
                } catch (NoStreamsFound &e) {
                    std::cout << "There are no active streams";
                }
                clear();
                return false;
            case cavg:
                try {
                    sz.averageViews();
                    wait();
                } catch (NoStreamsFound &e) {
                    std::cout << "There are no active streams";
                }
                clear();
                return false;
            case cquant:
                sz.streamQuantity();
                wait();
                clear();
                return false;
            case cprivatepublic:
                privatepublic(sz);
                clear();
                return false;
            case cmostcreated:
                try {
                    sz.mostCreated();
                    wait();
                } catch (NoStreamsFound &e) {
                    std::cout << "No streams have been created yet";
                }
                clear();
                return false;
            case cmostviewed:
                try {
                    sz.mostViewed();
                    wait();
                } catch (UserNotFound &e) {
                    std::cout << "There are no streamers";
                }
                clear();
                return false;
            case climit:
                try {
                    int limit;
                    std::cout << "\nStock limit : ";
                    std::cin >> limit;
                    sz.setMerchLimit(limit);
                } catch (NotNumber &e) {
                    std::cout << "Not a number";
                }
                clear();
                return false;
            case cback:
                clear();
                return true;
            case crange:
                float lower, higher;
                std::cout << "Lower value of range: ";
                std::cin >> lower;
                std::cout << "Higher value of range: ";
                std::cin >> higher;
                sz.adminDonocheck(lower, higher);
                wait();
                clear();
                return false;
            case camount:
                sz.listDonoHighest();
                wait();
                clear();
                return false;
            case crating:
                sz.listDonoRating();
                wait();
                clear();
                return false;
            default:
                std::cout << "Bad command";
                clear();
                return false;
        }
    } else {
        std::cout << "Bad input";
        clear();
        return false;
    }
}

/**
 * Logic to handle receiving a nickname and selecting a user from it then loading their menu
 * @param sz StreamZ platform
 */
void selection(StreamZ &sz) {
    clear();
    std::cout << "User's nickname: ";
    std::string nick;
    User* point;
    fflush(stdin);
    std::getline(std::cin, nick);
    try {
        point = sz.select(nick);
    } catch (UserNotFound& e) {
        std::cout << "Could not find " << nick;
        return;
    } catch (AccountNotActive &e) {
        std::cout << "Found " << nick << " but their account is inactive. Reactivate first!";
        return;
    }
    if(point->type() == "viewer") {
        Viewer* v = dynamic_cast<Viewer *>(point);
        bool end = false;
        while(!end) {
            end = viewerMenu(sz, v);
        }
    } else if (point->type() == "streamer") {
        Streamer* s = dynamic_cast<Streamer *>(point);
        bool end = false;
        while(!end) {
            end = streamerMenu(sz, s);
        }
    } else if (point->type() == "admin") {
        Admin* a = dynamic_cast<Admin *>(point);
        bool end = false;
        while(!end) {
            end = adminMenu(sz, a);
        }
    }
}

/**
 * Handles getting inputs to create a user
 * @param sz StreamZ platform
 * @param type Type of user to create
 */
void create(StreamZ &sz, const std::string& type) {
    clear();
    std::string name, nick;
    Date d;
    std::cout << "User name: ";
    fflush(stdin);
    getline(std::cin, name);
    std::cout << "User nickname: ";
    fflush(stdin);
    getline(std::cin, nick);
    std::cout << "Birthdate YYYY/MM/DD: ";
    try {
        d = dateParser();
    } catch (BadDate &e) {
        std::cout << "Bad date";
        return;
    }
    try {
        if(type == "viewer") {
            sz.addViewer(name, nick, d.year, d.month ,d.day);
        } else if(type == "streamer") {
            sz.addStreamer(name, nick, d.year, d.month, d.day);
        } else if(type == "admin") {
            sz.addAdmin(name, nick, d.year, d.month, d.day);
        }
    } catch(NickIsTaken& e) {
        std::cout << "Nickname " << nick << "  is already in use";
        return;
    } catch(ViewerTooYoung& e) {
        std::cout << "Viewers most be >12 years old. Viewer is: " << e.age();
        return;
    } catch(StreamerTooYoung& e) {
        std::cout << "Streamers must be >15 years old. Streamer is: " << e.age();
        return;
    }
    std::cout << "Created succesfully";
}

/**
 * Handles de/reactivating accounts
 * @param sz StreamZ platform
 * @param reactivate Whether to reactivate or deactivate otherwise
 */
void activation(StreamZ &sz, bool reactivate) {
    clear();
    std::cout << "Nickname of user: ";
    std::string nick;
    fflush(stdin);
    getline(std::cin, nick);
    if(reactivate) {
        try {
            sz.reactivateUser(nick);
        } catch(UserNotFound &e) {
            std::cout << "Couldn't find inactive user with that nickname";
        }
    } else {
        try {
            sz.deactivateUser(nick);
        } catch(UserNotFound &e) {
            std::cout << "Couldn't find active user with that nickname";
        }
    }
}

/**
 * Main menu handling the initial display and commands.
 * @param sz StreamZ platform
 * @return If it returns true, the program closes, otherwise, it loops and restarts
 */
bool menu(StreamZ &sz) {
    std::cout << "Commands:\nLoad from file - load\nSave to file - save\n"
                 "Create streamer - streamer\nCreate viewer - viewer\nCreate admin - admin\n"
                 "Select user - selection\nRemove user - remove\nTemporarily deactivate user - deactivate\n"
                 "Reactivate inactive user - reactivate\nClose - close\n\n";
    std::string command;
    if(std::cin >> command) {
        switch (hashit(command)) {
            case cload:
                load(sz);
                clear();
                return false;
            case csave:
                save(sz);
                clear();
                return false;
            case cstreamer:
                create(sz, "streamer");
                clear();
                return false;
            case cviewer:
                create(sz, "viewer");
                clear();
                return false;
            case cadmin:
                create(sz, "admin");
                clear();
                return false;
            case cselection:
                selection(sz);
                clear();
                return false;
            case cremove: {
                std::string nick;
                clear();
                fflush(stdin);
                std::cout << "Who to remove by nick: ";
                std::getline(std::cin, nick);
                try {
                    sz.deleteUser(nick);
                } catch (UserNotFound &e) {
                    std::cout << "Could not find " << nick;
                    clear();
                    return false;
                }
                std::cout << "Done";
                clear();
                return false;
            }
            case deact:
                activation(sz,false);
                clear();
                return false;
            case react:
                activation(sz,true);
                clear();
                return false;
            case cclose:
                return true;
            default:
                std::cout << "Bad command";
                clear();
                return false;
        }
    } else {
        std::cout << "Bad input";
        clear();
        return false;
    }
}

int main() {
    StreamZ sz;
    bool end = false;
    while(!end) {
        end = menu(sz);
    }
}