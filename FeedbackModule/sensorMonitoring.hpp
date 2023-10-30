#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include "foflManager.hpp"


struct Sensor{
    int CPU0_DIMM1_TEMP;
    int CPU0_DIMM2_TEMP;
    int CPU0_DIMM3_TEMP;
    int CPU0_DIMM4_TEMP;
    int CPU0_DIMM5_TEMP;
    int CPU0_DIMM6_TEMP;
    int CPU0_DIMM7_TEMP;
    int CPU0_DIMM8_TEMP;
    int CPU0_TEMP;
    int CPU1_TEMP;
    int LM75_TEMP0;
    int LM75_TEMP1;
    int LM75_TEMP2;
    int LM75_TEMP3;
    int LM75_TEMP4;
    int CabinetTemp;
    int Disk_Temp;
};

class SensorMonitoring{
    public:
    SensorMonitoring(){};
    Sensor sensor;
    private:

};

void readingSensor();
int readingJson(std::string path);

