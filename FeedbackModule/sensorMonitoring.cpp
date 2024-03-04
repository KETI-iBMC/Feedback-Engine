#include "sensorMonitoring.hpp"
#include "foflPredict.hpp"
#include <iostream>

#define REPEAT_TIME 30
#define REDFISH_SENSOR "/redfish/v1/Chassis/Sensors"
SensorMonitoring sensorMonitoring;
int predictCount = 0;
void readingSensor(){
    initFOFL();
    bool kernelPanic = false;
    while(true){
        std::cout<<"\n";
        sensorMonitoring.sensor.CPU0_DIMM1_TEMP = readingJson("/CPU0_DIMM1_TEMP.json");
        sensorMonitoring.sensor.CPU0_DIMM2_TEMP = readingJson("/CPU0_DIMM2_TEMP.json");
        sensorMonitoring.sensor.CPU0_DIMM3_TEMP = readingJson("/CPU0_DIMM3_TEMP.json");
        sensorMonitoring.sensor.CPU0_DIMM4_TEMP = readingJson("/CPU0_DIMM4_TEMP.json");
        sensorMonitoring.sensor.CPU0_DIMM5_TEMP = readingJson("/CPU0_DIMM5_TEMP.json");
        sensorMonitoring.sensor.CPU0_DIMM6_TEMP = readingJson("/CPU0_DIMM6_TEMP.json");
        sensorMonitoring.sensor.CPU0_DIMM7_TEMP = readingJson("/CPU0_DIMM7_TEMP.json");
        sensorMonitoring.sensor.CPU0_DIMM8_TEMP = readingJson("/CPU0_DIMM8_TEMP.json");
        sensorMonitoring.sensor.CPU0_TEMP = readingJson("/CPU0_TEMP.json");
        sensorMonitoring.sensor.CPU1_TEMP = readingJson("/CPU1_TEMP.json");
        sensorMonitoring.sensor.CabinetTemp = readingJson("/CabinetTemp.json");
        sensorMonitoring.sensor.LM75_TEMP0 = readingJson("/LM75_TEMP0.json");
        sensorMonitoring.sensor.LM75_TEMP1 = readingJson("/LM75_TEMP1.json");
        sensorMonitoring.sensor.LM75_TEMP2 = readingJson("/LM75_TEMP2.json");
        sensorMonitoring.sensor.LM75_TEMP3 = readingJson("/LM75_TEMP3.json");
        sensorMonitoring.sensor.LM75_TEMP4 = readingJson("/LM75_TEMP4.json");

        changeState(CPUtemp, sensorMonitoring.sensor.CPU0_TEMP, 0);
        changeState(CPUtemp, sensorMonitoring.sensor.CPU1_TEMP, 1);
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM1_TEMP, 0);    
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM2_TEMP, 1);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM3_TEMP, 2);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM4_TEMP, 3);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM5_TEMP,4 );   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM6_TEMP,5);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM7_TEMP,6);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM8_TEMP,7);   
        changeState(Cabinettemp, sensorMonitoring.sensor.CabinetTemp,0);
        //changeState(diskTemp,sensorMonitoring.sensor.LM75_TEMP0,0);
        //changeState(diskTemp,sensorMonitoring.sensor.LM75_TEMP1,1); 
        //changeState(diskTemp,sensorMonitoring.sensor.LM75_TEMP2,2); 
        //changeState(diskTemp,sensorMonitoring.sensor.LM75_TEMP3,3); 
        //changeState(diskTemp,sensorMonitoring.sensor.LM75_TEMP4,4);
        if(predictCount == 1){       
            storeSensorData(sensorMonitoring.sensor.CPU1_TEMP);
            predictCount = 0;
        }
        else{
            predictCount++;
        }
        std::this_thread::sleep_for(std::chrono::seconds(60000));
    }
}

int readingJson(std::string path){
    std::string filePath = std::string(REDFISH_SENSOR) + path;
    int reading;
    std::ifstream file(filePath);
    //std::cout<<filePath<<"\n";
    //파일이 존재하는지 체크해라!!!
    if(!file.is_open()){
        std::cerr << "CANNOT OPEN FILE "<<path<<"\n";
        //파일 못연다! 오류 처리 필요
        return 0;
    }
    std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    rapidjson::Document document;
    document.Parse(json.c_str());
    
    if (!document.HasParseError()) {
        if (document.HasMember("Reading") && document["Reading"].IsInt()) {
            reading = document["Reading"].GetInt();
            std::cout<<path << " Reading: " << reading << std::endl;
        } else {
            std::cout << "READING VALUE IS NOT CORRECT" << std::endl;
            reading = 0;
        }
    } 
    else {
        std::cout << "JSON PARSING ERROR" << std::endl;
        reading = 0;
    }
    file.close(); // 파일 닫기
    return reading;
}