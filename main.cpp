//
// Created by james on 10/03/24.
//

#include <iostream>
#include <vector>
#include "EventList.cpp"

using namespace std;

//Command line arguments
double lambda = 0;
double mu_eval = 0;
double mu_treatment = 0;
double mu_cleanup = 0;
double total_patients = 0;
double num_rooms = 0;
double num_nurses = 0;
double num_janitors = 0;
double seed = 0;

//Global variables
double currentTime;

struct Event{
    double timeOfEvent;
    int type;
    double nodeArrivalTime;
    double nodeServiceTime;
};

struct Patient{
    double arrivalTime;
    double evaluatiomTime;
    double treatmentTime;
    double priorityValue;
};

int verifyCommandLineArguments(int argc, char* argv[]){
    if(argc < 10) {
        cout << "ERROR: Not enough arguments" << endl;
        return 0;
    }
    lambda = atof(argv[1]);
    mu_eval = atof(argv[2]);
    mu_treatment = atof(argv[3]);
    mu_cleanup = atof(argv[4]);
    total_patients = atoi(argv[5]);
    num_rooms = atoi(argv[6]);
    num_nurses = atoi(argv[7]);
    num_janitors = atoi(argv[8]);
    seed = atof(argv[9]);

    if(lambda <= 0){
        cout << "ERROR: lambda must be non-negative" << endl;
        return 0;
    }
    if(mu_eval <= 0){
        cout << "ERROR: mu_eval must be non-negative" << endl;
        return 0;
    }
    if(mu_treatment <= 0){
        cout << "ERROR: mu_treatment must be non-negative" << endl;
        return 0;
    }
    if(mu_cleanup <= 0){
        cout << "ERROR: mu_cleanup must be non-negative" << endl;
        return 0;
    }
    if(total_patients <= 0){
        cout << "ERROR: total_patients must be non-negative" << endl;
        return 0;
    }
    if(num_rooms <= 0){
        cout << "ERROR: num_rooms must be non-negative" << endl;
        return 0;
    }
    if(num_nurses <= 0){
        cout << "ERROR: num_nurses must be non-negative" << endl;
        return 0;
    }
    if(num_janitors <= 0){
        cout << "ERROR: num_janitors must be non-negative" << endl;
        return 0;
    }
    if(seed <= 0){
        cout << "ERROR: seed must be non-negative" << endl;
        return 0;
    }
    return 1;
}


void simulation();



int main(int argc, char* argv[]){

    int inputsAllValid = verifyCommandLineArguments(argc, argv);
    if(inputsAllValid){

    }

}

