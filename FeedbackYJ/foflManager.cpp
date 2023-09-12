#include "foflManager.hpp"
#include <iostream>

FOFLManager foflManager;

void initFOFL(){
    //폴리시 얻어와서 초기화 필요
    foflManager.cpuTempFOFL.ongoing = false;
    foflManager.cpuTempFOFL.current = Green;
    foflManager.dimmTempFOFL.current = Green;
    return;
}

void changeState(FeedbackType type, double val){
    switch (type)
    {
    case CPUtemp: 
    std::cout<<int(CPUtemp);
        double *thres = returnPolicy(int(CPUtemp));
        if(thres == nullptr){
            std::cout<<"null 포인터\n";
        }

        std::cout<<thres[0];
        if(val <= thres[0]){
            foflManager.cpuTempFOFL.current = Green;
                    std::cout<<"getPolicy1\n";
            if(foflManager.cpuTempFOFL.ongoing == true){
                endFeedback(type);
                        std::cout<<"getPolicy2\n";
            }
            foflManager.cpuTempFOFL.ongoing = false;
            std::cout<<"Green\n";
        }
        else if(val <= thres[1]){
            foflManager.cpuTempFOFL.current = Yellow;
            if(foflManager.cpuTempFOFL.ongoing == true){
                timeCalculator(type, Yellow, true, val);
            }
            else{
                timeCalculator(type, Yellow, false, val);
            }
        }
        else if (val <= thres[2]){
            foflManager.cpuTempFOFL.current = Orange;
            if(foflManager.cpuTempFOFL.ongoing == true){
                timeCalculator(type, Orange, true, val);
            }
            else{
                timeCalculator(type, Orange, false, val);
            }
        }
        else{
            foflManager.cpuTempFOFL.current = Red;
            takeFanControl();
            timeCalculator(type,Red,true, val);
        }
        break;
    }
    std::cout<<"Current State CPU : "<<foflManager.cpuTempFOFL.current<<"\n";
}

void getCurrentFOFL(){

    return;
}

void endFeedback(FeedbackType type){
    std::cout<<"return Fan Control to IBMC\n";
}

void timeCalculator(FeedbackType type, FOFLState state, bool ongoing, double val){
    if(ongoing == false){
        //온도 상승에 걸리는 시간;
        //학습 데이터
    }

    if(ongoing == true){
        std::cout<<"It will take 180 seconds to reach Green Temperature\n";
        //온도 하락에 걸리는 시간;
    }
}

void takeFanControl(){
    std::cout<<"get Fan Control from IBMC\n";
}
