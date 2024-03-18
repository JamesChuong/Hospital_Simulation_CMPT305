//
// Created by james on 13/03/24.
//
#include <vector>
#include <iostream>
#include "main.cpp"
#include "EvaluationQueue.cpp"

class PriorityQueue{
private:

    double num_rooms = 0;
    double cumulativeWaitingTime = 0;
    double cumulativeResponseTime = 0;
    double numPatientsInPQ = 0;
    double numDepartures = 0;
    double numPatientsFinishingTreatment = 0;
    vector<Room> Rooms;
    vector<Patient> priorityList;

    void bubbleDown(int index){
        int leftChild = 2*index+1;
        int rightChild = 2*index+2;
        int smallerChild = leftChild;
        //Base case
        if(reinterpret_cast<unsigned long>(left) <= priorityList.size() - 1){
            if(priorityList[leftChild].priorityValue > priorityList[rightChild].priorityValue){
                smallerChild = rightChild;
            }
            if(priorityList[smallerChild].priorityValue < priorityList[index].priorityValue){
                swap(priorityList[smallerChild], priorityList[index]);
                bubbleDown(smallerChild);
            }
        }
    }

    void bubbleUp(int index){
        int parent = (index-1)/2;
        if(parent >= 0){
            if(priorityList[parent].priorityValue > priorityList[index].priorityValue){
                swap(priorityList[parent], priorityList[index]);
                bubbleUp(parent);
            }
        }
    }

    void pushIntoPriorityList(Patient newPatient){
        priorityList.push_back(newPatient);
        bubbleUp(priorityList.size()-1);
    }

    void popFromPriorityList(){
        if(priorityList.size() == 0) return;
        swap(priorityList[0], priorityList[priorityList.size()-1]);
        int newSize = priorityList.size()-1;
        priorityList.resize(newSize);
        bubbleDown(0);
    }

    void peek(Patient* patient){
        if(priorityList.empty()) return;
        *patient = priorityList.front();
    }

public:

    PriorityQueue(double num_rooms){
        this->num_rooms = num_rooms;
        for(int i = 0; i < this->num_rooms; i++){
            Rooms[i].isReady = true;
            Rooms[i].roomNumber = i;
        }
    }

    void processArrivalIntoPriorityQueue(Patient patient, EventList eventList){
        pushIntoPriorityList(patient);
        numPatientsInPQ++;
        for(int i = 0; i < num_rooms; i++){
            if(Rooms[i].isReady){
                Event arrivalToRoom;
                patient.roomNumber = i;
                arrivalToRoom.timeOfEvent = patient.arrivalTimeIntoPQ;
                arrivalToRoom.patient = patient;
                arrivalToRoom.type = 5;
                arrivalToRoom.room = Rooms[i];
                eventList.push(arrivalToRoom);
            }
        }
    }

    void processTreatmentEvent(Event treatmentEvent, EventList eventList){
        if(priorityList.empty()) return;
        numPatientsFinishingTreatment++;
        Patient nextPatient;
        peek(&nextPatient);
        popFromPriorityList();
        Rooms[treatmentEvent.room.roomNumber].isReady = false;
        if(currentTime / 60 > 6){
            cumulativeWaitingTime += (treatmentEvent.timeOfEvent - nextPatient.arrivalTime);
        }
        Event departureFromRoom;
        departureFromRoom.timeOfEvent = treatmentEvent.timeOfEvent+nextPatient.treatmentTime;
        departureFromRoom.type = 6;
        departureFromRoom.patient = nextPatient;
        eventList.push(departureFromRoom);
    }

    void processDepartureFromPQ(Event departureEvent, EventList eventList){
        numPatientsInPQ--;
        numDepartures++;
        if(currentTime / 60 > 6){
            cumulativeResponseTime += (departureEvent.timeOfEvent - departureEvent.patient.arrivalTime);
        }
        Event roomCleanupEvent;
        roomCleanupEvent.timeOfEvent = departureEvent.timeOfEvent;
        roomCleanupEvent.type = 7;
        roomCleanupEvent.patient = departureEvent.patient;
        roomCleanupEvent.room = Rooms[departureEvent.patient.roomNumber];
        roomCleanupEvent.room.arrivalTimeForCleanup = departureEvent.timeOfEvent;
        eventList.push(roomCleanupEvent);
    }

    void processFinishingCleanUp(Event roomIsReadyEvent, EventList eventList){
        Rooms[roomIsReadyEvent.room.roomNumber].isReady = true;
        if(!priorityList.empty()){
            Event nextServiceEvent;
            nextServiceEvent.timeOfEvent = roomIsReadyEvent.timeOfEvent;
            nextServiceEvent.type = 5;
            nextServiceEvent.room = Rooms[roomIsReadyEvent.room.roomNumber];
            eventList.push(nextServiceEvent);
        }
    }
    double returnNumPatientsInPQ(){
        return numPatientsInPQ;
    }

    double returnAvgWaitTime(){
        return cumulativeWaitingTime/numPatientsFinishingTreatment;
    }

    double returnAvgResponseTime(){
        return cumulativeResponseTime/numDepartures;
    }
};
