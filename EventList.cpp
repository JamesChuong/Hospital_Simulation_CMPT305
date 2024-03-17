//
// Created by james on 10/03/24.
//

#include <vector>
#include <iostream>
#include "main.cpp"

using namespace std;
class EventList {
private:

    //The implementation of the event list is a min heap, that way removals are done in O(log(n)) time
    vector<struct Event> eventList;

    void bubbleDown(int index){
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

    void bubbleUp(int index){
        int parent = (index-1)/2;
        if(parent >= 0){
            if(eventList[parent].timeOfEvent > eventList[index].timeOfEvent){
                swap(eventList[parent], eventList[index]);
                bubbleUp(parent);
            }
        }
    }

public:

    void push(Event newEvent){
        eventList.push_back(newEvent);
        bubbleUp(eventList.size()-1);
    }

    void pop(){
        if(eventList.size() == 0) return;
        swap(eventList[0], eventList[eventList.size()-1]);
        int newSize = eventList.size()-1;
        eventList.resize(newSize);
        bubbleDown(0);
    }

    struct Event peek(){
        return eventList[0];
    }

};