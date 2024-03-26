//
// Created by james on 19/03/24.
//

#pragma once

#ifndef INC_305_A6_PRIORITYQUEUE_H
#define INC_305_A6_PRIORITYQUEUE_H

#endif //INC_305_A6_PRIORITYQUEUE_H

#ifndef INC_305_A6_MAIN_H
#define INC_305_A6_MAIN_H
#endif

#ifndef INC_305_A6_EVENTLIST_H
#define INC_305_A6_EVENTLIST_H

#endif //INC_305_A6_EVENTLIST_H

#include <vector>
#include <iostream>
//#include "main.h"
#include "EventList.h"

class PriorityQueue{
private:

    double num_rooms = 0;
    double cumulativeWaitingTime = 0;
    double cumulativeResponseTime = 0;
    double numPatientsInPQ = 0;
    double numDepartures = 0;
    double numPatientsFinishingTreatment = 0;
    vector<Room*> Rooms;
    vector<Patient*> priorityList;

    void bubbleDown(int index);

    void bubbleUp(int index);

    void pushIntoPriorityList(Patient* newPatient);

    void popFromPriorityList();

    Patient* peek();

    Patient* clonePatient(Patient* newPatient);
    Room* cloneRoom(Room* newRoom);

public:

    PriorityQueue(double num_rooms);

    ~PriorityQueue();

    void dumpPQ(){
        for(int i = 0; i < (int)priorityList.size(); i++){
            cout << "{" << priorityList[i]->arrivalTimeIntoPQ << ", " << priorityList[i]->priorityValue << "}, ";
        }
        cout << endl;
    }

    void processArrivalIntoPriorityQueue(Event* newArrival, EventList* eventList);

    void processTreatmentEvent(Event* treatmentEvent, EventList* eventList, double currentTime);

    void processDepartureFromPQ(Event* departureEvent, EventList* eventList, double currentTime);

    void processFinishingCleanUp(Event* roomIsReadyEvent, EventList* eventList);

    double returnNumPatientsInPQ();

    double returnAvgWaitTime();

    double returnAvgResponseTime();

    int returnNumDepartures();

};