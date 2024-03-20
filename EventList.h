//
// Created by james on 19/03/24.
//

#pragma once

#ifndef INC_305_A6_EVENTLIST_H
#define INC_305_A6_EVENTLIST_H

#endif //INC_305_A6_EVENTLIST_H

#ifndef INC_305_A6_MAIN_H
#define INC_305_A6_MAIN_H
#endif

#include <vector>
#include <iostream>
#include "main.h"

using namespace std;

class EventList {
private:

    //The implementation of the event list is a min heap, that way removals are done in O(log(n)) time
    vector<Event> eventList;

    void bubbleDown(int index);

    void bubbleUp(int index);

public:

    void push(Event newEvent);

    void pop();

     Event peek();

    bool isEmpty();
};