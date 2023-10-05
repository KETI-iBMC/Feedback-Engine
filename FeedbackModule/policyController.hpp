#pragma once

enum PolicyList{
    CPU=1,
    DIMM=2,
    Cabinet=3,
    Lm75=4,
};

struct TempPolicy{
    int redThres = 85;
    int orangeThres = 75;
    int yellowThres = 65;
    bool greenActive = true;
    bool yellowActive = true;
    bool orangeActive = true;
    bool redActive = true;
    bool wellConnected = false;
};

class PolicyController{
    public:
        TempPolicy cpuTempPolicy;
        TempPolicy dimmTempPolicy;
        TempPolicy cabinetTempPolicy;
        TempPolicy lm75TempPolicy;

        PolicyController(){};
    private:


};

void readPolicy();
int* returnPolicy(int type);
void getCurrentPolicy();
void printPolicy(TempPolicy policy);