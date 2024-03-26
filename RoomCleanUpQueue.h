//
// Created by james on 19/03/24.
//
#pragma once

#ifndef INC_305_A6_ROOMCLEANUPQUEUE_H
#define INC_305_A6_ROOMCLEANUPQUEUE_H

#endif //INC_305_A6_ROOMCLEANUPQUEUE_H

#ifndef INC_305_A6_MAIN_H
#define INC_305_A6_MAIN_H

#endif

#ifndef INC_305_A6_EVENTLIST_H
#define INC_305_A6_EVENTLIST_H

#endif //INC_305_A6_EVENTLIST_H

#include <vector>
#include <queue>
//#include "main.h"
#include "EventList.h"

using namespace std;

class RoomCleanUpQueue{

private:

    queue<Room*> RoomQueue;

    double mu_cleanup = 0;
    double numJanitors = 0;
    double cumulativeWaitingTimePlusWaitingTime = 0;
    int numDepartures = 0;

    struct Janitor{
        bool notBusy;    //true if a janitor is not cleaning a room, false otherwise
    };
    vector<Janitor> Janitors;

    Room* cloneRoom(Room* newRoom);

public:

    RoomCleanUpQueue(double mu_cleanup, double numJanitors);

    ~RoomCleanUpQueue();

    void processArrivalForCleanUp(Event* arrivalEvent, EventList* eventList, double currentTime);

    void processCleanUp(Event* cleanUpEvent, EventList* eventList, double currentTime);

    void processFinishingCleanUp(Event* finishCleanUpEvent, EventList* eventList);

    double returnAvgCleanUpTime();

};