//
// Created by james on 10/03/24.
//

#include <iostream>
#include <math.h>
#include "main.h"
#include "EventList.h"
#include "EvaluationQueue.h"
#include "PriorityQueue.h"
#include "RoomCleanUpQueue.h"

using namespace std;

//Command line arguments
double lambda = 0;
double mu_eval = 0;
double mu_treatment = 0;
double mu_cleanup = 0;
double total_patients = 0;
double num_rooms = 0;
double num_nurses = 0;
double num_janitors = 0;
double seed = 0;

//Global variables
double currentTime = 0;
double arrivalTimeOfPrevPatient = 0;
EventList* eventList = new EventList();

int verifyCommandLineArguments(int argc, char* argv[]){
    if(argc < 10) {
        cout << "ERROR: Not enough arguments" << endl;
        return 0;
    }
    lambda = atof(argv[1]);
    mu_eval = atof(argv[2]);
    mu_treatment = atof(argv[3]);
    mu_cleanup = atof(argv[4]);
    total_patients = atoi(argv[5]);
    num_rooms = atoi(argv[6]);
    num_nurses = atoi(argv[7]);
    num_janitors = atoi(argv[8]);
    seed = atof(argv[9]);

    if(lambda <= 0){
        cout << "ERROR: lambda must be non-negative" << endl;
        return 0;
    }
    if(mu_eval <= 0){
        cout << "ERROR: mu_eval must be non-negative" << endl;
        return 0;
    }
    if(mu_treatment <= 0){
        cout << "ERROR: mu_treatment must be non-negative" << endl;
        return 0;
    }
    if(mu_cleanup <= 0){
        cout << "ERROR: mu_cleanup must be non-negative" << endl;
        return 0;
    }
    if(total_patients <= 0){
        cout << "ERROR: total_patients must be non-negative" << endl;
        return 0;
    }
    if(num_rooms <= 0){
        cout << "ERROR: num_rooms must be non-negative" << endl;
        return 0;
    }
    if(num_nurses <= 0){
        cout << "ERROR: num_nurses must be non-negative" << endl;
        return 0;
    }
    if(num_janitors <= 0){
        cout << "ERROR: num_janitors must be non-negative" << endl;
        return 0;
    }
    if(seed <= 0){
        cout << "ERROR: seed must be non-negative" << endl;
        return 0;
    }
    return 1;
}

//Processes arrival of a patient into the hospital (before entering the "E" queue) and schedules the same event
//for the next patient
Patient processNewArrival(Patient newPatient, Event arrivalEvent){

    //Schedule arrival into the hospital
    Event arrivalIntoEQueue;
    arrivalIntoEQueue.timeOfEvent = newPatient.arrivalTime;
    arrivalIntoEQueue.type = 1;
    arrivalIntoEQueue.patient = newPatient;
    eventList->push(arrivalIntoEQueue);

    //Schedule the arrival of the next patient
    Patient nextPatient;
    nextPatient.arrivalTime = arrivalTimeOfPrevPatient + (-(1 / lambda) * log(rand() / (RAND_MAX + 1.0)));
    nextPatient.treatmentTime = (-(1/mu_treatment)*log(rand()/(RAND_MAX+1.0)));
    nextPatient.evaluationTime = (-(1/mu_eval)*log(rand()/(RAND_MAX+1.0)));
    arrivalTimeOfPrevPatient = nextPatient.arrivalTime;

    Event nextPatientArrival;
    nextPatientArrival.timeOfEvent = nextPatient.arrivalTime;
    nextPatientArrival.type = 0;
    nextPatientArrival.patient = nextPatient;
    eventList->push(nextPatientArrival);

    return nextPatient;
}

void printStatistics(int numDepartures, double avgNumberPatients, double EQAvgWaitingTime
                     , double PQAvgWaitingTime, double avgResponseTime, double avgCleanupTime ,int droppedArrivals){
    printf("Departures = %d\nMean_num_patients = %lf\n"
           "Mean_wait_E_queue = %lf\nMean_wait_P_queue = %lf\nMean_response_time = %lf\nMean_cleanup_time = %lf\n"
           "Dropped_arrivals = %d\n", numDepartures, avgNumberPatients, EQAvgWaitingTime, PQAvgWaitingTime, avgResponseTime, avgCleanupTime, droppedArrivals);
}

