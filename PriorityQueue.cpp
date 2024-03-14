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
    double num_janitors = 0;
    double mu_treatment = 0;
    double mu_cleanup = 0;

    struct Room{
        bool isBeingCleaned;
        bool isReady;
    };

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

    void pushFromPriorityList(Patient newPatient){
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

public:


};
