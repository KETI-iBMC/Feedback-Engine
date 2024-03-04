#include "foflPredict.hpp"


FOFLPredict foflPredcit;
void initPredictFOFL(){
    foflPredcit.curTemp = 0;
    foflPredcit.pastTemp = 0;
    while(1){
        //storePredictData();
        std::this_thread::sleep_for(std::chrono::seconds(60000));
    }
}

int predictTemp(int curTemp,int targetTemp, bool ongoing){
    double minChange = 0;
    int calculateTime = 0;
    minChange = foflPredcit.curTemp - foflPredcit.pastTemp;
    if(ongoing == false){ //피드백 상황이 아닌경우
        if(minChange < 1){
            return 0; // 안정적
        }
        else{
            calculateTime = static_cast<int>((targetTemp-curTemp)/(minChange / 10));
        }
    }
    else{ // 피드백 수행시 온도 하락의 경우
        if(minChange >= 0){
            return 0; // 계산 수행중
        }
        else{
            minChange = foflPredcit.pastTemp - foflPredcit.curTemp;
            calculateTime = static_cast<int>(curTemp-targetTemp) /(minChange / 10);
        }
    }
    return calculateTime;
}

void storePredictData(){
    double nextCPUTemp = 0;
    std::ifstream jsonFile("/conf/lfc.json");
        if (!jsonFile.is_open()) {
        std::cerr << "Failed to open JSON file." << std::endl;
        return;
    }

    rapidjson::IStreamWrapper jsonStream(jsonFile);
    rapidjson::Document document;
    document.ParseStream(jsonStream);

        if (document.HasParseError()) {
        std::cerr << "JSON parse error: " << document.GetParseError() << std::endl;
        return;
    }

        // "NextCPUTmp" 필드 값 가져오기
    if (document.HasMember("NextCPUTmp") && document["NextCPUTmp"].IsDouble()) {
        nextCPUTemp = document["NextCPUTmp"].GetDouble();
        std::cout << "NextCPUTmp: " << nextCPUTemp << std::endl;
    } else {
        std::cerr << "NextCPUTmp field not found or has an invalid type." << std::endl;
        return;
    }

    nextCPUTemp = std::floor(nextCPUTemp * 100) / 100;

    foflPredcit.pastTemp = foflPredcit.curTemp;
    foflPredcit.curTemp = nextCPUTemp;

}

void storeSensorData(int sensor){
    foflPredcit.pastTemp  = foflPredcit.curTemp;
    foflPredcit.curTemp = sensor;

    return;
}