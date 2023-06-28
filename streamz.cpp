//
// Created by Duarte on 19/10/2020.
//

#include "streamz.h"

/*! \file streamz.cpp
    \brief Streamz body.
*/

#define PURPLE 5
#define RED 4
#define GREEN 2
#define WHITE 7
#define YELLOW 14

bool StreamZ::uniqueNickname(std::string nick) const {
    try {
        select(nick);
    } catch (UserNotFound &e) {
        return true;
    }
    return false;
}
Viewer* StreamZ::addViewer(std::string name, std::string nick, int y, int m, int d) {
    if(!uniqueNickname(nick)) { //make sure the nickname is new
        throw NickIsTaken(nick);
    }
    Viewer* v = new Viewer(name, nick, y, m, d); //create and add it
    viewerList.insert(v);
    return v;
}
Streamer* StreamZ::addStreamer(std::string name, std::string nick, int y, int m, int d) {
    if(!uniqueNickname(nick)) { //see viewer
        throw NickIsTaken(nick);
    }
    Streamer* s = new Streamer(name, nick, y, m, d);
    streamerList.insert(s);
    return s;
}
Admin* StreamZ::addAdmin(std::string name, std::string nick, int y, int m, int d) {
    if(!uniqueNickname(nick)) { //see viewer
        throw NickIsTaken(nick);
    }
    Admin* a = new Admin(name, nick, y, m, d);
    adminList.insert(a);
    return a;
}
std::vector<Stream*> StreamZ::activeStreams() const {
    std::vector<Stream*> ret;
    for(auto streamer: streamerList.getActive()) { //loop trough every streamer
        if(streamer->streaming()) { //add their stream to the vector, if theyre streaming
            ret.push_back(streamer->getStream());
        }
    }
    return ret;
}
std::vector<Stream*> StreamZ::inactiveStreams() const {
    std::vector<Stream*> ret;
    std::vector<Stream*> add;
    for(auto streamer: streamerList.getActive()) { //loop trough every streamer
        if(!streamer->inactive()) {
            add = streamer->getStreamHistory(); //get their history
        }
        if(!add.empty()) { //if it isnt empty, add it to ret
            ret.insert(ret.end(), add.begin(), add.end());
        }
    }
    return ret;
}
std::vector<Stream*> StreamZ::allStreams() const {
    std::vector<Stream*> ret = activeStreams();
    std::vector<Stream*> add = inactiveStreams();
    ret.insert(ret.end(), add.begin(), add.end()); //get both active and inactive and concat
    return ret;
}

void StreamZ::printVector(const std::vector<Stream*> &vec) const {
    if(vec.empty()) { //first we need to know if there are any streams to print, to avoid printing nothing
        throw NoStreamsFound();
    }
    for (auto stream: vec) { //okay print every stream
        std::cout << "--------------------------------------\n"; //put some separation between them
        stream->info();
    }
    std::cout << "--------------------------------------\n";
}

void StreamZ::filterStreams(std::vector<Stream*> &vec, std::string lang) const {
    std::vector<Stream*>::iterator it = vec.begin(); //iterate trough the vector and
    while(it != vec.end()) {
        Stream* pt = *it;
        if(pt->getLanguage() != firstUpper(lang)) { //if the languages dont match erase it
            vec.erase(it);
        } else {
            it++;
        }
    }
}
void StreamZ::filterStreams(std::vector<Stream*> &vec, int age) const {
    std::vector<Stream*>::iterator it = vec.begin(); //iterate trough the vector
    while(it != vec.end()) {
        Stream* pt = *it;
        if(pt->ageLimit() > age) { //if the age limit is bigger than our given age, erase it
            vec.erase(it);
        } else {
            it++;
        }
    }
}
void StreamZ::sortLikes(std::vector<Stream*> &vec) {
    for (unsigned int p = 1; p < vec.size(); p++) //simple sort
    {
        Stream* tmp = vec[p];
        int j;
        for (j = p; j > 0 && tmp->getLikes() >= vec[j-1]->getLikes(); j--)
            vec[j] = vec[j-1];
        vec[j] = tmp;
    }
}
void StreamZ::sortViews(std::vector<Stream*> &vec) {
    for (unsigned int p = 1; p < vec.size(); p++)
    {
        Stream* tmp = vec[p];
        int j;
        for (j = p; j > 0 && tmp->getViewerCount() >= vec[j-1]->getViewerCount(); j--)
            vec[j] = vec[j-1];
        vec[j] = tmp;
    }
}

