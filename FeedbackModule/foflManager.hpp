#pragma once
#include <iostream>
#include "dbManager.hpp"
#include <chrono>
#include <ctime>
enum FOFLState{
    Red = 3,
    Orange= 2,
    Yellow= 1,
    Green =0 ,
};
enum FeedbackType{
    CPUtemp = 0,
    DIMMtemp =1 ,
    Cabinettemp = 2,
    diskTemp = 3,
    chassisPanic =4 ,
};

struct CPUTempFOFL{
    FOFLState current;
    FeedbackType type;
    bool ongoing;
    int greenCount = 0;
    int yellowCount = 0;
    int orangeCount = 0;
    int redCount = 0;
};

struct DIMMTempFOFL
{
    FOFLState current;
    FeedbackType type;
    bool ongoing;
    int greenCount = 0;
    int yellowCount = 0;
    int orangeCount = 0;
    int redCount = 0;
};

struct CabinetTempFOFL
{
    FOFLState current;
    FeedbackType type;
    bool ongoing;
    int greenCount = 0;
    int yellowCount = 0;
    int orangeCount = 0;
    int redCount = 0;
};

struct diskTempFOFL
{
    FOFLState current;
    FeedbackType type;
    bool ongoing;
    int greenCount = 0;
    int yellowCount = 0;
    int orangeCount = 0;
    int redCount = 0;
};

struct chassisPaniceFOFL{
    int kernelPanicCount = 0;
    bool ongoing;
};


class FOFLManager {
    public:
    CPUTempFOFL cpuTempFOFL[4];
    DIMMTempFOFL dimmTempFOFL[8];
    CabinetTempFOFL cabinetTempFOFL[1];
    diskTempFOFL diskTempFOFL[4];
    chassisPaniceFOFL chassisFOFL[1];

    FOFLManager(){};
    private:
};

class PredictModel {
    public:


    PredictModel(){}
    private:

};

void initFOFL();
void changeState(FeedbackType type, int val, int num); // 값을 통해 state 변화
void redToGreenTime(FeedbackType type); // red to green 단계 시간, ibmc로 전송 예정
void takeFanControl();
void endFeedback(FeedbackType type); // red단계 피드백 진행 후 green 도달시 피드백 종료, ibmc로 종료 전송
void timeCalculator(FeedbackType type, FOFLState state, bool ongoing, int val); // 상승 예정 시간 알람, ibmc로 전송 예정
FOFLState getCurrentState(FeedbackType type, int val);
void getCurrentFOFL();
std::string getCurrentTimestamp();
void storeStateJson();
void temperatureFeedback();