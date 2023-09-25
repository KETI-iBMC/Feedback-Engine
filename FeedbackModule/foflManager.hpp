#include "policyController.hpp"

enum FOFLState{
    Red = 0,
    Orange,
    Yellow,
    Green,
};
enum FeedbackType{
    CPUtemp = 0,
    DIMMtemp,
    Cabinettemp,
    LM75temp,
};

struct CPUTempFOFL{
    FOFLState current;
    FeedbackType type;
    bool ongoing;
};

struct DIMMTempFOFL
{
    FOFLState current;
    FeedbackType type;
    bool ongoing;
};

struct CabinetTempFOFL
{
    FOFLState current;
    FeedbackType type;
    bool ongoing;
}

struct LM75TempFOFL
{
    FOFLState current;
    FeedbackType type;
    bool ongoing;
}


class FOFLManager {
    public:
    CPUTempFOFL cpuTempFOFL;
    DIMMTempFOFL dimmTempFOFL;
    CabinetTempFOFL cabinetTempFOFL;
    LM75TempFOFL lm75TempFOFL;

    FOFLManager(){};
    private:
};

class PredictModel {
    public:


    PredictModel(){}
    private:

};

void initFOFL();
void changeState(FeedbackType type, double val); // 값을 통해 state 변화
void redToGreenTime(FeedbackType type); // red to green 단계 시간, ibmc로 전송 예정
void takeFanControl();
void endFeedback(FeedbackType type); // red단계 피드백 진행 후 green 도달시 피드백 종료, ibmc로 종료 전송
void timeCalculator(FeedbackType type, FOFLState state, bool ongoing, double val); // 상승 예정 시간 알람, ibmc로 전송 예정
FOFLState getCurrentState(FeedbackType type, double val);
void getCurrentFOFL();