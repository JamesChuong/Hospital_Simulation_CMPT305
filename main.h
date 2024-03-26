//
// Created by james on 19/03/24.
//

#pragma once

#ifndef INC_305_A6_MAIN_H
#define INC_305_A6_MAIN_H

#endif //INC_305_A6_MAIN_H

#include <iostream>
#include <math.h>

//#include "EventList.h"
//#include "EvaluationQueue.h"
//#include "PriorityQueue.h"
//#include "RoomCleanUpQueue.h"

struct Patient{
    double arrivalTime;
    double arrivalTimeIntoPQ;   //Time the patient arrived in the priority queue
    double evaluationTime;
    double treatmentTime;
    double priorityValue;
    double nurseNumber;
    double roomNumber;
};

struct Room{
    bool isReady;       //true if it is ready for patients, false otherwise
    int janitorNumber;  //The janitor assigned to clean it up
    int roomNumber;     //The number it has been assigned to
    double arrivalTimeForCleanup;   //The time it entered cleanup
};

struct Event{
    double timeOfEvent;
    int type;
    Patient* patient;
    Room* room;
};

Patient* clonePatient(Patient* newPatient);

int verifyCommandLineArguments(int argc, char* argv[]);

void processNewArrival(Patient* newPatient, Event* arrivalEvent);

void printStatistics(int numDepartures, double avgNumberPatients, double EQAvgWaitingTime
        , double PQAvgWaitingTime, double avgResponseTime, double avgCleanupTime ,int droppedArrivals);

void simulation();