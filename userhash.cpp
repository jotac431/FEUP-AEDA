//
// Created by Duarte on 12/12/2020.
//

#include "userhash.h"

template <class U>
int UserHash<U>::hash(std::string nick) const {
    nick = lowercase(nick);
    int pos;
    for(auto ch: nick) {
        pos += ch;
    }
    pos %= table.size();
    return pos;
}
template <class U>
int UserHash<U>::hash(int pos) const {
    pos = pow(pos, 2);
    pos %= table.size();
    return pos;
}

template <class U>
bool UserHash<U>::isPrime(int n) const {
    // Corner cases
    if (n <= 1)  return false;
    if (n <= 3)  return true;

    // This is checked so that we can skip
    // middle five numbers in below loop
    if (n%2 == 0 || n%3 == 0) return false;

    for (int i=5; i*i<=n; i=i+6)
        if (n%i == 0 || n%(i+2) == 0)
            return false;

    return true;
}

template <class U>
int UserHash<U>::nextPrime(int n) const
{

    // Base case
    if (n <= 1)
        return 2;

    int prime = n;
    bool found = false;

    // Loop continuously until isPrime returns
    // true for a number greater than n
    while (!found) {
        prime++;
        if (isPrime(prime))
            found = true;
    }

    return prime;
}

template <class U>
void UserHash<U>::rehash()
{
    std::vector<HashEntry> oldTable = table;
    table.resize(nextPrime(2 * oldTable.size()));
    for(int j = 0; j < table.size(); j++ )
        table[j].info = EMPTY;
    occupied.clear();
    for(int i = 0; i < oldTable.size(); i++ )
        if(oldTable[i].info == OCCUPIED) {
            insert(oldTable[i].element);
        }
}

template <class U>
UserHash<U>::UserHash(int size) {
    size = nextPrime(size);
    while(size != 0) {
        size--;
        table.push_back(HashEntry());
    }
}

template <class U>
int UserHash<U>::findEntry(std::string nick) const{
    int pos = hash(nick);
    while(true) {
        if(table[pos].info == OCCUPIED) {
            if(lowercase(table[pos].element->getNick()) == lowercase(nick)) {
                return pos;
            } else {
                pos = hash(pos);
                continue;
            }
        } else if(table[pos].info == DELETED) {
            pos = hash(pos);
            continue;
        } else if(table[pos].info == EMPTY) {
            throw UserNotFound();
        }
    }
}

template <class U>
U * UserHash<U>::find(std::string nickname) const {
    int pos = findEntry(nickname);
    return table[pos].element;
}

template <class U>
void UserHash<U>::insert(U* user) {
    int pos = hash(user->getNick());
    while(table[pos].info != EMPTY) {
        pos = hash(pos);
    }
    table[pos] = HashEntry(user, OCCUPIED);
    occupied.push_back(pos);
    if(occupied.size()*2 >= table.size()) {
        rehash();
    }
}

template <class U>
void UserHash<U>::delet(std::string nickname) {
    int pos = findEntry(nickname);
    User* toRemove = table[pos].element;
    delete toRemove;
    table[pos].info = DELETED;
}

template <class U>
std::vector<U*> UserHash<U>::getList() const {
    std::vector<U*> ret;
    for(auto pos: occupied) {
        if(table[pos].info == OCCUPIED) {
            ret.push_back(table[pos].element);
        }
    }
    return ret;
}

template <class U>
bool UserHash<U>::empty() const {
    return occupied.size() == 0;
}

template <class U>
std::vector<U*> UserHash<U>::getActive() const {
    auto ret = getList();
    for(auto it = ret.begin(); it < ret.end(); it++) {
        if((*it)->inactive()) {
            it = ret.erase(it);
        }
    }
    return ret;
}