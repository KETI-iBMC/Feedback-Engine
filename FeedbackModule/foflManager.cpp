#include "foflManager.hpp"
#include "dbManager.hpp"
#include "policyController.hpp"
#include <thread>
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "foflPredict.hpp"

FOFLManager foflManager;

void initFOFL(){
    //폴리시 얻어와서 초기화 필요
    foflManager.cpuTempFOFL -> ongoing = false;
    foflManager.cpuTempFOFL -> current = Green;
    foflManager.cpuTempFOFL -> greenCount = 30;
    foflManager.cpuTempFOFL -> yellowCount = 30;
    foflManager.cpuTempFOFL -> orangeCount = 30;
    foflManager.cpuTempFOFL -> redCount = 30;
    foflManager.dimmTempFOFL -> current = Green;
    foflManager.dimmTempFOFL -> ongoing = false;
    foflManager.dimmTempFOFL -> greenCount = 30;
    foflManager.dimmTempFOFL -> yellowCount = 30;
    foflManager.dimmTempFOFL -> orangeCount = 30;
    foflManager.dimmTempFOFL -> redCount = 30;
    foflManager.cabinetTempFOFL -> current = Green;
    foflManager.cabinetTempFOFL -> ongoing = false;
    foflManager.cabinetTempFOFL -> greenCount = 30;
    foflManager.cabinetTempFOFL -> yellowCount = 30;
    foflManager.cabinetTempFOFL -> orangeCount = 30;
    foflManager.cabinetTempFOFL -> redCount = 30;
    foflManager.diskTempFOFL -> current = Green;
    foflManager.diskTempFOFL -> ongoing = false;
    foflManager.diskTempFOFL -> greenCount = 30;
    foflManager.diskTempFOFL -> yellowCount = 30;
    foflManager.diskTempFOFL -> orangeCount = 30;
    foflManager.diskTempFOFL -> redCount = 30;
    // foflManager.chassisFOFL -> kernelPanicCount = 30;
    // foflManager.chassisFOFL -> ongoing = false;
}

