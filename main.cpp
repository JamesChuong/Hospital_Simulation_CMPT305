//
// Created by james on 10/03/24.
//

#include <iostream>
#include <vector>
#include "EventList.cpp"
#include "EvaluationQueue.cpp"
#include "PriorityQueue.cpp"
#include "RoomCleanUpQueue.cpp"
#include <queue>
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
double currentTime;
double arrivalTimeOfPrevPatient = 0;

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
    Patient patient;
    Room room;
};

//The event list for the entire simulation
EventList eventList = EventList();


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
    eventList.push(arrivalIntoEQueue);

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
    eventList.push(nextPatientArrival);

}

void simulation(){

    //Initialize the first patient
    Patient firstPatient;
    firstPatient.arrivalTime = arrivalTimeOfPrevPatient + (-(1 / lambda) * log(rand() / (RAND_MAX + 1.0)));
    firstPatient.evaluationTime = (-(1 / mu_eval) * log(rand() / (RAND_MAX + 1.0)));
    firstPatient.treatmentTime = (-(1 / mu_treatment) * log(rand() / (RAND_MAX + 1.0)));

    //Initialize the first event
    Event firstPatientArriving;
    firstPatientArriving.timeOfEvent = firstPatient.arrivalTime;
    firstPatientArriving.type = 0;
    firstPatientArriving.patient = firstPatient;
    eventList.push(firstPatientArriving);

    EvaluationQueue EQueue = EvaluationQueue(num_nurses, total_patients);
    PriorityQueue PQueue = PriorityQueue(num_rooms);

    while(currentTime/60 < 30){

        if(eventList.isEmpty()){
            printStatistics(EQueue.returnNumPatientsInEQueue()+PQueue.returnNumPatientsInPQ()
                            , EQueue.returnAvgWaitTime(), PQueue.returnAvgWaitTime()
                            , PQueue.returnAvgResponseTime(), EQueue.returnDroppedArrivals());
        }
    }

}


int main(int argc, char* argv[]){

    int inputsAllValid = verifyCommandLineArguments(argc, argv);
    if(inputsAllValid){
        srand(seed);
    }

}

