#include "policyController.hpp"
#include "feedbackDbus.hpp"
#include <chrono>
#include <iostream>
#include <thread>

PolicyController policyController;

void initPolicy(){

}

void readPolicy(){
    while(true){
        policyController.cpuTempPolicy = getTempPolicy(PolicyList::CPU);
        policyController.dimmTempPolicy = getTempPolicy(PolicyList::DIMM);
        policyController.cabinetTempPolicy = getTempPolicy(PolicyList::Cabinet);
        policyController.lm75TempPolicy = getTempPolicy(PolicyList::Lm75);
        printPolicy(policyController.cpuTempPolicy);
        printPolicy(policyController.dimmTempPolicy);
        printPolicy(policyController.cabinetTempPolicy);
        printPolicy(policyController.lm75TempPolicy);
        std::this_thread::sleep_for(std::chrono::seconds(20));
    }
}

int* returnPolicy(int type){
    int *thres = nullptr;
    switch (type)
    {
    case 0:
        thres = new int[3];
        thres[0] = policyController.cpuTempPolicy.yellowThres;
        thres[1] = policyController.cpuTempPolicy.orangeThres;
        thres[2] = policyController.cpuTempPolicy.redThres;
        return thres;
        break;
    case 1:
        thres = new int[3];
        thres[0] = policyController.dimmTempPolicy.yellowThres;
        thres[1] = policyController.dimmTempPolicy.orangeThres;
        thres[2] = policyController.dimmTempPolicy.redThres;
        return thres;
        break;
    case 2:
        thres = new int[3];
        thres[0] = policyController.cabinetTempPolicy.yellowThres;
        thres[1] = policyController.cabinetTempPolicy.orangeThres;
        thres[2] = policyController.cabinetTempPolicy.redThres;
        return thres;
        break;
    case 3:
        thres = new int[3];
        thres[0] = policyController.lm75TempPolicy.yellowThres;
        thres[1] = policyController.lm75TempPolicy.orangeThres;
        thres[2] = policyController.lm75TempPolicy.redThres;
        return thres;
        break;
    }
}

void printPolicy(TempPolicy tempPolicy){
    if(tempPolicy.wellConnected = false){
        std::cout<<"Can't get Policy\n";
        return;
    }
    std::cout<<tempPolicy.greenActive<<" "<<tempPolicy.yellowThres<<" "<<tempPolicy.yellowActive<<" ";
    std::cout<<tempPolicy.orangeThres<<" "<<tempPolicy.orangeActive<<" "<<tempPolicy.redThres<<" "<<tempPolicy.redActive<<"\n";
    return;
}


void getCurrentPolicy(){

}