void changeState(FeedbackType type, int val, int num){
    int calculateTime = 0;
    
    std::string timestamp = getCurrentTimestamp();
    std::cout<<"\nFOFL Manager Running\nTime : "<<timestamp<<" type : "<<type<<" value : "<<val<<" module num : "<<num<<std::endl;
    switch (type)
    {
    case CPUtemp: {
        int *Cputhres = returnPolicy(int(CPUtemp), num);
        bool *CpuActive = getActivation(int(CPUtemp), num);
        if(val <= 0){
            foflManager.cpuTempFOFL[num].current = Green;
        }
        else if(val < Cputhres[0]){
            if(CpuActive[0] == true){
                foflManager.cpuTempFOFL[num].greenCount += 1;
                if(foflManager.cpuTempFOFL[num].greenCount > 30){
                    DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 0, num, 7, 3);
                    foflManager.cpuTempFOFL[num].greenCount = 0;
                    foflManager.cpuTempFOFL[num].yellowCount = 30;
                    foflManager.cpuTempFOFL[num].orangeCount = 30;
                    foflManager.cpuTempFOFL[num].redCount = 30;
                }
            }
            foflManager.cpuTempFOFL[num].current = Green;
            if(foflManager.cpuTempFOFL[num].ongoing == true){
                DatabaseHandler::getInstance("/conf/feedback.db").updateProgress(0,num,1);
                endFeedback(type);
            }
            calculateTime = timeCalculator(type, Green, false, val, Cputhres[0]);
            foflManager.cpuTempFOFL[num].ongoing = false;
        }
        else if(val < Cputhres[1]){
            if(CpuActive[1] == true){
                foflManager.cpuTempFOFL[num].yellowCount += 1;
                if(foflManager.cpuTempFOFL[num].yellowCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 0, num, 30, 3);
                    foflManager.cpuTempFOFL[num].greenCount = 30;
                    foflManager.cpuTempFOFL[num].yellowCount = 0;
                    foflManager.cpuTempFOFL[num].orangeCount = 30;
                    foflManager.cpuTempFOFL[num].redCount = 30;    
                }
            }
            foflManager.cpuTempFOFL[num].current = Yellow;
            if(foflManager.cpuTempFOFL[num].ongoing == true){
                calculateTime = timeCalculator(type, Yellow, true, val, Cputhres[0]);
            }
            else{
                calculateTime = timeCalculator(type, Yellow, false, val, Cputhres[1]);
            }
        }
        else if (val < Cputhres[2]){
            if(CpuActive[2] == true){
                foflManager.cpuTempFOFL[num].orangeCount += 1;
                if(foflManager.cpuTempFOFL[num].orangeCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 0, num, 5, 3);
                    foflManager.cpuTempFOFL[num].greenCount = 30;
                    foflManager.cpuTempFOFL[num].yellowCount = 30;
                    foflManager.cpuTempFOFL[num].orangeCount = 0;
                    foflManager.cpuTempFOFL[num].redCount = 30;   
                }
            }
            foflManager.cpuTempFOFL[num].current = Orange;
            if(foflManager.cpuTempFOFL[num].ongoing == true){
                calculateTime = timeCalculator(type, Orange, true, val, Cputhres[0]);
            }
            else{
                calculateTime = timeCalculator(type, Orange, false, val, Cputhres[2]);
            }
        }
        else{
            if(CpuActive[3] == true){
                foflManager.cpuTempFOFL[num].redCount += 1;
                if(foflManager.cpuTempFOFL[num].redCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 0, num, 1, 3);
                    foflManager.cpuTempFOFL[num].greenCount = 30;
                    foflManager.cpuTempFOFL[num].yellowCount = 30;
                    foflManager.cpuTempFOFL[num].orangeCount = 30;
                    foflManager.cpuTempFOFL[num].redCount = 0;   
                }
            }
            if(foflManager.cpuTempFOFL[num].ongoing == true){
                std::cout<<"\nFEEDBACK PROCESS CONTINUED..."<<"\n";
                break;
            }
            else{
            foflManager.cpuTempFOFL[num].ongoing = true;
            foflManager.cpuTempFOFL[num].current = Red;
            DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 0, num, 1, 0);
            takeFanControl();
            calculateTime = timeCalculator(type,Red,true, val, Cputhres[0]);
            }
        }
        updateState(0,num,int(foflManager.cpuTempFOFL[num].current),foflManager.cpuTempFOFL[num].ongoing, calculateTime);
        break;
    }
    case DIMMtemp:{
        int *Dimmthres = returnPolicy(int(DIMMtemp), num);
        bool *DimmActive = getActivation(int(DIMMtemp),num);
        if(val <= 0){
            foflManager.dimmTempFOFL[num].current = Green;
        }
        else if(val < Dimmthres[0]){
            if(DimmActive[0] == true){
                foflManager.dimmTempFOFL[num].greenCount += 1;
                if(foflManager.dimmTempFOFL[num].greenCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 1, num, 7, 3);
                    foflManager.dimmTempFOFL[num].greenCount = 0;
                    foflManager.dimmTempFOFL[num].yellowCount = 30;
                    foflManager.dimmTempFOFL[num].orangeCount = 30;
                    foflManager.dimmTempFOFL[num].redCount = 30;
                }
            }
            foflManager.dimmTempFOFL[num].current = Green;
            if(foflManager.dimmTempFOFL[num].ongoing == true){
                DatabaseHandler::getInstance("/conf/feedback.db").updateProgress(1,num,1);
                endFeedback(type);
                
            }
            calculateTime = timeCalculator(type, Green, false, val, Dimmthres[0]);
            foflManager.dimmTempFOFL[num].ongoing = false;
        }
        else if(val < Dimmthres[1]){
            if(DimmActive[1] == true){
                foflManager.dimmTempFOFL[num].yellowCount += 1;
                if(foflManager.dimmTempFOFL[num].yellowCount> 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 1, num, 30, 3);
                    foflManager.dimmTempFOFL[num].greenCount = 30;
                    foflManager.dimmTempFOFL[num].yellowCount = 0;
                    foflManager.dimmTempFOFL[num].orangeCount = 30;
                    foflManager.dimmTempFOFL[num].redCount = 30;
                }
            }
            foflManager.dimmTempFOFL[num].current = Yellow;
            if(foflManager.dimmTempFOFL[num].ongoing == true){
                calculateTime = timeCalculator(type, Yellow, true, val, Dimmthres[0]);
            }
            else{
                calculateTime = timeCalculator(type, Yellow, false, val, Dimmthres[1]);
            }
        }
        else if (val < Dimmthres[2]){
            if(DimmActive[2] == true){
                foflManager.dimmTempFOFL[num].orangeCount += 1;
                if(foflManager.dimmTempFOFL[num].orangeCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 1, num, 5, 3);
                    foflManager.dimmTempFOFL[num].greenCount = 30;
                    foflManager.dimmTempFOFL[num].yellowCount = 30;
                    foflManager.dimmTempFOFL[num].orangeCount = 0;
                    foflManager.dimmTempFOFL[num].redCount = 30;
                }
            }
            foflManager.dimmTempFOFL[num].current = Orange;
            if(foflManager.dimmTempFOFL[num].ongoing == true){
                calculateTime = timeCalculator(type, Orange, true, val, Dimmthres[0]);
            }
            else{
                calculateTime = timeCalculator(type, Orange, false, val, Dimmthres[2]);
            }
        }
        else{
            if(DimmActive[3] == true){
                foflManager.dimmTempFOFL[num].redCount += 1;
                if(foflManager.dimmTempFOFL[num].redCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 1, num, 1, 3);
                    foflManager.dimmTempFOFL[num].greenCount = 30;
                    foflManager.dimmTempFOFL[num].yellowCount = 30;
                    foflManager.dimmTempFOFL[num].orangeCount = 30;
                    foflManager.dimmTempFOFL[num].redCount = 0;
                }
            }
            if(foflManager.dimmTempFOFL[num].ongoing == true){
                break;
            }
            else{
            foflManager.dimmTempFOFL[num].ongoing = true;
            foflManager.dimmTempFOFL[num].current = Red;
            std::string timestamp = getCurrentTimestamp();
            DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 1, num, 1, 0);
            takeFanControl();
            calculateTime = timeCalculator(type,Red,true, val, Dimmthres[0]);
            }
        }
    
        updateState(1,num,int(foflManager.dimmTempFOFL[num].current),foflManager.dimmTempFOFL[num].ongoing, calculateTime);
        break;
    }
    case Cabinettemp:{
        int *CabinetThres = returnPolicy(int(Cabinettemp), num);
        bool *CabinetActive = getActivation(int(Cabinettemp),num);
        if(val <= 0){
            foflManager.cabinetTempFOFL[num].current = Green;
        }
        else if(val < CabinetThres[0]){
            if(CabinetActive[0] == true){
                foflManager.cabinetTempFOFL[num].greenCount += 1;
                if(foflManager.cabinetTempFOFL[num].greenCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp,4,num,7,3);
                    foflManager.cabinetTempFOFL[num].greenCount = 0;
                    foflManager.cabinetTempFOFL[num].yellowCount = 30;
                    foflManager.cabinetTempFOFL[num].orangeCount = 30;
                    foflManager.cabinetTempFOFL[num].redCount = 30;
                }
            }
            foflManager.cabinetTempFOFL[num].current = Green;
            if(foflManager.cabinetTempFOFL[num].ongoing == true){
                DatabaseHandler::getInstance("/conf/feedback.db").updateProgress(4,num,1);
                endFeedback(type);
            }
            calculateTime = timeCalculator(type, Green, false, val, CabinetThres[0]);
            foflManager.cabinetTempFOFL[num].ongoing = false;
        }
        else if(val < CabinetThres[1]){
            if(CabinetActive[1] == true){
                foflManager.cabinetTempFOFL[num].yellowCount += 1;
                if(foflManager.cabinetTempFOFL[num].yellowCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp,4,num,30,3);
                    foflManager.cabinetTempFOFL[num].greenCount = 30;
                    foflManager.cabinetTempFOFL[num].yellowCount = 0;
                    foflManager.cabinetTempFOFL[num].orangeCount = 30;
                    foflManager.cabinetTempFOFL[num].redCount = 30;
                }
            }
            foflManager.cabinetTempFOFL[num].current = Yellow;
            if(foflManager.cabinetTempFOFL[num].ongoing == true){
                calculateTime = timeCalculator(type, Yellow, true, val, CabinetThres[0]);
            }
            else{
                calculateTime =timeCalculator(type, Yellow, false, val, CabinetThres[1]);
            }
        }
        else if (val < CabinetThres[2]){
            if(CabinetActive[2] == true){
                foflManager.cabinetTempFOFL[num].orangeCount += 1;
                if(foflManager.cabinetTempFOFL[num].orangeCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp,4,num,5,3);
                    foflManager.cabinetTempFOFL[num].greenCount = 30;
                    foflManager.cabinetTempFOFL[num].yellowCount = 30;
                    foflManager.cabinetTempFOFL[num].orangeCount = 0;
                    foflManager.cabinetTempFOFL[num].redCount = 30;
                }
            }
            foflManager.cabinetTempFOFL[num].current = Orange;
            if(foflManager.cabinetTempFOFL[num].ongoing == true){
                calculateTime =timeCalculator(type, Orange, true, val, CabinetThres[0]);
            }
            else{
                calculateTime =timeCalculator(type, Orange, false, val, CabinetThres[2]);
            }
        }
        else{
            if(CabinetActive[3] == true){
                foflManager.cabinetTempFOFL[num].redCount += 1;
                if(foflManager.cabinetTempFOFL[num].redCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp,4,num,1,3);
                    foflManager.cabinetTempFOFL[num].greenCount = 30;
                    foflManager.cabinetTempFOFL[num].yellowCount = 30;
                    foflManager.cabinetTempFOFL[num].orangeCount = 30;
                    foflManager.cabinetTempFOFL[num].redCount = 0;
                }
            }
            if(foflManager.cabinetTempFOFL[num].ongoing == true){
                break;
            }
            else{
            foflManager.cabinetTempFOFL[num].ongoing = true;
            foflManager.cabinetTempFOFL[num].current = Red;
            std::string timestamp = getCurrentTimestamp();
            DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 4, num, 1, 0);
            takeFanControl();
            calculateTime = timeCalculator(type,Red,true, val, CabinetThres[0]);
            }
        }
        updateState(4,num,int(foflManager.cabinetTempFOFL[num].current),foflManager.cabinetTempFOFL[num].ongoing, calculateTime);
        break;    
    }

    //disk temp
    case diskTemp:{
        int *DiskThres = returnPolicy(int(diskTemp), num);
        bool *DiskActive = getActivation(int(diskTemp),num);
        if(val <= 0){
            foflManager.diskTempFOFL[num].current = Green;
        }
        else if(val < DiskThres[0]){
            if(DiskActive[0] == true){
                foflManager.diskTempFOFL[num].greenCount += 1;
                if(foflManager.diskTempFOFL[num].greenCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 2, num, 7, 3);
                    foflManager.diskTempFOFL[num].greenCount = 0;
                    foflManager.diskTempFOFL[num].yellowCount = 30;
                    foflManager.diskTempFOFL[num].orangeCount = 30;
                    foflManager.diskTempFOFL[num].redCount = 30;
                }
            }
            foflManager.diskTempFOFL[num].current = Green;
            if(foflManager.diskTempFOFL[num].ongoing == true){
                DatabaseHandler::getInstance("/conf/feedback.db").updateProgress(2,num,1);
                endFeedback(type);
            }
            calculateTime = timeCalculator(type, Green, false, val, DiskThres[0]);
            foflManager.diskTempFOFL[num].ongoing = false;
        }
        else if(val < DiskThres[1]){
            if(DiskActive[1] == true){
                foflManager.diskTempFOFL[num].yellowCount += 1;
                if(foflManager.diskTempFOFL[num].yellowCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 2, num, 30, 3);
                    foflManager.diskTempFOFL[num].greenCount = 30;
                    foflManager.diskTempFOFL[num].yellowCount = 0;
                    foflManager.diskTempFOFL[num].orangeCount = 30;
                    foflManager.diskTempFOFL[num].redCount = 30;
                }
            }
            foflManager.diskTempFOFL[num].current = Yellow;
            if(foflManager.diskTempFOFL[num].ongoing == true){
                calculateTime =timeCalculator(type, Yellow, true, val, DiskThres[0]);
            }
            else{
                calculateTime =timeCalculator(type, Yellow, false, val, DiskThres[1]);
            }
        }
        else if (val < DiskThres[2]){
            if(DiskActive[2] == true){
                foflManager.diskTempFOFL[num].orangeCount += 1;
                if(foflManager.diskTempFOFL[num].orangeCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 2, num, 5, 3);
                    foflManager.diskTempFOFL[num].greenCount = 30;
                    foflManager.diskTempFOFL[num].yellowCount = 30;
                    foflManager.diskTempFOFL[num].orangeCount = 0;
                    foflManager.diskTempFOFL[num].redCount = 30;
                }
            }
            foflManager.diskTempFOFL[num].current = Orange;
            if(foflManager.diskTempFOFL[num].ongoing == true){
                calculateTime =timeCalculator(type, Orange, true, val, DiskThres[0]);
            }
            else{
                calculateTime =timeCalculator(type, Orange, false, val, DiskThres[2]);
            }
        }
        else{
            if(DiskActive[3] == true){
                foflManager.diskTempFOFL[num].redCount += 1;
                if(foflManager.diskTempFOFL[num].redCount > 30){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 2, num, 1, 3);
                    foflManager.diskTempFOFL[num].greenCount = 30;
                    foflManager.diskTempFOFL[num].yellowCount = 30;
                    foflManager.diskTempFOFL[num].orangeCount = 30;
                    foflManager.diskTempFOFL[num].redCount = 0;
                }
            }
            if(foflManager.diskTempFOFL[num].ongoing == true){
                break;
            }
            else{
            foflManager.diskTempFOFL[num].ongoing = true;
            foflManager.diskTempFOFL[num].current = Red;
            std::string timestamp = getCurrentTimestamp();
            DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp, 2, num, 1, 0);
            takeFanControl();
            calculateTime =timeCalculator(type,Red,true, val, DiskThres[0]);
            }
        }
        updateState(2,num,int(foflManager.diskTempFOFL[num].current),foflManager.diskTempFOFL[num].ongoing, calculateTime);
        break;        
    }
    }
}


