#include "rapidjson/document.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include "foflManager.hpp"


struct Sensor{
    double CPU0_DIMM1_TEMP;
    double CPU0_DIMM2_TEMP;
    double CPU0_DIMM3_TEMP;
    double CPU0_DIMM4_TEMP;
    double CPU0_DIMM5_TEMP;
    double CPU0_DIMM6_TEMP;
    double CPU0_DIMM7_TEMP;
    double CPU0_DIMM8_TEMP;
    double CPU0_TEMP;
    double CPU1_TEMP;
    double LM75_TEMP0;
    double LM75_TEMP1;
    double LM75_TEMP2;
    double LM75_TEMP3;
    double LM75_TEMP4;
    double CabinetTemp;
    double P12V_PSU1;
    double P12V_PSU2;
    double P1V05_NAC;
    double P1V05_PCH;
    double P1V2_VDDQ;
    double P1V8;
    double P1V8_NACDELAY;
    double P3V3;
    double P5V;
    double PVCCIN;
    double PVNN_NAC;
    double PVNN_PCH;
    double PVNN_PCH_CPU0;
    double PVPP;
    double PVTT;
};

class SensorMonitoring{
    public:
    SensorMonitoring(){};
    Sensor sensor;
    private:

};

void readingSensor();
double readingJson(std::string path);