void StreamZ::saveData(std::string filepath) const {
    std::ofstream file;
    file.open(filepath); //open the file
    file << "limit\n" << merchLimit << std::endl;
    for(auto streamer: streamerList.getList()) { //save the strings from every user
        file << streamer->save();
    }
    for(auto viewer: viewerList.getList()) {
        file << viewer->save();
    }
    for(auto admin: adminList.getList()) {
        file << admin->save();
    }
    for(BSTItrPre<Donate> bItr(donations); !bItr.isAtEnd(); bItr.advance()){
        file << bItr.retrieve().save();
    }
    file.close(); //then close
}

void StreamZ::loadData(std::string filepath) {
    std::ifstream file;
    file.open(filepath); //open
    if(file.fail()){
        throw BadFile();
    }
    Streamer* lastStreamer = nullptr;
    Viewer* lastViewer = nullptr;
    Admin* lastAdmin = nullptr;
    PrivateStream* lastPrivate = nullptr; //streamers, viewers and privatestreams have extra possible actions, so we
    std::string command, clear;         //keep them in mind
    while(!file.eof()) {
        std::getline(file, command);
        if(command.empty()) { //got nothing, end of file
            break;
        }
        if(command == "limit") {
            int lim;
            file >> lim;
            this->merchLimit = lim;
            std::getline(file, clear);
        }
        else if(command == "streamer" || command == "viewer" || command == "admin") { //command is to construct a user
            std::string name, nickname, statestring;
            int y, m, d;
            std::getline(file, name);
            std::getline(file, nickname);
            std::getline(file, statestring);
            status st;
            if(statestring == "active") {
                st = ACTIVE;
            } else if (statestring == "inactive") {
                st = INACTIVE;
            } else if(statestring == "reactivated") {
                st = REACTIVATED;
            }
            file >> y >> m >> d;
            std::getline(file, clear); //get all the info
            if(command == "streamer") { //and construct the right one
                lastStreamer = addStreamer(name, nickname, y, m ,d);
                lastStreamer->updateState(st);
            } else if(command == "viewer") {
                lastViewer = addViewer(name, nickname, y, m, d);
                lastViewer->updateState(st);
            } else {
                lastAdmin = addAdmin(name, nickname, y, m ,d);
                lastAdmin->updateState(st);
            }
        } else if (lastStreamer != nullptr && (command == "stream" || command == "privatestream"
        || command == "oldstream" || command == "oldprivatestream")) { //we have something to add to the last streamer
            std::string name, lang;
            int age, y, m, d, l, dl;
            std::getline(file, name);
            std::getline(file, lang);
            file >> age >> y >> m >> d >> l >> dl; //comon data
            if (command == "stream") {
                std::getline(file, clear);
                lastStreamer->setStream(new Stream(name, lang, age, lastStreamer, y, m, d, l, dl));
            } else if (command == "oldstream") { //if its old we need the views too, and push to history
                int views;
                file >> views;
                std::getline(file, clear);
                lastStreamer->addToHistory(new Stream(name, lang, age, lastStreamer, y, m, d, l, dl, views));
            } else { //its private, we need the capacity and to save it to lastPrivate
                int capacity;
                file >> capacity;
                if (command == "oldprivatestream") {
                    int views;
                    file >> views;
                    std::getline(file, clear);
                    lastPrivate = new PrivateStream(name, lang, age, lastStreamer, capacity, y, m, d, l, dl, views);
                    lastStreamer->addToHistory(lastPrivate);
                } else {
                    std::getline(file, clear);
                    lastPrivate = new PrivateStream(name, lang, age, lastStreamer, capacity, y, m, d, l, dl);
                    lastStreamer->setStream(lastPrivate);
                }
            }
        } else if (lastStreamer != nullptr && command == "order") {
            std::string nick;
            int qnt, disp;
            std::getline(file, nick);
            file >> qnt >> disp;
            std::getline(file, clear);
            purchOrder(nick, lastStreamer->getNick(), qnt, disp);
        } else if(lastViewer != nullptr && command == "watching") { //the viewer we added is also watching someone
            std::string nick;
            getline(file, nick);
            viewerJoinStream(lastViewer, nick); //have them join
        } else if(lastPrivate != nullptr && (command == "feedback" || command == "whitelist")) { //private stream has stuff
            std::string msg;
            if (command == "feedback") { //if its feedback
                while (msg != "feedbackend") { //add every line until we get to feedbackend
                    getline(file, msg);
                    if (msg != "feedbackend") {
                        lastPrivate->privateFeedback(msg);
                    }
                }
            } else { //must be whitelist
                while (msg != "whitelistend") {
                    getline(file, msg); //add every nick until we get to the end
                    if (msg != "whitelistend") {
                        lastPrivate->addToList(msg);
                    }
                }
            }
        } else if(command == "donation") {
            std::string nick;
            float am;
            int eval;
            std::getline(file, nick);
            file >> am >> eval;
            std::getline(file, clear);
            addDonation(nick, am, eval);
        } else { //something went wrong
            throw BadFile();
        }
    }
}