void getCurrentFOFL(){

    return;
}

void endFeedback(FeedbackType type){
    foflManager.ongoingCount -= 1;
    if(foflManager.ongoingCount > 0){
        return;}
    const char* filename = "/conf/tempfeedback.json";
    std::ifstream inputFile(filename);
    bool fileExists = inputFile.is_open();
    rapidjson::Document doc;
    if(fileExists) {
        std::string jsonData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        // JSON 파싱
        doc.Parse(jsonData.c_str());

        if(doc.HasMember("STATUS")){
            doc["STATUS"] = 1;
        } 
    }

    else{
        doc.SetObject();
        doc.AddMember("FEEDBACK_TYPE","TEMPERATURE",doc.GetAllocator());
        doc.AddMember("STATUS",1,doc.GetAllocator());
    }

    std::ofstream outputFile(filename);
    if(!outputFile){
        std::cerr <<"FAILED TO CONTROL FAN"<<std::endl;
        return;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    outputFile<<buffer.GetString();
    outputFile.close();

    std::cout<<"\nSUCCESS TO RETURN FAN CONTROL\n";
}

int timeCalculator(FeedbackType type, FOFLState state, bool ongoing, int val, int targetVal){
    int calculateTime = 0;
    if(ongoing == false){
        calculateTime = predictTemp(val, targetVal, ongoing);
        //온도 상승에 걸리는 시간;
        //학습 데이터
    }

    if(ongoing == true){
        calculateTime = predictTemp(val, targetVal, ongoing);
        std::cout<<"\nFEEDBACK START\n";
        //온도 하락에 걸리는 시간;
    }
    return calculateTime;
}

void takeFanControl(){
    foflManager.ongoingCount += 1;
    if(foflManager.ongoingCount > 1){
        return;}
    const char* filename = "/conf/tempfeedback.json";
    std::ifstream inputFile(filename);
    bool fileExists = inputFile.is_open();
    rapidjson::Document doc;
    if(fileExists) {
        std::string jsonData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        // JSON 파싱
        doc.Parse(jsonData.c_str());

        if(doc.HasMember("STATUS")){
            doc["STATUS"] = 1;
        } 
    }

    else{
        doc.SetObject();
        doc.AddMember("FEEDBACK_TYPE","TEMPERATURE",doc.GetAllocator());
        doc.AddMember("STATUS",1,doc.GetAllocator());
    }

    std::ofstream outputFile(filename);
    if(!outputFile){
        std::cerr <<"FAILED TO CONTROL FAN"<<std::endl;
        return;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    outputFile<<buffer.GetString();
    outputFile.close();

    std::cout<<"\nSUCCESS TO GET FAN CONTROL\n";
}

std::string getCurrentTimestamp(){
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    char timestamp[30];  // 충분한 공간을 확보합니다.

    // 형식 지정을 통해 timestamp 문자열 생성
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    return std::string(timestamp);
}

void temperatureFeedback(){

}