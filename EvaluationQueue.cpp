//
// Created by james on 11/03/24.
//

#include <vector>
#include <iostream>
#include <queue>
#include "main.h"
#include "EvaluationQueue.h"
#include "EventList.h"

EvaluationQueue::EvaluationQueue(double num_nurses, double total_patients, double mu_treatment){
    this->total_patients = total_patients;
    this->num_nurses = num_nurses;
    this->mu_treatment = mu_treatment;
    for(int i = 0; i < this->num_nurses; i++){
        Nurse newNurse;
        newNurse.isBusy = false;
        Nurses.push_back(newNurse);
    }
}

EvaluationQueue::~EvaluationQueue(){
    while(!evaluationQueue.empty()){
        Patient* nextPatient = evaluationQueue.front();
        evaluationQueue.pop();
        if(nextPatient != NULL){
            delete nextPatient;
        }
    }
}

Patient* EvaluationQueue::clonePatient(Patient* newPatient){
    if(newPatient == NULL) return NULL;
    Patient* clonedPatient = new Patient;
    clonedPatient->nurseNumber = newPatient->nurseNumber;
    clonedPatient->priorityValue = newPatient->priorityValue;
    clonedPatient->arrivalTimeIntoPQ = newPatient->arrivalTimeIntoPQ;
    clonedPatient->roomNumber = newPatient->roomNumber;
    clonedPatient->arrivalTime = newPatient->arrivalTime;
    clonedPatient->treatmentTime = newPatient-> treatmentTime;
    clonedPatient->evaluationTime = newPatient->evaluationTime;
    return clonedPatient;
}

void EvaluationQueue::processArrivalIntoQueue(Patient* newPatient, EventList* eventList, int numPatientsInPriorityQueue){
    if(newPatient == NULL) return;
    if(numPatientsInQueue+numPatientsInPriorityQueue < total_patients){
        //cout << "An arrival" << endl;
        //cout << newPatient->arrivalTime << " " << newPatient->evaluationTime << endl;
        numPatientsInQueue++;
        Patient* arrivingPatient = clonePatient(newPatient);
        for(int i = 0; i < (int)Nurses.size(); i++){
            if(!Nurses[i].isBusy){
                arrivingPatient->nurseNumber = i;
                Nurses[i].isBusy = true;
                Event* newServiceEvent = new Event;
                newServiceEvent->timeOfEvent = arrivingPatient->arrivalTime;
                newServiceEvent->type = 2;
                newServiceEvent->patient = clonePatient(arrivingPatient);
                newServiceEvent->room = NULL;
                eventList->push(newServiceEvent);
                evaluationQueue.push(arrivingPatient);
                return;
            }
        }
        cout << "No nurses available" << endl;
        evaluationQueue.push(arrivingPatient);

    } else {
        //cout << "Dropping Arrivial" << endl;
        droppedArrivals++;
    }
}

void EvaluationQueue::processEvaluation(Event* serviceEvent, EventList* eventList, double currentTime){
    if(evaluationQueue.empty()) return;
    numPatientsFinishingEvaluation++;
    Patient* nextPatient = evaluationQueue.front();
    evaluationQueue.pop();
    //Nurses[serviceEvent->patient->nurseNumber].isBusy = true;

    if(currentTime / 60 > 6){
        double timeLastServiceEvent = serviceEvent->timeOfEvent;
        //cout << "Evaluating" << endl;
        //cout << timeLastServiceEvent << ", " << nextPatient->arrivalTime << ", " << serviceEvent->timeOfEvent + nextPatient->evaluationTime << endl;
        cumulativeEvaluationWaitingTime += (timeLastServiceEvent-nextPatient->arrivalTime);
    }

    double priorityValue = (double)rand()/RAND_MAX;
    if(priorityValue > 0.5){
        nextPatient->treatmentTime = 1/mu_treatment;
    } else {
        nextPatient->treatmentTime = 1/(2*mu_treatment);
    }
    Event* departureFromEvaluation = new Event;
    nextPatient->nurseNumber = serviceEvent->patient->nurseNumber;
    nextPatient->priorityValue = priorityValue;
    departureFromEvaluation->timeOfEvent = serviceEvent->timeOfEvent+nextPatient->evaluationTime;
    departureFromEvaluation->type = 3;
    departureFromEvaluation->patient = nextPatient;
    departureFromEvaluation->room = NULL;
    //cout << "This evaluation will finish at " << departureFromEvaluation->timeOfEvent << endl;
    eventList->push(departureFromEvaluation);
}

void EvaluationQueue::processDeparture(Event* departureEvent, EventList* eventList, double currentTime){
    if(departureEvent == NULL) return;
    Nurses[departureEvent->patient->nurseNumber].isBusy = false;
    numPatientsInQueue--;
    if(!evaluationQueue.empty()){
        Event* nextEvaluationEvent = new Event;
        nextEvaluationEvent->timeOfEvent = departureEvent->timeOfEvent;
        nextEvaluationEvent->type = 2;
        nextEvaluationEvent->patient = clonePatient(departureEvent->patient);
        nextEvaluationEvent->room = NULL;
        eventList->push(nextEvaluationEvent);
    }

    Event* arrivalToPriorityQueue = new Event;
    arrivalToPriorityQueue->timeOfEvent = departureEvent->timeOfEvent;
    arrivalToPriorityQueue->type = 4;
    arrivalToPriorityQueue->patient = clonePatient(departureEvent->patient);
    arrivalToPriorityQueue->patient->arrivalTimeIntoPQ = departureEvent->timeOfEvent;
    arrivalToPriorityQueue->room = NULL;
    eventList->push(arrivalToPriorityQueue);

}

double EvaluationQueue::returnAvgWaitTime(){
    return cumulativeEvaluationWaitingTime/numPatientsFinishingEvaluation;
}

double EvaluationQueue::returnDroppedArrivals(){
    return droppedArrivals;
};

double EvaluationQueue::returnNumPatientsInEQueue(){
    return numPatientsInQueue;
}