#pragma once

struct CPUTempPolicy
{
    double redThres = 85;
    double orangeThres = 75;
    double yellowThres = 65;
};

struct DIMMTempPolicy
{
    double redThres = 85;
    double orangeThres = 75;
    double yellowThres = 65;
};


class PolicyController{
    public:
        CPUTempPolicy cpuTempPolicy;
        DIMMTempPolicy dimmTempPolicy;
        PolicyController(){};
    private:


};

double* returnPolicy(int type);
void getCurrentPolicy();