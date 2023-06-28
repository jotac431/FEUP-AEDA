cmake_minimum_required(VERSION 3.17)
project(aedaproj)

set(CMAKE_CXX_STANDARD 14)

add_executable(aedaproj main.cpp streamz.cpp streamz.h utils.cpp utils.h users.cpp users.h streams.cpp streams.h date.cpp date.h exceptions.h userhash.cpp userhash.h)