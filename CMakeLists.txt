cmake_minimum_required(VERSION 3.27)
project(305_a6)

set(CMAKE_CXX_STANDARD 14)

add_executable(305_a6
        EventList.cpp
        main.cpp
        EvaluationQueue.cpp
        PriorityQueue.cpp
        RoomCleanUpQueue.cpp
        EvaluationQueue.h
        PriorityQueue.h
        RoomCleanUpQueue.h)
