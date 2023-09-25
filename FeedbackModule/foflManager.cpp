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
    if(val <= 0){
        return;
    }
    switch (type)
    {
    case CPUtemp: 
    std::cout<<int(CPUtemp);
        double *Cputhres = returnPolicy(int(CPUtemp));
        if(Cputhres == nullptr){
            std::cout<<"null 포인터\n";
        }
        if(val <= Cputhres[0]){
            foflManager.cpuTempFOFL.current = Green;
                    std::cout<<"getPolicy1\n";
            if(foflManager.cpuTempFOFL.ongoing == true){
                endFeedback(type);
                        std::cout<<"getPolicy2\n";
            }
            foflManager.cpuTempFOFL.ongoing = false;
            std::cout<<"Green\n";
        }
        else if(val <= Cputhres[1]){
            foflManager.cpuTempFOFL.current = Yellow;
            if(foflManager.cpuTempFOFL.ongoing == true){
                timeCalculator(type, Yellow, true, val);
            }
            else{
                timeCalculator(type, Yellow, false, val);
            }
        }
        else if (val <= Cputhres[2]){
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
    case DIMMtemp:
    std::cout<<int(DIMMtemp)<<"\n";
        double *Dimmthres = returnPolicy(int(DIMMtemp));
        if(val <= Dimmthres[0]){
            foflManager.dimmTempFOFL.current = Green;
            if(foflManager.dimmTempFOFL.ongoing == true){
                endFeedback(type);
            }
            foflManager.dimmTempFOFL.ongoing = false;
            std::cout<<"Green\n";
        }
        else if(val <= Dimmthres[1]){
            foflManager.dimmTempFOFL.current = Yellow;
            if(foflManager.dimmTempFOFL.ongoing == true){
                timeCalculator(type, Yellow, true, val);
            }
            else{
                timeCalculator(type, Yellow, false, val);
            }
        }
        else if (val <= Dimmthres[2]){
            foflManager.dimmTempFOFL.current = Orange;
            if(foflManager.dimmTempFOFL.ongoing == true){
                timeCalculator(type, Orange, true, val);
            }
            else{
                timeCalculator(type, Orange, false, val);
            }
        }
        else{
            foflManager.dimmTempFOFL.current = Red;
            takeFanControl();
            timeCalculator(type,Red,true, val);
        }
        break;
    
    case Cabinettemp:
    std::cout<<int(Cabinettemp)<<"\n";
        double *CabinetThres = returnPolicy(int(Cabinettemp));
        if(val <= CabinetThres[0]){
            foflManager.cabinetTempFOFL.current = Green;
            if(foflManager.cabinetTempFOFL.ongoing == true){
                endFeedback(type);
            }
            foflManager.cabinetTempFOFL.ongoing = false;
            std::cout<<"Green\n";
        }
        else if(val <= CabinetThres[1]){
            foflManager.cabinetTempFOFL.current = Yellow;
            if(foflManager.cabinetTempFOFL.ongoing == true){
                timeCalculator(type, Yellow, true, val);
            }
            else{
                timeCalculator(type, Yellow, false, val);
            }
        }
        else if (val <= CabinetThres[2]){
            foflManager.cabinetTempFOFL.current = Orange;
            if(foflManager.cabinetTempFOFL.ongoing == true){
                timeCalculator(type, Orange, true, val);
            }
            else{
                timeCalculator(type, Orange, false, val);
            }
        }
        else{
            foflManager.cabinetTempFOFL.current = Red;
            takeFanControl();
            timeCalculator(type,Red,true, val);
        }
        break;    

    case LM75temp:
    std::cout<<int(LM75temp)<<"\n";
        double *LM75thres = returnPolicy(int(LM75temp));
        if(val <= LM75thres[0]){
            foflManager.lm75TempFOFL.current = Green;
            if(foflManager.lm75TempFOFL.ongoing == true){
                endFeedback(type);
            }
            foflManager.lm75TempFOFL.ongoing = false;
            std::cout<<"Green\n";
        }
        else if(val <= LM75thres[1]){
            foflManager.lm75TempFOFL.current = Yellow;
            if(foflManager.lm75TempFOFL.ongoing == true){
                timeCalculator(type, Yellow, true, val);
            }
            else{
                timeCalculator(type, Yellow, false, val);
            }
        }
        else if (val <= LM75thres[2]){
            foflManager.lm75TempFOFL.current = Orange;
            if(foflManager.lm75TempFOFL.ongoing == true){
                timeCalculator(type, Orange, true, val);
            }
            else{
                timeCalculator(type, Orange, false, val);
            }
        }
        else{
            foflManager.lm75TempFOFL.current = Red;
            takeFanControl();
            timeCalculator(type,Red,true, val);
        }
        break;        
    }
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
