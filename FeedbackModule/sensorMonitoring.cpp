#include "sensorMonitoring.hpp"

#define REPEAT_TIME 30
#define REDFISH_SENSOR "/redfish/v1/Chassis/Sensors"
SensorMonitoring sensorMonitoring;

void readingSensor(){
    initFOFL();
    while(true){

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

        changeState(CPUtemp, sensorMonitoring.sensor.CPU0_TEMP);
        changeState(CPUtemp, sensorMonitoring.sensor.CPU1_TEMP);
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM1_TEMP);    
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM2_TEMP);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM3_TEMP);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM4_TEMP);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM5_TEMP);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM6_TEMP);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM7_TEMP);   
        changeState(DIMMtemp, sensorMonitoring.sensor.CPU0_DIMM8_TEMP);   
        changeState(Cabinettemp, sensorMonitoring.sensor.CabinetTemp);
        changeState(LM75temp,sensorMonitoring.sensor.LM75_TEMP0);
        changeState(LM75temp,sensorMonitoring.sensor.LM75_TEMP1); 
        changeState(LM75temp,sensorMonitoring.sensor.LM75_TEMP2); 
        changeState(LM75temp,sensorMonitoring.sensor.LM75_TEMP3); 
        changeState(LM75temp,sensorMonitoring.sensor.LM75_TEMP4);          
           
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

int readingJson(std::string path){
    std::string filePath = std::string(REDFISH_SENSOR) + path;
    int reading;
    std::ifstream file(filePath);
    //std::cout<<filePath<<"\n";
    //파일이 존재하는지 체크해라!!!
    if(!file.is_open()){
        std::cerr << "~~~~~~~~CANNOT OPEN THE FILE~~~~~~~\n";
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
            std::cout << "Reading 값이 존재하지 않거나 부적절한 형식입니다." << std::endl;
            reading = 0;
        }
    } 
    else {
        std::cout << "JSON 파싱 오류" << std::endl;
        reading = 0;
    }
    std::cout<<"Reading Over\n";
    file.close(); // 파일 닫기
    std::cout<<"File Closed\n";
    return reading;
}