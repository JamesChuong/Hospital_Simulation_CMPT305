//
// Created by james on 13/03/24.
//
#include <vector>
#include <iostream>
#include <queue>
#include "main.cpp"
#include "EvaluationQueue.cpp"

class PriorityQueue{
private:

    double num_rooms = 0;
    double cumulativeWaitingTime;
    double cumulativeResponseTime;

    struct Room{
        bool isReady;       //true if it is ready for patients, false otherwise
    };

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
        }
    }

    void processArrivalIntoPriorityQueue(Patient patient, EventList eventList){

        pushIntoPriorityList(patient);

        for(int i = 0; i < num_rooms; i++){
            if(Rooms[i].isReady){
                patient.roomNumber = i;
                Event arrivalToPQ;
                arrivalToPQ.timeOfEvent = patient.arrivalTime;
                arrivalToPQ.patient = patient;
                arrivalToPQ.type = 5;
                eventList.push(arrivalToPQ);
            }
        }
    }

    void processTreatmentEvent(Event treatmentEvent, EventList eventList){
        if(priorityList.empty()) return;

        Patient nextPatient;
        peek(&nextPatient);
        popFromPriorityList();
        Rooms[treatmentEvent.patient.roomNumber].isReady = false;
        if(currentTime % 60 > 6){
            cumulativeWaitingTime += (treatmentEvent.timeOfEvent - nextPatient.arrivalTime);
        }

        if(!priorityList.empty()){
            Event departureFromRoom;
            departureFromRoom.timeOfEvent = treatmentEvent.timeOfEvent+nextPatient.treatmentTime;
            departureFromRoom.type = 6;
            departureFromRoom.patient = nextPatient;
            eventList.push(departureFromRoom);
        }

    }
};
