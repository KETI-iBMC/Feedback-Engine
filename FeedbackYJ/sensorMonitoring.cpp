#include "sensorMonitoring.hpp"

#define REPEAT_TIME 30
#define REDFISH_SENSOR "/redfish/v1/Chassis/Sensors"
SensorMonitoring sensorMonitoring;

void readingSensor(){
    initFOFL();
    while(true){

        // sensorMonitoring -> sensor.CPU0_DIMM1_TEMP = readingJson("/CPU0_DIMM1_TEMP.json");
        // sensorMonitoring -> sensor.CPU0_DIMM2_TEMP = readingJson("/CPU0_DIMM2_TEMP.json");
        // sensorMonitoring->sensor.CPU0_DIMM3_TEMP = readingJson("/CPU0_DIMM3_TEMP.json");
        // sensorMonitoring->sensor.CPU0_DIMM4_TEMP = readingJson("/CPU0_DIMM4_TEMP.json");
        // sensorMonitoring->sensor.CPU0_DIMM5_TEMP = readingJson("/CPU0_DIMM5_TEMP.json");
        // sensorMonitoring->sensor.CPU0_DIMM6_TEMP = readingJson("/CPU0_DIMM6_TEMP.json");
        // sensorMonitoring->sensor.CPU0_DIMM7_TEMP = readingJson("/CPU0_DIMM7_TEMP.json");
        // sensorMonitoring->sensor.CPU0_DIMM8_TEMP = readingJson("/CPU0_DIMM8_TEMP.json");
        //sensorMonitoring -> sensor.CPU0_TEMP = readingJson("/CPU0_TEMP.json");
        sensorMonitoring.sensor.CPU1_TEMP = readingJson("/CPU1_TEMP.json");
        std::cout<<"Reading CPU1"<<"\n";
        sensorMonitoring.sensor.CabinetTemp = readingJson("/CabinetTemp.json");
        // sensorMonitoring->sensor.LM75_TEMP0 = readingJson("/LM75_TEMP0.json");
        // sensorMonitoring->sensor.LM75_TEMP1 = readingJson("/LM75_TEMP1.json");
        // sensorMonitoring->sensor.LM75_TEMP2 = readingJson("/LM75_TEMP2.json");
        // sensorMonitoring->sensor.LM75_TEMP3 = readingJson("/LM75_TEMP3.json");
        // sensorMonitoring->sensor.LM75_TEMP4 = readingJson("/LM75_TEMP4.json");

        // sensorMonitoring->sensor.P12V_PSU1 = readingJson("/P12V_PSU1.json");
        // sensorMonitoring->sensor.P12V_PSU2 = readingJson("/P12V_PSU2.json");
        // sensorMonitoring->sensor.P1V05_NAC = readingJson("/P1V05_NAC.json");
        // sensorMonitoring->sensor.P1V05_PCH = readingJson("/P1V05_PCH.json");
        // sensorMonitoring->sensor.P1V2_VDDQ = readingJson("/P1V2_VDDQ.json");
        // sensorMonitoring->sensor.P1V8 = readingJson("/P1V8.json");
        // sensorMonitoring->sensor.P1V8_NACDELAY = readingJson("/P1V8_NACDELAY.json");
        // sensorMonitoring->sensor.P3V3 = readingJson("/P3V3.json");
        // sensorMonitoring->sensor.P5V = readingJson("/P5V.json");  
        // sensorMonitoring->sensor.PVCCIN = readingJson("/PVCCIN.json");   
        // sensorMonitoring->sensor.PVNN_NAC = readingJson("/PVNN_NAC.json");   
        // sensorMonitoring->sensor.PVNN_PCH = readingJson("/PVNN_PCH.json");   
        // sensorMonitoring->sensor.PVNN_PCH_CPU0 = readingJson("/PVNN_PCH_CPU0.json");   
        // sensorMonitoring->sensor.PVPP = readingJson("/PVPP.json");   
        // sensorMonitoring->sensor.PVTT = readingJson("/PVTT.json");        
        //changeState(CPUtemp, sensorMonitoring->sensor.CPU0_TEMP);
        changeState(CPUtemp, sensorMonitoring.sensor.CPU1_TEMP);       
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

double readingJson(std::string path){
    std::string filePath = std::string(REDFISH_SENSOR) + path;
    double reading;
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
            reading = double(document["Reading"].GetInt());
            std::cout << "Reading: " << reading << std::endl;
        } else {
            std::cout << "Reading 값이 존재하지 않거나 부적절한 형식입니다." << std::endl;
        }
    } else {
        std::cout << "JSON 파싱 오류" << std::endl;
    }
    std::cout<<"Reading Over\n";
    file.close(); // 파일 닫기
    std::cout<<"File Closed\n";
    return reading;
}