void simulation(){

    double avgNumPatients;
    double cumulativeArea = 0;


    //Initialize the first patient
    Patient firstPatient;
    firstPatient.arrivalTime = arrivalTimeOfPrevPatient + (-(1 / lambda) * log(rand() / (RAND_MAX + 1.0)));
    firstPatient.evaluationTime = (-(1 / mu_eval) * log(rand() / (RAND_MAX + 1.0)));
    firstPatient.treatmentTime = (-(1 / mu_treatment) * log(rand() / (RAND_MAX + 1.0)));
    arrivalTimeOfPrevPatient = firstPatient.arrivalTime;

    //Initialize the first event and push it into the event list
    Event firstPatientArriving;
    firstPatientArriving.timeOfEvent = firstPatient.arrivalTime;
    firstPatientArriving.type = 0;
    firstPatientArriving.patient = firstPatient;
    eventList->push(firstPatientArriving);

    //Initialize subsystems
    EvaluationQueue EQueue = EvaluationQueue(num_nurses, total_patients);
    PriorityQueue PQueue = PriorityQueue(num_rooms);
    RoomCleanUpQueue RoomQueue = RoomCleanUpQueue(mu_cleanup, num_janitors);

    Patient currentPatient = firstPatient;
    double timeLastEvent = 0;
    while(currentTime/60 < 30){

        if(eventList->isEmpty()){
            cout << "Event list is empty!" << endl;
            return;
        }

        Event earlistEvent = eventList->peek();
        eventList->pop();
        currentTime = earlistEvent.timeOfEvent;
        cout << currentTime << endl;
        cumulativeArea += (currentTime-timeLastEvent)*(EQueue.returnNumPatientsInEQueue()+PQueue.returnNumPatientsInPQ());
        timeLastEvent = currentTime;
        if(earlistEvent.type == 0){
            currentPatient = processNewArrival(currentPatient, earlistEvent);
        } else if(earlistEvent.type == 1){
            EQueue.processArrivalIntoQueue(earlistEvent.patient, eventList
                                           , PQueue.returnNumPatientsInPQ());
        } else if(earlistEvent.type == 2){
            EQueue.processEvaluation(earlistEvent, eventList, currentTime);
        } else if(earlistEvent.type == 3){
            EQueue.processDeparture(earlistEvent, eventList, currentTime);
        } else if(earlistEvent.type == 4){
            PQueue.processArrivalIntoPriorityQueue(earlistEvent.patient, eventList);
        } else if(earlistEvent.type == 5){
            PQueue.processTreatmentEvent(earlistEvent, eventList, currentTime);
        } else if(earlistEvent.type == 6){
            PQueue.processDepartureFromPQ(earlistEvent, eventList, currentTime);
        } else if(earlistEvent.type == 7){
            RoomQueue.processArrivalForCleanUp(earlistEvent, eventList, currentTime);
        } else if(earlistEvent.type == 8){
            RoomQueue.processCleanUp(earlistEvent, eventList);
        } else if(earlistEvent.type == 9){
            RoomQueue.processFinishingCleanUp(earlistEvent, eventList);
        } else if(earlistEvent.type == 10){
            PQueue.processFinishingCleanUp(earlistEvent, eventList);
        }

        avgNumPatients = cumulativeArea/currentTime;
        if(currentTime/60 == 5){
            printStatistics(PQueue.returnNumDepartures(), avgNumPatients
                    , EQueue.returnAvgWaitTime(), PQueue.returnAvgWaitTime()
                    , PQueue.returnAvgResponseTime(),
                            RoomQueue.returnAvgCleanUpTime(),
                            EQueue.returnDroppedArrivals());
        }
    }
    avgNumPatients = cumulativeArea/currentTime;
    printStatistics(PQueue.returnNumDepartures(), avgNumPatients, EQueue.returnAvgWaitTime(), PQueue.returnAvgWaitTime(), PQueue.returnAvgResponseTime(),RoomQueue.returnAvgCleanUpTime(),EQueue.returnDroppedArrivals());
    delete eventList;
}

int main(int argc, char* argv[]){

    int inputsAllValid = verifyCommandLineArguments(argc, argv);
    if(inputsAllValid){
        srand(seed);
        simulation();
    }

}

