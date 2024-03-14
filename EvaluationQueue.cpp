//
// Created by james on 11/03/24.
//

#include <vector>
#include <iostream>
#include <queue>
#include "main.cpp"
#include <random>

using namespace std;

class Hospital{

private:

    double total_patients = 0;
    double numPatientsInQueue = 0;
    double num_nurses = 0;
    double numDepartures;
    //Statistics
    double cumulativeEvaluationWaitingTime = 0;

    struct Nurse{
        bool isBusy;     //false for busy, true otherwise
    };

    queue<struct Patient> evaluationQueue;
    vector<Nurse> Nurses;

public:

    Hospital(double mu_eval, double num_nurses, double total_patients){
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
                    Event newServiceEvent;
                    newServiceEvent.timeOfEvent = newPatient.arrivalTime;
                    newServiceEvent.type = 2;
                    newServiceEvent.nodeEvaluationTime = -99999;
                    newServiceEvent.nodeEvaluationTime = -99999;
                    newServiceEvent.nurseNumber = i;
                    eventList.push(newServiceEvent);
                    return;
                }
            }
        }
    }

    void processEvaluation(Event serviceEvent, EventList eventList){

        if(evaluationQueue.empty()) return;

        srand(seed);
        Patient nextPatient = evaluationQueue.front();
        evaluationQueue.pop();

        Nurses[serviceEvent.nurseNumber].isBusy = true;

        if(currentTime%60 > 6){
            double timeLastServiceEvent = serviceEvent.timeOfEvent;
            cumulativeEvaluationWaitingTime += (timeLastServiceEvent-nextPatient.arrivalTime);
        }
        double priorityValue = (double)rand()/RAND_MAX;
        Event departureFromEvaluation;
        departureFromEvaluation.timeOfEvent = serviceEvent.timeOfEvent+nextPatient.evaluationTime;
        departureFromEvaluation.type = 3;
        departureFromEvaluation.priorityValue = priorityValue;
        departureFromEvaluation.nodeEvaluationTime = -99999;
        departureFromEvaluation.nodeTreatmentTime = -99999;
        departureFromEvaluation.nurseNumber = serviceEvent.nurseNumber;
        eventList.push(departureFromEvaluation);
    }

    void processDeparture(Event departureEvent, EventList eventList){

        Nurses[departureEvent.nurseNumber].isBusy = false;
        numPatientsInQueue--;
        numDepartures++;
        if(!evaluationQueue.empty()){
            Event arrivalToPriorityQueue;
            arrivalToPriorityQueue.timeOfEvent = departureEvent.timeOfEvent;
            arrivalToPriorityQueue.type = 4;
            arrivalToPriorityQueue.priorityValue = departureEvent.priorityValue;
            arrivalToPriorityQueue.nodeTreatmentTime = -99999;
            arrivalToPriorityQueue.roomNumber = -99999;
            arrivalToPriorityQueue.nodeEvaluationTime = -99999;
            arrivalToPriorityQueue.nurseNumber = -99999;
            eventList.push(arrivalToPriorityQueue);
        }

    }

    double returnStatistics(){
        return cumulativeEvaluationWaitingTime/numDepartures;
    }

};

