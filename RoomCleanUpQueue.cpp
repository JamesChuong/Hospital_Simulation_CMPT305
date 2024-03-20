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

void RoomCleanUpQueue::processArrivalForCleanUp(Event arrivalEvent, EventList* eventList, double currentTime){
    RoomQueue.push(arrivalEvent.room);
    for(int i = 0; i < numJanitors; i++){
        if(Janitors[i].notBusy){
            Event cleanupEvent;
            cleanupEvent.timeOfEvent = currentTime;
            cleanupEvent.type = 8;
            cleanupEvent.room = arrivalEvent.room;
            cleanupEvent.room.janitorNumber = i;
            eventList->push(cleanupEvent);
        }
    }
}

void RoomCleanUpQueue::processCleanUp(Event cleanUpEvent, EventList* eventList){
    if(RoomQueue.empty()) return;

    Room nextRoom = RoomQueue.front();
    RoomQueue.pop();
    nextRoom.janitorNumber = cleanUpEvent.room.janitorNumber;
    Janitors[cleanUpEvent.room.janitorNumber].notBusy = false;
    cumulativeWaitingTimePlusWaitingTime += (cleanUpEvent.timeOfEvent - nextRoom.arrivalTimeForCleanup + 1/mu_cleanup);

    Event finishingCleanUpEvent;
    finishingCleanUpEvent.timeOfEvent = cleanUpEvent.timeOfEvent + (1/mu_cleanup);
    finishingCleanUpEvent.type = 9;
    finishingCleanUpEvent.room = nextRoom;
    eventList->push(finishingCleanUpEvent);

}

void RoomCleanUpQueue::processFinishingCleanUp(Event finishCleanUpEvent, EventList* eventList){
    Janitors[finishCleanUpEvent.room.janitorNumber].notBusy = true;
    numDepartures++;
    if(!RoomQueue.empty()){
        Event nextCleanUpEvent;
        nextCleanUpEvent.timeOfEvent = finishCleanUpEvent.timeOfEvent;
        nextCleanUpEvent.type = 8;
        nextCleanUpEvent.room.janitorNumber = finishCleanUpEvent.room.janitorNumber;
        eventList->push(nextCleanUpEvent);
    }
    Event roomIsReadyEvent;
    roomIsReadyEvent.timeOfEvent = finishCleanUpEvent.timeOfEvent;
    roomIsReadyEvent.type = 10;
    roomIsReadyEvent.room = finishCleanUpEvent.room;
    eventList->push(roomIsReadyEvent);
}

double RoomCleanUpQueue::returnAvgCleanUpTime(){
    return cumulativeWaitingTimePlusWaitingTime/numDepartures;
}