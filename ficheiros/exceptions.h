//
// Created by Duarte on 13/11/2020.
//

#ifndef AEDAPROJ_EXCEPTIONS_H
#define AEDAPROJ_EXCEPTIONS_H

#include <string>

/*! \file exceptions.h
    \brief Contains exceptions used in the project.
*/

/**
 * Exception: Repeated nickname
 */
class NickIsTaken {
private:
    std::string conflict;
public:
    NickIsTaken(std::string con) { conflict = con;};
    std::string getConflict() const { return conflict;};
};
/**
 * Exception: tried to search for someone who doesnt exist
 */
class UserNotFound {
public:
    UserNotFound(){};
};
/**
 * Exception: Tried to do an action that requires a streamer to be streaming, but they weren't.
 */
class StreamerNotStreaming {
public:
    StreamerNotStreaming(){};
};
/**
 * Exception: Tried to do something that conflicted with the active stream, like creating a new one
 */
class StreamerAlreadyStreaming {
public:
    StreamerAlreadyStreaming(){};
};
/**
 * Exception: tried to do an action that requires a viewer to be watching a stream, but they weren't.
 */
class ViewerNotWatching {
public:
    ViewerNotWatching(){};
};
/**
 * Exception: tried to do an action that conflicted with the viewer currently watching.
 */
class ViewerAlreadyWatching {
public:
    ViewerAlreadyWatching(){};
};
/**
 * Exception: Viewer is too young to be created
 */
class ViewerTooYoung {
private:
    int a;
public:
    ViewerTooYoung(int a){ this->a = a;};
    int age() { return a;};
};
/**
 * Exception: Streamer is too young to be created
 */
class StreamerTooYoung {
private:
    int a;
public:
    StreamerTooYoung(int a){ this->a = a;};
    int age() { return a;};
};
/**
 * Exception: tried to join a stream but blocked by age limit
 */
class TooYoungForStream {
private:
    int a;
public:
    TooYoungForStream(int a){this->a = a;};
    int ageLimit() { return a;};
};
/**
 * Exception: tried to join a private stream but blocked by capacity
 */
class PrivateStreamFull {
public:
    PrivateStreamFull(){};
};
/**
 * Exception: tried to join a private stream but blocked by the whitelist
 */
class NotInWhitelist {
public:
    NotInWhitelist(){};
};
/**
 * Exception: we couldnt find any streams for a given action
 */
class NoStreamsFound {
public:
    NoStreamsFound(){};
};
/**
 * Exception: failed on opening or reading a file with data
 */
class BadFile {
public:
    BadFile(){};
};
/**
 * Exception: user gave a data in a format we failed to read
 */
class BadDate {
public:
    BadDate(){};
};
/**
 * Exception: tried to a private specific action on a public stream
 */
class StreamNotPrivate{
public:
    StreamNotPrivate(){};
};

#endif //AEDAPROJ_EXCEPTIONS_H
