#pragma once
#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>
#include <string>
#include "feedbackDbus.hpp"
enum PolicyList{
    CPU=1,
    DIMM=2,
    Cabinet=3,
    Disk=4,
};

enum ModuleIdentNo{
    None = 0,
    cpu = 1,
    DIMM1 = 2,
    DIMM2 = 3,
    DIMM3 = 4,
    DIMM4 = 5,
    DIMM5 = 6,
    DIMM6 = 7,
    DIMM7 = 8,
    DIMM8 = 9,
    NVME0 = 10,
    cabinet = 11,
};

struct TempPolicy{
    std::string moduleName;
    std::string state = "GREEN";
    int redThres = 85;
    int orangeThres = 75;
    int yellowThres = 65;
    bool greenActive = true;
    bool yellowActive = true;
    bool orangeActive = true;
    bool redActive = true;
    bool activation = true;
};

class PolicyController{
    public:
        TempPolicy cpuTempPolicy[4];
        TempPolicy dimmTempPolicy[8];
        TempPolicy cabinetTempPolicy[1];
        TempPolicy diskTempPolicy[4];

        PolicyController(){};
    private:
};

void initPolicy();
void readPolicy();
int* returnPolicy(int type, int num);
void getCurrentPolicy();
void printPolicy(TempPolicy policy);
void getPolicy(std::string json);
int getModuleNum(const char* string);
bool* getActivation(int type, int moduleNum);
bool intToBool(int num);
void updateState(int moduleName, int moduleNum, int state);
void updatefoflJson();
void updateFOFL_Policy(int moduleName, int moduleNUm, int state);