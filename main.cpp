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

Patient* clonePatient(Patient* newPatient){
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

//Processes arrival of a patient into the hospital (before entering the "E" queue) and schedules the same event
//for the next patient
void processNewArrival(Patient* newPatient, Event* arrivalEvent){
    //Schedule arrival into the hospital
    Event* arrivalIntoEQueue = new Event;
    arrivalIntoEQueue->timeOfEvent = newPatient->arrivalTime;
    arrivalIntoEQueue->type = 1;
    arrivalIntoEQueue->patient = clonePatient(newPatient);
    arrivalIntoEQueue->room = NULL;
    eventList->push(arrivalIntoEQueue);

    //Schedule the arrival of the next patient
    Patient* nextPatient = new Patient;
    nextPatient->arrivalTime = arrivalTimeOfPrevPatient + (-(1 / lambda) * log(rand() / (RAND_MAX + 1.0)));
    nextPatient->treatmentTime = 0;
    nextPatient->evaluationTime = (-(1/mu_eval)*log(rand()/(RAND_MAX+1.0)));
    arrivalTimeOfPrevPatient = nextPatient->arrivalTime;
    nextPatient->nurseNumber = -99999;
    nextPatient->roomNumber = -99999;
    nextPatient->arrivalTimeIntoPQ = -99999;
    nextPatient->priorityValue = -99999;

    Event* nextPatientArrival = new Event;
    nextPatientArrival->timeOfEvent = nextPatient->arrivalTime;
    nextPatientArrival->type = 0;
    nextPatientArrival->patient = nextPatient;
    nextPatientArrival->room = NULL;
    eventList->push(nextPatientArrival);

    //return nextPatient;
}

void printStatistics(int numDepartures, double avgNumberPatients, double EQAvgWaitingTime
                     , double PQAvgWaitingTime, double avgResponseTime, double avgCleanupTime ,int droppedArrivals){
    printf("Departures = %d\nMean_num_patients = %lf\n"
           "Mean_wait_E_queue = %lf\nMean_wait_P_queue = %lf\nMean_response_time = %lf\nMean_cleanup_time = %lf\n"
           "Dropped_arrivals = %d\n", numDepartures, avgNumberPatients, EQAvgWaitingTime, PQAvgWaitingTime, avgResponseTime, avgCleanupTime, droppedArrivals);
}

void simulation(){

    double avgNumPatients = 0;
    double cumulativeArea = 0;

    //Initialize the first patient
    Patient* firstPatient = new Patient;
    firstPatient->arrivalTime = arrivalTimeOfPrevPatient + (-(1 / lambda) * log(rand() / (RAND_MAX + 1.0)));
    firstPatient->evaluationTime = (-(1 / mu_eval) * log(rand() / (RAND_MAX + 1.0)));
    firstPatient->treatmentTime = 0;
    firstPatient->nurseNumber = -99999;
    firstPatient->roomNumber = -99999;
    firstPatient->arrivalTimeIntoPQ = -99999;
    firstPatient->priorityValue = -99999;
    arrivalTimeOfPrevPatient = firstPatient->arrivalTime;

    //Initialize the first event and push it into the event list
    Event* firstPatientArriving = new Event;
    firstPatientArriving->timeOfEvent = firstPatient->arrivalTime;
    firstPatientArriving->type = 0;
    firstPatientArriving->patient = clonePatient(firstPatient);
    firstPatientArriving->room = NULL;
    eventList->push(firstPatientArriving);
    delete firstPatient;
    //Initialize subsystems
    EvaluationQueue EQueue = EvaluationQueue(num_nurses, total_patients, mu_treatment);
    PriorityQueue PQueue = PriorityQueue(num_rooms);
    RoomCleanUpQueue RoomQueue = RoomCleanUpQueue(mu_cleanup, num_janitors);

    double timeLastEvent = 0;

    //Main loop of simulation
    while(currentTime < 1800){

        if(eventList->isEmpty()){
            cout << "Event list is empty!" << endl;
            return;
        }
        //cout << "Before popping" << endl;
        //eventList->dumpEventList();

        Event* earlistEvent = eventList->peek();
        eventList->pop();
        currentTime = earlistEvent->timeOfEvent;

        //cout << "After popping" << endl;
        //eventList->dumpEventList();

        //cout << earlistEvent->type << endl;
        cumulativeArea += (currentTime-timeLastEvent)*(EQueue.returnNumPatientsInEQueue()+PQueue.returnNumPatientsInPQ());
        timeLastEvent = currentTime;
        if(earlistEvent->type == 0){
            //cout << "creating another patient" << endl << currentTime << endl;
            processNewArrival(earlistEvent->patient, earlistEvent);
        } else if(earlistEvent->type == 1){
            //cout << "Patient entering E queue" << endl << currentTime << endl;
            EQueue.processArrivalIntoQueue(earlistEvent->patient, eventList
                                           , PQueue.returnNumPatientsInPQ());
        } else if(earlistEvent->type == 2){
            //cout << "Patient being evaluated in E queue" << endl << currentTime << endl;
            EQueue.processEvaluation(earlistEvent, eventList, currentTime);
        } else if(earlistEvent->type == 3){
            //cout << "Patient leaving E queue" << endl << currentTime << endl;
            EQueue.processDeparture(earlistEvent, eventList, currentTime);
        } else if(earlistEvent->type == 4){
            //PQueue.dumpPQ();
            //cout << "Patient entering P queue" << endl << currentTime << endl;
            PQueue.processArrivalIntoPriorityQueue(earlistEvent, eventList);
            //PQueue.dumpPQ();

        } else if(earlistEvent->type == 5){
            //cout << "Patient being treated in P queue" << endl << currentTime << endl;
            PQueue.processTreatmentEvent(earlistEvent, eventList, currentTime);
        } else if(earlistEvent->type == 6){
            //PQueue.dumpPQ();

            //cout << "Patient leaving P queue" << endl << currentTime << endl;
            PQueue.processDepartureFromPQ(earlistEvent, eventList, currentTime);
        } else if(earlistEvent->type == 7){
            //cout << "Room waiting to be cleaned up" << endl << currentTime << endl;
            RoomQueue.processArrivalForCleanUp(earlistEvent, eventList, currentTime);
        } else if(earlistEvent->type == 8){
            //cout << "Room being cleaned up" << endl << currentTime << endl;
            RoomQueue.processCleanUp(earlistEvent, eventList, currentTime);
        } else if(earlistEvent->type == 9){
            //cout << "Room is done being cleaned" << endl << currentTime << endl;
            RoomQueue.processFinishingCleanUp(earlistEvent, eventList);
        } else if(earlistEvent->type == 10){
            //cout << "Ready to treat another patient" << endl;
            //cout << currentTime << endl;
            PQueue.processFinishingCleanUp(earlistEvent, eventList);
        }

        if(earlistEvent->patient != NULL){
            //cout << "Deleting earlistEvent->patient" << endl;
            delete earlistEvent->patient;
        }
        if(earlistEvent->room != NULL){
            //cout << "Deleting earlistEvent->room" << endl;
            delete earlistEvent->room;
        }
        //cout << "Deleting earlistEvent" << endl;
        delete earlistEvent;
    }

    PQueue.dumpPQ();
    avgNumPatients = cumulativeArea/currentTime;
    printStatistics(PQueue.returnNumDepartures(), avgNumPatients, EQueue.returnAvgWaitTime(), PQueue.returnAvgWaitTime(), PQueue.returnAvgResponseTime(),RoomQueue.returnAvgCleanUpTime(),EQueue.returnDroppedArrivals());
    //eventList->dumpEventList();
    delete eventList;
}

int main(int argc, char* argv[]){

    int inputsAllValid = verifyCommandLineArguments(argc, argv);
    if(inputsAllValid){
        srand(seed);
        simulation();
    }

}

