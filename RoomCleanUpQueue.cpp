//
// Created by james on 17/03/24.
//

#include <vector>
#include <queue>
#include "main.h"
#include "EventList.h"
#include "RoomCleanUpQueue.h"
using namespace std;

RoomCleanUpQueue::RoomCleanUpQueue(double mu_cleanup, double numJanitors){
    this->mu_cleanup = mu_cleanup;
    this->numJanitors = numJanitors;
    for(int i = 0; i < numJanitors; i++){
        Janitor newJanitor;
        newJanitor.notBusy = true;
        Janitors.push_back(newJanitor);
    }
}

RoomCleanUpQueue::~RoomCleanUpQueue(){
    while(!RoomQueue.empty()){
        Room* currentRoom = RoomQueue.front();
        RoomQueue.pop();
        if(currentRoom != NULL){
            delete currentRoom;
        }
    }
}

Room* RoomCleanUpQueue::cloneRoom(Room* newRoom){
    if(newRoom == NULL) return NULL;
    Room* clonedRoom = new Room;
    clonedRoom->janitorNumber = newRoom->janitorNumber;
    clonedRoom->roomNumber = newRoom->roomNumber;
    clonedRoom->isReady = newRoom->isReady;
    clonedRoom->arrivalTimeForCleanup = newRoom->arrivalTimeForCleanup;
    return clonedRoom;
}

void RoomCleanUpQueue::processArrivalForCleanUp(Event* arrivalEvent, EventList* eventList, double currentTime){
    if(arrivalEvent == NULL) return;
    Room* newRoom = cloneRoom(arrivalEvent->room);
    RoomQueue.push(newRoom);
    for(int i = 0; i < numJanitors; i++){
        if(Janitors[i].notBusy){
            Event* cleanupEvent = new Event;
            cleanupEvent->timeOfEvent = currentTime;
            cleanupEvent->type = 8;
            cleanupEvent->patient = NULL;
            cleanupEvent->room = cloneRoom(newRoom);
            cleanupEvent->room->janitorNumber = i;
            eventList->push(cleanupEvent);
            return;
        }
    }
}

void RoomCleanUpQueue::processCleanUp(Event* cleanUpEvent, EventList* eventList, double currentTime){
    if(RoomQueue.empty() || cleanUpEvent == NULL) return;
    Room* nextRoom = RoomQueue.front();
    RoomQueue.pop();
    nextRoom->janitorNumber = cleanUpEvent->room->janitorNumber;
    Janitors[cleanUpEvent->room->janitorNumber].notBusy = false;
    if(currentTime / 60 > 6){
        cumulativeWaitingTimePlusWaitingTime += (cleanUpEvent->timeOfEvent - nextRoom->arrivalTimeForCleanup + 1/mu_cleanup);
    }
    Event* finishingCleanUpEvent = new Event;
    finishingCleanUpEvent->timeOfEvent = cleanUpEvent->timeOfEvent + (1/mu_cleanup);
    finishingCleanUpEvent->type = 9;
    finishingCleanUpEvent->patient = NULL;
    finishingCleanUpEvent->room = nextRoom;
    eventList->push(finishingCleanUpEvent);
}

void RoomCleanUpQueue::processFinishingCleanUp(Event* finishCleanUpEvent, EventList* eventList){
    if(finishCleanUpEvent == NULL) return;
    Janitors[finishCleanUpEvent->room->janitorNumber].notBusy = true;
    numDepartures++;
    if(!RoomQueue.empty()){
        Event* nextCleanUpEvent = new Event;
        nextCleanUpEvent->timeOfEvent = finishCleanUpEvent->timeOfEvent;
        nextCleanUpEvent->type = 8;
        nextCleanUpEvent->patient = NULL;
        nextCleanUpEvent->room = cloneRoom(finishCleanUpEvent->room);
        eventList->push(nextCleanUpEvent);
    }
    Event* roomIsReadyEvent = new Event;
    roomIsReadyEvent->timeOfEvent = finishCleanUpEvent->timeOfEvent;
    roomIsReadyEvent->type = 10;
    roomIsReadyEvent->patient = NULL;
    roomIsReadyEvent->room = cloneRoom(finishCleanUpEvent->room);
    eventList->push(roomIsReadyEvent);
}

double RoomCleanUpQueue::returnAvgCleanUpTime(){
    return cumulativeWaitingTimePlusWaitingTime/numDepartures;
}