User* StreamZ::select(std::string nick) const{ //got a nick from the console, get the pointer it corresponds to
    nick = lowercase(nick);
    User* ret;
    try {
        ret = streamerList.find(nick);
    } catch (UserNotFound &e) {
        try {
            ret = viewerList.find(nick);
        } catch (UserNotFound &e) {
            try {
                ret = adminList.find(nick);
            } catch (UserNotFound &e) {
                throw UserNotFound();
            }
        }
    }
    if(ret->inactive()) throw AccountNotActive();
    return ret;
}

void StreamZ::listActiveStreams() const {
    printVector(activeStreams());
}
void StreamZ::search(std::string language) const {
    std::vector<Stream*> toFilter = allStreams();
    filterStreams(toFilter, language);
    printVector(toFilter);
}
void StreamZ::search(int age) const {
    std::vector<Stream*> toFilter = allStreams();
    filterStreams(toFilter, age);
    printVector(toFilter);
}
void StreamZ::search(std::string language, int age) const {
    std::vector<Stream*> toFilter = allStreams();
    filterStreams(toFilter, language);
    filterStreams(toFilter, age);
    printVector(toFilter);
}
void StreamZ::listUsers(bool active) const {
    Color(PURPLE);std::cout<<"Streamers:\n";Color(WHITE); //first we list the streamers
    std::vector<Streamer*> st;
    std::vector<Viewer*> vw;
    std::vector<Admin*> ad;
    if(active) {
        st = streamerList.getActive();
        vw = viewerList.getActive();
        ad = adminList.getActive();
    } else {
        st = streamerList.getList();
        vw = viewerList.getList();
        ad = adminList.getList();
    }
    for(auto streamer: st) {
        std::cout << *streamer;
    }
    Color(PURPLE);std::cout<<"Viewers:\n";Color(WHITE); //then we list the regular viewers
    for(auto viewer: vw) {
        std::cout << *viewer;
    }
    Color(PURPLE);std::cout<<"Admins:\n";Color(WHITE); //finally list admins
    for(auto admin: ad) {
        std::cout << *admin;
    }
}
void StreamZ::listTop() const {
    std::vector<Stream*> top = inactiveStreams(); //here we just wanna rank the ones that are done
    if(top.empty()) {
        throw NoStreamsFound();
    }
    Color(PURPLE);std::cout<<"TOP 10 MOST LIKED STREAMS IN HISTORY\n\n";Color(WHITE);
    sortLikes(top); //sort
    if(top.size() > 10) { //but only print 10 max
        top.resize(10);
    }
    printVector(top);
    top = inactiveStreams();
    Color(PURPLE);std::cout<<"\n\nTOP 10 MOST VIEWED STREAMS IN HISTORY\n\n";Color(WHITE);
    sortViews(top); //sort
    if(top.size() > 10) { //10 max
        top.resize(10);
    }
    printVector(top);
}
void StreamZ::streamQuantity() const {
    std::cout << "In total there have been " << allStreams().size() << " created streams.\n";
}
void StreamZ::averageViews() const {
    std::vector <Stream*> active = activeStreams();
    std::vector <Stream*> inactive = inactiveStreams();
    if(active.empty() && inactive.empty()) { //cant get averages, there are no streams
        throw NoStreamsFound();
    }
    float avg = 0;
    if(!active.empty()) { //if theres active streams, get their average
        for (auto stream: active) {
            avg += stream->getViewerCount();
        }
        avg = avg / active.size();
        std::cout << "The average currently active stream has " << avg << " viewers.\n";
    }
    if(!inactive.empty()) { //if theres streams in history, get their average
        avg = 0;
        for (auto stream: inactive) {
            avg += stream->getViewerCount();
        }
        avg = avg / inactive.size();
        std::cout << "The average stream that has ended had " << avg << " viewers.\n";
    }
}
void StreamZ::privatePublic(Date d1, Date d2) const {
    std::vector<Stream*> streams = allStreams(); //get all
    int pub = 0;
    int priv = 0;
    for(auto stream: streams) {
        if(d1 <= stream->getDate() && stream->getDate() <= d2) { //check if they were created in the range
            if(stream->isPrivate()) { //increment the right counter
                priv++;
            } else {
                pub++;
            }
        }
    }
    std::cout << pub << " public streams were started in the given time period.\n";
    std::cout << priv << " private streams were started in the given time period.\n";
}
void StreamZ::mostCreated() const {
    std::vector<Stream*> streams = allStreams();
    if(streams.empty()) { //if theres no streams, theres no mostCreated
        throw NoStreamsFound();
    }
    std::map<std::string, int> langs;
    int pub = 0;
    int priv = 0;
    for(auto stream: streams) { //count their types
        if(stream->isPrivate()) {
            priv++;
        } else {
            pub++;
        }
        ++langs[stream->getLanguage()]; //create/increase language:occurences map
    }
    std::cout << pub << " public streams have been created, as opposed to " << priv << " private.\n";
    int amount = 0;
    std::string most;
    for(auto& lang : langs) { //find the language with the most occurences
        if(lang.second > amount) {
            amount = lang.second;
            most = lang.first;
        }
    }
    std::cout << "The most common language is " << most << " with " << amount << " streams.\n";
}
void StreamZ::mostViewed() const {
    if(streamerList.getActive().size()) { //theres no mostviewed if theres no streamers period
        throw UserNotFound();
    }
    std::map<std::string, int> stViewers;
    for(auto streamer: streamerList.getActive()) { //go trough every streamer
        int viewers = 0;
        if(streamer->streaming()) { //get both active and inactive viewers
            viewers += streamer->getStream()->getViewerCount();
        }
        for(auto stream: streamer->getStreamHistory()) {
            viewers += stream->getViewerCount();
        }
        stViewers.insert({streamer->getNick(), viewers}); //add to map
    }
    int amount = 0;
    std::string most;
    for(auto& str : stViewers) { //find the one with most viewers
        if(str.second > amount) {
            amount = str.second;
            most = str.first;
        }
    }
    std::cout << "The most viewed streamer is " << most << " with " << amount << " total views.\n";
}
void StreamZ::viewerJoinStream(Viewer *v, std::string streamerNick) {
    if(!v->watching()) { //first check: is the viewer already watching? if not go, otherwise throw
        Streamer *streamer = streamerList.find(streamerNick);
        if(!streamer->inactive()) { //check activity
            if (lowercase(streamer->getNick()) == lowercase(streamerNick)) { //guy we want?
                if(streamer->streaming()) { //stream going on?
                    if(v->getAge() < streamer->getStream()->ageLimit()) { //check if age is good
                        throw TooYoungForStream(streamer->getStream()->ageLimit()); //nope
                    }//ok age is good, were good to go, need private checks now
                    if(streamer->getStream()->isPrivate()) { //is it private
                        PrivateStream* privPointer = dynamic_cast<PrivateStream *>(streamer->getStream());//get pointer
                        if(privPointer->isFull()) {//check for capacity
                            throw PrivateStreamFull();
                        }
                        if(!privPointer->userAllowed(v->getNick())) {//check for whitelist
                            throw NotInWhitelist();
                        }
                    }//all checks done, lets join
                    streamer->getStream()->addViewer(v); //adds the viewer to stream
                    v->watchStream(streamer->getStream()); //and has him point to it
                    return; //were done with joining
                } else { //the streamer isnt streaming
                    throw StreamerNotStreaming();
                }
            }
        } else {
            throw AccountNotActive();
        }
    } else {
        throw ViewerAlreadyWatching();
    }
    return;
}

