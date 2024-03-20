//
// Created by james on 11/03/24.
//

#include <vector>
#include <iostream>
#include <queue>
#include "main.h"
#include "EvaluationQueue.h"
#include "EventList.h"

EvaluationQueue::EvaluationQueue(double num_nurses, double total_patients){
    this->total_patients = total_patients;
    this->num_nurses = num_nurses;
    for(int i = 0; i < this->num_nurses; i++){
        struct Nurse newNurse;
        newNurse.isBusy = false;
        Nurses.push_back(newNurse);
    }
}

void EvaluationQueue::processArrivalIntoQueue(Patient newPatient, EventList* eventList, int numPatientsInPriorityQueue){
    if(numPatientsInQueue+numPatientsInPriorityQueue < total_patients){
        numPatientsInQueue++;

        for(int i = 0; i < (int)Nurses.size(); i++){
            if(!Nurses[i].isBusy){

                newPatient.nurseNumber = i;
                Event newServiceEvent;
                newServiceEvent.timeOfEvent = newPatient.arrivalTime;
                newServiceEvent.type = 2;
                newServiceEvent.patient = newPatient;
                eventList->push(newServiceEvent);
                evaluationQueue.push(newPatient);
                return;
            }
        }
        evaluationQueue.push(newPatient);

    } else {
        droppedArrivals++;
    }
}

void EvaluationQueue::processEvaluation(Event serviceEvent, EventList* eventList, double currentTime){
    if(evaluationQueue.empty()) return;
    numPatientsFinishingEvaluation++;
    Patient nextPatient = evaluationQueue.front();
    evaluationQueue.pop();
    Nurses[serviceEvent.patient.nurseNumber].isBusy = true;
    if(currentTime / 60 > 6){
        double timeLastServiceEvent = serviceEvent.timeOfEvent;
        cumulativeEvaluationWaitingTime += (timeLastServiceEvent-nextPatient.arrivalTime);
    }

    double priorityValue = (double)rand()/RAND_MAX;
    Event departureFromEvaluation;
    departureFromEvaluation.timeOfEvent = serviceEvent.timeOfEvent+nextPatient.evaluationTime;
    departureFromEvaluation.type = 3;
    departureFromEvaluation.patient.priorityValue = priorityValue;
    departureFromEvaluation.patient.nurseNumber = serviceEvent.patient.nurseNumber;
    eventList->push(departureFromEvaluation);
}

void EvaluationQueue::processDeparture(Event departureEvent, EventList* eventList, double currentTime){
    Nurses[departureEvent.patient.nurseNumber].isBusy = false;
    numPatientsInQueue--;

    if(!evaluationQueue.empty()){
        Event nextEvaluationEvent;
        nextEvaluationEvent.patient.nurseNumber = departureEvent.patient.nurseNumber;
        nextEvaluationEvent.timeOfEvent = departureEvent.timeOfEvent;
        nextEvaluationEvent.type = 2;
        eventList->push(nextEvaluationEvent);
    }

    Event arrivalToPriorityQueue;
    arrivalToPriorityQueue.timeOfEvent = departureEvent.timeOfEvent;
    arrivalToPriorityQueue.type = 4;
    arrivalToPriorityQueue.patient = departureEvent.patient;
    arrivalToPriorityQueue.patient.arrivalTimeIntoPQ = currentTime;
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