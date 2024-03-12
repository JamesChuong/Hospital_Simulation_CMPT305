//
// Created by james on 11/03/24.
//

#include <vector>
#include <iostream>
#include <queue>
#include "main.cpp"

using namespace std;

class Hospital{

private:



    double mu_eval = 0;
    double mu_treatment = 0;
    double mu_cleanup = 0;
    double total_patients = 0;
    double num_rooms = 0;
    double num_nurses = 0;
    double num_janitors = 0;
    double numPatientsInHospital = 0;

    struct Nurse{
        bool isBusy;     //false for busy, true otherwise
    };

    queue<struct Patient> evaluationQueue;
    vector<Patient> priorityList;
    vector<Nurse> Nurses;

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


    Hospital(double mu_eval, double mu_treatment, double num_rooms, double mu_cleanup
             , double num_janitors, double num_nurses, double total_patients){
        this->mu_eval = mu_eval;
        this->mu_treatment = mu_treatment;
        this->num_rooms = num_rooms;
        this->mu_cleanup = mu_cleanup;
        this->num_nurses = num_nurses;
        this->mu_treatment = mu_treatment;
        this->total_patients = total_patients;

        for(int i = 0; i < total_patients; i++){
            struct Nurse newNurse;
            newNurse.isBusy = true;
            Nurses.push_back(newNurse);
        }
    }

    void processArrival(Patient newPatient, EventList eventList){
        if(numPatientsInHospital < total_patients){
            evaluationQueue.push(newPatient);
            numPatientsInHospital++;

            for(int i = 0; i < Nurses.size(); i++){
                if(!Nurses[i].isBusy){
                    Event newServiceEvent;
                    newServiceEvent.timeOfEvent = newPatient.arrivalTime;
                    newServiceEvent.type = 2;
                    newServiceEvent.nodeEvaluationTime = -99999;
                    newServiceEvent.nodeEvaluationTime = -99999;
                    eventList.push(newServiceEvent);
                    return;
                }
            }
        }
    }
};