void StreamZ::deleteUser(std::string nick) {
    nick = lowercase(nick);
    try {
        streamerList.delet(nick);
    } catch (UserNotFound &e) {
        try {
            viewerList.delet(nick);
        } catch (UserNotFound &e) {
            try {
                adminList.delet(nick);
            } catch (UserNotFound &e) {
                throw UserNotFound();
            }
        }
    }
}

void StreamZ::deactivateUser(std::string nick) {
    nick = lowercase(nick);
    try {
        Streamer* p = streamerList.find(nick);
        if(p->inactive()) {
            throw UserNotFound();
        }
        p->updateState(INACTIVE);
        if(p->streaming()) {
            p->endStream();
        }
    } catch (UserNotFound &e) {
        try {
            Viewer* p = viewerList.find(nick);
            if(p->inactive()) {
                throw UserNotFound();
            }
            p->updateState(INACTIVE);
            if(p->watching()) {
                p->leaveStream();
            }
        } catch (UserNotFound &e) {
            try {
                Admin* p = adminList.find(nick);
                if(p->inactive()) {
                    throw UserNotFound();
                }
                p->updateState(INACTIVE);
            } catch (UserNotFound &e) {
                throw UserNotFound();
            }
        }
    }
}

void StreamZ::reactivateUser(std::string nick) {
    nick = lowercase(nick);
    try {
        Streamer* p = streamerList.find(nick);
        if(!p->inactive()) {
            throw UserNotFound();
        }
        p->updateState(REACTIVATED);
    } catch (UserNotFound &e) {
        try {
            Viewer* p = viewerList.find(nick);
            if(!p->inactive()) {
                throw UserNotFound();
            }
            p->updateState(ACTIVE);
        } catch (UserNotFound &e) {
            try {
                Admin* p = adminList.find(nick);
                if(!p->inactive()) {
                    throw UserNotFound();
                }
                p->updateState(ACTIVE);
            } catch (UserNotFound &e) {
                throw UserNotFound();
            }
        }
    }
}

