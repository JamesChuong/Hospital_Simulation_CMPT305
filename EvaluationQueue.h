//
// Created by james on 19/03/24.
//
#pragma once

#ifndef INC_305_A6_EVALUATIONQUEUE_H
#define INC_305_A6_EVALUATIONQUEUE_H

#endif //INC_305_A6_EVALUATIONQUEUE_H

#ifndef INC_305_A6_MAIN_H
#define INC_305_A6_MAIN_H
#endif

#ifndef INC_305_A6_EVENTLIST_H
#define INC_305_A6_EVENTLIST_H
#endif //INC_305_A6_EVENTLIST_H

#include <vector>
#include <iostream>
#include <queue>
//#include "main.h"
#include "EventList.h"

using namespace std;

class EvaluationQueue{

private:

    double mu_treatment = 0;
    double total_patients = 0;
    double numPatientsInQueue = 0;
    double num_nurses = 0;
    double numPatientsFinishingEvaluation = 0;
    //Statistics
    double cumulativeEvaluationWaitingTime = 0;
    double droppedArrivals = 0;
    struct Nurse{
        bool isBusy;     //false for busy, true otherwise
    };

    queue<struct Patient*> evaluationQueue;
    vector<Nurse> Nurses;

    Patient* clonePatient(Patient* newPatient);

public:

    EvaluationQueue(double num_nurses, double total_patients, double mu_treatment);

    ~EvaluationQueue();

    void processArrivalIntoQueue(Patient* newPatient, EventList* eventList, int numPatientsInPriorityQueue);

    void processEvaluation(Event* serviceEvent, EventList* eventList, double currentTime);

    void processDeparture(Event* departureEvent, EventList* eventList, double currentTime);

    double returnAvgWaitTime();

    double returnDroppedArrivals();

    double returnNumPatientsInEQueue();

};

