#include "policyController.hpp"

PolicyController policyController;

void initPolicy(){
    getCurrentPolicy();

}

double* returnPolicy(int type){
    double *thres = nullptr;
    switch (type)
    {
    case 0:
        thres = new double[3];
        thres[0] = policyController.cpuTempPolicy.yellowThres;
        thres[1] = policyController.cpuTempPolicy.orangeThres;
        thres[2] = policyController.cpuTempPolicy.redThres;
        return thres;
        break;
    }
}

void getCurrentPolicy(){

}