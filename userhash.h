//
// Created by Duarte on 12/12/2020.
//

#include "users.h"

#ifndef AEDAPROJ_USERHASH_H
#define AEDAPROJ_USERHASH_H

enum state {
    OCCUPIED,
    DELETED,
    EMPTY
};

template<class U> class UserHash {
private:
    struct HashEntry {
        U* element;
        state info;
        HashEntry(U* e = nullptr, state i = EMPTY):
                element(e), info(i) {}
    };
    std::vector<HashEntry> table;
    std::vector<int> occupied;
    void rehash();
    bool isPrime(int n) const;
    int nextPrime(int n) const;
    int hash(std::string nick) const;
    int hash(int pos) const;
    int findEntry(std::string nick) const;
public:
    explicit UserHash(int size = 101);
    U * find(std::string nickname) const;
    void insert(U* user);
    void delet(std::string nickname);
    std::vector<U*> getList() const;
    bool empty() const;
    std::vector<U*> getActive() const;
};

template class UserHash<Viewer>;
template class UserHash<Streamer>;
template class UserHash<Admin>;

#endif //AEDAPROJ_USERHASH_H
