//
// Created by james on 13/03/24.
//
#include <vector>
#include <iostream>
#include "main.h"
#include "EventList.h"
#include "PriorityQueue.h"

PriorityQueue::PriorityQueue(double num_rooms){
    this->num_rooms = num_rooms;
    for(int i = 0; i < this->num_rooms; i++){
        Room* newRoom = new Room{true, -99999, i, -99999};
        Rooms.push_back(newRoom);
    }
}

PriorityQueue::~PriorityQueue(){
    while(!priorityList.empty()){
        Patient* currentPatient = priorityList.back();
        priorityList.pop_back();
        delete currentPatient;
    }

    while(!Rooms.empty()){
        Room* currentRoom = Rooms.back();
        Rooms.pop_back();
        delete currentRoom;
    }
}

void PriorityQueue::bubbleDown(int index){
    int leftChild = 2*index+1;
    int rightChild = 2*index+2;
    int largerChild = leftChild;
    //Base case
    if(leftChild <= (int)priorityList.size() - 1){

        if(rightChild <= (int)priorityList.size() -1){
            if(priorityList[leftChild]->priorityValue < priorityList[rightChild]->priorityValue){
                largerChild = rightChild;
            }
        }

        if(priorityList[largerChild]->priorityValue < priorityList[index]->priorityValue){
            swap(priorityList[largerChild], priorityList[index]);
            bubbleDown(largerChild);
        }
    }
}

void PriorityQueue::bubbleUp(int index){
    int parent = (index-1)/2;
    if(parent >= 0){
        if(priorityList[parent]->priorityValue < priorityList[index]->priorityValue){
            swap(priorityList[parent], priorityList[index]);
            bubbleUp(parent);
        }
    }
}

void PriorityQueue::pushIntoPriorityList(Patient* newPatient){
    priorityList.push_back(newPatient);
    bubbleUp(priorityList.size()-1);
}

void PriorityQueue::popFromPriorityList(){
    if(priorityList.size() == 0) return;
    swap(priorityList[0], priorityList[priorityList.size()-1]);
    int newSize = priorityList.size()-1;
    priorityList.resize(newSize);
    if(priorityList.size() != 0){
        bubbleDown(0);
    }
}

Patient* PriorityQueue::peek(){
    if(priorityList.empty()) return NULL;
    return priorityList[0];
}

Patient* PriorityQueue::clonePatient(Patient* newPatient){
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

Room* PriorityQueue::cloneRoom(Room* newRoom){
    if(newRoom == NULL) return NULL;
    Room* clonedRoom = new Room;
    clonedRoom->janitorNumber = newRoom->janitorNumber;
    clonedRoom->roomNumber = newRoom->roomNumber;
    clonedRoom->isReady = newRoom->isReady;
    clonedRoom->arrivalTimeForCleanup = newRoom->arrivalTimeForCleanup;
    return clonedRoom;
}

void PriorityQueue::processArrivalIntoPriorityQueue(Event* arrivalEvent, EventList* eventList){
    if(arrivalEvent == NULL || arrivalEvent->patient == NULL) return;
    Patient* newPatient = clonePatient(arrivalEvent->patient);
    pushIntoPriorityList(newPatient);
    numPatientsInPQ++;
    for(int i = 0; i < num_rooms; i++){
        if(Rooms[i]->isReady){
            Event* arrivalToRoom = new Event;
            newPatient->roomNumber = i;
            arrivalToRoom->timeOfEvent = newPatient->arrivalTimeIntoPQ;
            arrivalToRoom->patient = clonePatient(newPatient);
            arrivalToRoom->type = 5;
            arrivalToRoom->room = cloneRoom(Rooms[i]);
            eventList->push(arrivalToRoom);
            return;
        }
    }
}

void PriorityQueue::processTreatmentEvent(Event* treatmentEvent, EventList* eventList, double currentTime){
    if(priorityList.empty() || treatmentEvent == NULL) return;

    numPatientsFinishingTreatment++;

    Patient* nextPatient = peek();
    popFromPriorityList();

    Rooms[treatmentEvent->room->roomNumber]->isReady = false;

    if(currentTime / 60 > 6){
        cumulativeWaitingTime += (treatmentEvent->timeOfEvent - nextPatient->arrivalTime);
    }

    Event* departureFromRoom = new Event;
    nextPatient->roomNumber = treatmentEvent->room->roomNumber;
    departureFromRoom->timeOfEvent = treatmentEvent->timeOfEvent+nextPatient->treatmentTime;
    departureFromRoom->type = 6;
    departureFromRoom->patient = nextPatient;
    departureFromRoom->room = cloneRoom(treatmentEvent->room);
    eventList->push(departureFromRoom);
}

void PriorityQueue::processDepartureFromPQ(Event* departureEvent, EventList* eventList, double currentTime){
    if(departureEvent == NULL || departureEvent->patient == NULL) return;
    numPatientsInPQ--;
    numDepartures++;
    if(currentTime / 60 > 6){
        cumulativeResponseTime += (departureEvent->timeOfEvent - departureEvent->patient->arrivalTime);
    }
    Event* roomCleanupEvent = new Event;
    roomCleanupEvent->timeOfEvent = departureEvent->timeOfEvent;
    roomCleanupEvent->type = 7;
    roomCleanupEvent->patient = clonePatient(departureEvent->patient);
    roomCleanupEvent->room = cloneRoom(departureEvent->room);
    if(roomCleanupEvent->room != NULL){
        roomCleanupEvent->room->arrivalTimeForCleanup = departureEvent->timeOfEvent;
    }
    eventList->push(roomCleanupEvent);
}

void PriorityQueue::processFinishingCleanUp(Event* roomIsReadyEvent, EventList* eventList){
    Rooms[roomIsReadyEvent->room->roomNumber]->isReady = true;
    if(!priorityList.empty()){
        Event* nextServiceEvent = new Event;
        nextServiceEvent->timeOfEvent = roomIsReadyEvent->timeOfEvent;
        nextServiceEvent->type = 5;
        nextServiceEvent->patient = clonePatient(roomIsReadyEvent->patient);
        nextServiceEvent->room = cloneRoom(roomIsReadyEvent->room);
        eventList->push(nextServiceEvent);
    }
}

double PriorityQueue::returnNumPatientsInPQ(){
    return numPatientsInPQ;
}

double PriorityQueue::returnAvgWaitTime(){
    return cumulativeWaitingTime/numPatientsFinishingTreatment;
}

double PriorityQueue::returnAvgResponseTime(){
    return cumulativeResponseTime/numDepartures;
}

int PriorityQueue::returnNumDepartures(){
    return numDepartures;
}