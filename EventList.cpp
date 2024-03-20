//
// Created by james on 10/03/24.
//

#include <vector>
#include <iostream>
#include "main.h"
#include "EventList.h"
using namespace std;

void EventList::bubbleDown(int index) {
    int leftChild = 2*index+1;
    int rightChild = 2*index+2;
    int smallerChild = leftChild;
    //Base case
    if(reinterpret_cast<unsigned long>(left) <= eventList.size() - 1){
        if(eventList[leftChild].timeOfEvent > eventList[rightChild].timeOfEvent){
            smallerChild = rightChild;
        } else if (eventList[leftChild].timeOfEvent == eventList[rightChild].timeOfEvent){
            if(eventList[leftChild].type < eventList[rightChild].type){
                smallerChild = rightChild;
            }
        }
        if(eventList[smallerChild].timeOfEvent < eventList[index].timeOfEvent){
            swap(eventList[smallerChild], eventList[index]);
            bubbleDown(smallerChild);
        }
    }
}

void EventList::bubbleUp(int index){
    int parent = (index-1)/2;
    if(parent >= 0){
        if(eventList[parent].timeOfEvent > eventList[index].timeOfEvent){
            swap(eventList[parent], eventList[index]);
            bubbleUp(parent);
        }
    }
}

void EventList::pop(){
    if(eventList.size() == 0) return;
    swap(eventList[0], eventList[eventList.size()-1]);
    int newSize = eventList.size()-1;
    eventList.resize(newSize);
    if(eventList.size() != 0){
        bubbleDown(0);
    }
}

Event EventList::peek(){
    return eventList[0];
}

void EventList::push(Event newEvent){
    eventList.push_back(newEvent);
    bubbleUp(eventList.size()-1);
}

bool EventList::isEmpty(){
    return eventList.empty();
}