//
// Created by james on 11/03/24.
//

#include <vector>
#include <iostream>
#include <queue>
#include "main.cpp"
#include <random>

using namespace std;

class EvaluationQueue{

private:

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

    queue<struct Patient> evaluationQueue;
    vector<Nurse> Nurses;

public:

    EvaluationQueue(double num_nurses, double total_patients){
        this->total_patients = total_patients;
        this->num_nurses = num_nurses;
        for(int i = 0; i < this->num_nurses; i++){
            struct Nurse newNurse;
            newNurse.isBusy = true;
            Nurses.push_back(newNurse);
        }
    }

    void processArrivalIntoQueue(Patient newPatient, EventList eventList, int numPatientsInPriorityQueue){
        if(numPatientsInQueue+numPatientsInPriorityQueue < total_patients){
            evaluationQueue.push(newPatient);
            numPatientsInQueue++;

            for(int i = 0; i < Nurses.size(); i++){
                if(!Nurses[i].isBusy){
                    newPatient.nurseNumber = i;
                    Event newServiceEvent;
                    newServiceEvent.timeOfEvent = newPatient.arrivalTime;
                    newServiceEvent.type = 2;
                    newServiceEvent.patient = newPatient;
                    eventList.push(newServiceEvent);
                    return;
                }
            }
        } else {
            droppedArrivals++;
        }
    }

    void processEvaluation(Event serviceEvent, EventList eventList){
        if(evaluationQueue.empty()) return;
        numPatientsFinishingEvaluation++;
        srand(seed);
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
        eventList.push(departureFromEvaluation);
    }

    void processDeparture(Event departureEvent, EventList eventList){
        Nurses[departureEvent.patient.nurseNumber].isBusy = false;
        numPatientsInQueue--;
        if(!evaluationQueue.empty()){
            Event arrivalToPriorityQueue;
            arrivalToPriorityQueue.timeOfEvent = departureEvent.timeOfEvent;
            arrivalToPriorityQueue.type = 4;
            arrivalToPriorityQueue.patient = departureEvent.patient;
            arrivalToPriorityQueue.patient.arrivalTimeIntoPQ = currentTime;
            eventList.push(arrivalToPriorityQueue);
        }
    }

    double returnAvgWaitTime(){
        return cumulativeEvaluationWaitingTime/numPatientsFinishingEvaluation;
    }

    double returnDroppedArrivals(){
        return droppedArrivals;
    };

    double returnNumPatientsInEQueue(){
        return numPatientsInQueue;
    }
};