void StreamZ::setMerchLimit(int limit) {
    merchLimit = limit;
    for (int i = 0; i < streamerList.getList().size(); ++i) {
        if (streamerList.getList()[i]->getStockUsed() > limit)
            streamerList.getList()[i]->deletePurch(limit);
    }
}

void StreamZ::purchOrder(std::string v, std::string streamerNick, int qnt, int disp) {
    Streamer * s = streamerList.find(streamerNick);
    if(s->inactive()) {
        throw UserNotFound();
    }
    if (qnt > merchLimit - s->getStockUsed())
        throw StockNotEnough();
    if (disp < 1 || disp > 5)
        throw NotInRange();
    PurchaseOrder p(v, qnt, disp);
    s->pushPurch(p);
}

void StreamZ::deletOrder(std::string v, std::string streamerNick) {
    Streamer * s = streamerList.find(streamerNick);
    if(s->inactive()) {
        throw UserNotFound();
    }
    s->deletePurch(v);
}

void StreamZ::addDonation(std::string nick, float amount, unsigned int eval) {
    Streamer * s = streamerList.find(nick);
    if(s->inactive()) {
        throw UserNotFound();
    }
    nick = s->getNick();
    streamerEvaluation ev;
    if(amount < 0) {
        throw InvalidDonationValue();
    }
    if(eval < 1 || eval > 5) {
        throw InvalidEvaluation();
    }
    Donate d(nick, amount, eval);
    donations.insert(d);
}

void StreamZ::adminDonocheck(float interval1, float interval2) {
    Color(PURPLE);
    std::cout << "Donations in range [" << interval1 << ";" << interval2 << "]\n";
    Color(WHITE);
    bool gotAny = false;
    for (BSTItrIn<Donate> bItr(donations); !bItr.isAtEnd(); bItr.advance()) {
        Donate donation = bItr.retrieve();
        if (donation.getAmmount() >= interval1 && donation.getAmmount() <= interval2) {
            std::cout << donation;
            gotAny = true;
        } else if (donation.getAmmount() < interval1) {
            break;
        }
        if (!gotAny) {
            std::cout << "No donations found.\n";
        }
    }
}

void StreamZ::listDonoHighest() {
    if(donations.isEmpty()) {
        std::cout << "No donations found.\n";
        return;
    }
    Color(PURPLE);
    std::cout << "Donations by value\n";
    Color(WHITE);
    for(BSTItrIn<Donate> bItr(donations); !bItr.isAtEnd(); bItr.advance()){
        Donate donation = bItr.retrieve();
        std::cout << donation;
    }
}

void StreamZ::listDonoRating() {
    if(donations.isEmpty()) {
        std::cout << "No donations found.\n";
        return;
    }
    Color(PURPLE);
    std::cout << "Donations by rating\n";
    Color(WHITE);
    for(int i = 5; i >= 1; i--){
        for (BSTItrIn<Donate> bItr(donations); !bItr.isAtEnd(); bItr.advance()) {
            Donate donation = bItr.retrieve();
            if(donation.getEvaluation() == i) {
                std::cout << donation;
            }
        }
    }
}

void StreamZ::listDonoStreamer(std::string nick) {
    Color(PURPLE);
    std::cout << "Your donations:\n";
    Color(WHITE);
    bool gotAny = false;
    nick = lowercase(nick);
    for (BSTItrIn<Donate> bItr(donations); !bItr.isAtEnd(); bItr.advance()) {
        Donate donation = bItr.retrieve();
        if (lowercase(donation.getStreamer()) == nick) {
            std::cout << donation;
            gotAny = true;
        }
    }
    if (!gotAny) {
        std::cout << "No donations found.\n";
    }
}
