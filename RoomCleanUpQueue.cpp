//
// Created by james on 17/03/24.
//

#include <vector>
#include <queue>
#include "main.cpp"
class RoomCleanUpQueue{

private:

    queue<Room> RoomQueue;

    double mu_cleanup;
    double numJanitors;
    double cumulativeWaitingTimePlusWaitingTime = 0;
    int numDepartures = 0;

    struct Janitor{
        bool notBusy;    //true if a janitor is not cleaning a room, false otherwise
    };
    vector<Janitor> Janitors;

public:

    RoomCleanUpQueue(double mu_cleanup, double numJanitors){
        this->mu_cleanup = mu_cleanup;
        this->numJanitors = numJanitors;
        for(int i = 0; i < numJanitors; i++){
            Janitors[i].notBusy = true;
        }
    }

    void processArrivalForCleanUp(Event arrivalEvent, EventList eventList){
        RoomQueue.push(arrivalEvent.room);
        for(int i = 0; i < numJanitors; i++){
            if(Janitors[i].notBusy){
                Event cleanupEvent;
                cleanupEvent.timeOfEvent = currentTime;
                cleanupEvent.type = 8;
                cleanupEvent.room = arrivalEvent.room;
                cleanupEvent.room.janitorNumber = i;
                eventList.push(cleanupEvent);
            }
        }
    }

    void processCleanUp(Event cleanUpEvent, EventList eventList){
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
        eventList.push(finishingCleanUpEvent);

    }

    void processFinishingCleanUp(Event finishCleanUpEvent, EventList eventList){
        Janitors[finishCleanUpEvent.room.janitorNumber].notBusy = true;
        numDepartures++;
        if(!RoomQueue.empty()){
            Event nextCleanUpEvent;
            nextCleanUpEvent.timeOfEvent = finishCleanUpEvent.timeOfEvent;
            nextCleanUpEvent.type = 8;
            nextCleanUpEvent.room.janitorNumber = finishCleanUpEvent.room.janitorNumber;
            eventList.push(nextCleanUpEvent);
        }
        Event roomIsReadyEvent;
        roomIsReadyEvent.timeOfEvent = finishCleanUpEvent.timeOfEvent;
        roomIsReadyEvent.type = 10;
        roomIsReadyEvent.room = finishCleanUpEvent.room;
        eventList.push(roomIsReadyEvent);
    }

    double returnAvgCleanUpTime(){
        return cumulativeWaitingTimePlusWaitingTime/numDepartures;
    }

};