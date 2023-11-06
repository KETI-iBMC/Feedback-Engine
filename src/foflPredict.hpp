
#include <thread>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
class FOFLPredict{
    public:
        double curTemp;
        double pastTemp;
        FOFLPredict(){};
    private:
};

void storePredictData();
int predictTemp(int curTemp,int targetTemp, bool on);
void initPredictFOFL();
void storeSensorData(int sensor);