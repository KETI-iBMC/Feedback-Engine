#include "policyController.hpp"
#include "dbManager.hpp"
#include "rapidjson/document.h"
#include "feedbackDbus.hpp"
#include <ostream>

PolicyController policyController;
void initPolicy(){
    policyController.cpuTempPolicy->activation = false;
    policyController.cpuTempPolicy->greenActive = false;
    policyController.cpuTempPolicy->yellowActive = false;
    policyController.cpuTempPolicy->orangeActive = false;
    policyController.cpuTempPolicy->redActive = false;
    policyController.cpuTempPolicy->yellowThres = 65;
    policyController.cpuTempPolicy->orangeThres = 75;
    policyController.cpuTempPolicy->redThres = 85;
    policyController.cpuTempPolicy->state = "GREEN";

    policyController.dimmTempPolicy->activation = false;
    policyController.dimmTempPolicy->greenActive = false;
    policyController.dimmTempPolicy->yellowActive = false;
    policyController.dimmTempPolicy->orangeActive = false;
    policyController.dimmTempPolicy->redActive = false;
    policyController.dimmTempPolicy->yellowThres = 65;
    policyController.dimmTempPolicy->orangeThres = 75;
    policyController.dimmTempPolicy->redThres = 85;
    policyController.dimmTempPolicy->state = "GREEN";

    policyController.diskTempPolicy->activation = false;
    policyController.diskTempPolicy->greenActive = false;
    policyController.diskTempPolicy->yellowActive = false;
    policyController.diskTempPolicy->orangeActive = false;
    policyController.diskTempPolicy->redActive = false;
    policyController.diskTempPolicy->yellowThres = 65;
    policyController.diskTempPolicy->orangeThres = 75;
    policyController.diskTempPolicy->redThres = 85;
    policyController.diskTempPolicy->state = "GREEN";

    policyController.cabinetTempPolicy->activation = false;
    policyController.cabinetTempPolicy->greenActive = false;
    policyController.cabinetTempPolicy->yellowActive = false;
    policyController.cabinetTempPolicy->orangeActive = false;
    policyController.cabinetTempPolicy->redActive = false;
    policyController.cabinetTempPolicy->yellowThres = 65;
    policyController.cabinetTempPolicy->orangeThres = 75;
    policyController.cabinetTempPolicy->redThres = 85;
    policyController.cabinetTempPolicy->state = "GREEN";

    readPolicy();
}

void readPolicy(){
    while(true){
    const char* foflListFile = "/conf/fofllist.json";
    std::ifstream jsonFile(foflListFile);
    if(!jsonFile.is_open()){
        std::cerr<<"FAILED TO READN FOFLLIST"<<std::endl;
    }

    std::string jsonStr;
    std::string line;
    while(std::getline(jsonFile,line)){
        jsonStr += line;
    }

    jsonFile.close();
        rapidjson::Document document;
        document.Parse(jsonStr.c_str());
        if(!document.HasParseError()){
            const rapidjson::Value& foflArray = document["SENSOR_INFO"]["SENSOR"];

            for(rapidjson::SizeType i = 0; i< foflArray.Size(); i++){

                const rapidjson::Value& fofl = foflArray[i];
                const char* name = fofl["NAME"].GetString();
                int number = fofl["NUMBER"].GetInt();
                int green = fofl["GREEN"].GetInt();
                int yellow = fofl["YELLOW"].GetInt();
                int orange = fofl["ORANGE"].GetInt();
                int red = fofl["RED"].GetInt();
                bool greenActivate = intToBool(fofl["GREEN_ACTIVATE"].GetInt());
                bool yellowActivate = intToBool(fofl["YELLOW_ACTIVATE"].GetInt());
                bool orangeActivate = intToBool(fofl["ORANGE_ACTIVATE"].GetInt());
                bool redActivate = intToBool(fofl["RED_ACTIVATE"].GetInt());
                const char* state = fofl["STATE"].GetString();
                //cout<<""

                if(number == 1){
                    policyController.cpuTempPolicy[1].moduleName = name;
                    policyController.cpuTempPolicy[1].greenActive = greenActivate;
                    policyController.cpuTempPolicy[1].yellowActive = yellowActivate;
                    policyController.cpuTempPolicy[1].orangeActive = orangeActivate;
                    policyController.cpuTempPolicy[1].redActive = redActivate;
                    policyController.cpuTempPolicy[1].moduleName = name;
                    policyController.cpuTempPolicy[1].redThres = orange;
                    policyController.cpuTempPolicy[1].orangeThres = yellow;
                    policyController.cpuTempPolicy[1].yellowThres = green;
                }
                else if(number == 10){
                    policyController.diskTempPolicy[0].moduleName = name;
                    policyController.diskTempPolicy[0].greenActive = greenActivate;
                    policyController.diskTempPolicy[0].yellowActive = yellowActivate;
                    policyController.diskTempPolicy[0].orangeActive = orangeActivate;
                    policyController.diskTempPolicy[0].redActive = redActivate;
                    policyController.diskTempPolicy[0].moduleName = name;
                    policyController.diskTempPolicy[0].redThres = orange;
                    policyController.diskTempPolicy[0].orangeThres = yellow;
                    policyController.diskTempPolicy[0].yellowThres = green;
                }
                else if(number == 11){
                    policyController.cabinetTempPolicy[0].moduleName = name;
                    policyController.cabinetTempPolicy[0].greenActive = greenActivate;
                    policyController.cabinetTempPolicy[0].yellowActive = yellowActivate;
                    policyController.cabinetTempPolicy[0].orangeActive = orangeActivate;
                    policyController.cabinetTempPolicy[0].redActive = redActivate;
                    policyController.cabinetTempPolicy[0].moduleName = name;
                    policyController.cabinetTempPolicy[0].redThres = orange;
                    policyController.cabinetTempPolicy[0].orangeThres = yellow;
                    policyController.cabinetTempPolicy[0].yellowThres = green;
                }
                else{
                    number -= 2;
                    policyController.dimmTempPolicy[number].moduleName = name;
                    policyController.dimmTempPolicy[number].greenActive = greenActivate;
                    policyController.dimmTempPolicy[number].yellowActive = yellowActivate;
                    policyController.dimmTempPolicy[number].orangeActive = orangeActivate;
                    policyController.dimmTempPolicy[number].redActive = redActivate;
                    policyController.dimmTempPolicy[number].moduleName = name;
                    policyController.dimmTempPolicy[number].redThres = orange;
                    policyController.dimmTempPolicy[number].orangeThres = yellow;
                    policyController.dimmTempPolicy[number].yellowThres = green;
                }
            }
            std::cout<<"\nREAD FOFL LIST SUCCESSFULLY"<<std::endl;
        }
        else{
            std::cerr<<"\nFAILED TO PARSE FOFL List"<<std::endl;
        }
        updatefoflJson();

        printPolicy(policyController.cpuTempPolicy[1]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.dimmTempPolicy[0]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.dimmTempPolicy[1]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.dimmTempPolicy[2]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.dimmTempPolicy[3]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.dimmTempPolicy[4]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.dimmTempPolicy[5]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.dimmTempPolicy[6]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.dimmTempPolicy[7]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.cabinetTempPolicy[0]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        printPolicy(policyController.diskTempPolicy[0]);



        std::this_thread::sleep_for(std::chrono::seconds(60000));
    }

    // while(true){
    //     try{
    //     policyController.cpuTempPolicy = getTempPolicy(PolicyList::CPU);
    //     policyController.dimmTempPolicy = getTempPolicy(PolicyList::DIMM);
    //     policyController.cabinetTempPolicy = getTempPolicy(PolicyList::Cabinet);
    //     policyController.diskTempPolicy = getTempPolicy(PolicyList::Lm75);
    //     printPolicy(policyController.cpuTempPolicy);
    //     printPolicy(policyController.dimmTempPolicy);
    //     printPolicy(policyController.cabinetTempPolicy);
    //     printPolicy(policyController.diskTempPolicy);
    //     }
    //     catch (const DBus::Error& e){
    //         break;
    //         std::cout<<"DBus Error"<<e<<std::endl;
    //     }
    //     std::this_thread::sleep_for(std::chrono::seconds(20));
    // }
}

int* returnPolicy(int type, int moduleNum){
    int *thres = nullptr;
    switch (type)
    {
    case 0:
        thres = new int[3];
        thres[0] = policyController.cpuTempPolicy[moduleNum].yellowThres;
        thres[1] = policyController.cpuTempPolicy[moduleNum].orangeThres;
        thres[2] = policyController.cpuTempPolicy[moduleNum].redThres;
        return thres;
        break;
    case 1:
        thres = new int[3];
        thres[0] = policyController.dimmTempPolicy[moduleNum].yellowThres;
        thres[1] = policyController.dimmTempPolicy[moduleNum].orangeThres;
        thres[2] = policyController.dimmTempPolicy[moduleNum].redThres;
        return thres;
        break;
    case 2:
        thres = new int[3];
        thres[0] = policyController.cabinetTempPolicy[moduleNum].yellowThres;
        thres[1] = policyController.cabinetTempPolicy[moduleNum].orangeThres;
        thres[2] = policyController.cabinetTempPolicy[moduleNum].redThres;
        return thres;
        break;
    case 3:
        thres = new int[3];
        thres[0] = policyController.diskTempPolicy[moduleNum].yellowThres;
        thres[1] = policyController.diskTempPolicy[moduleNum].orangeThres;
        thres[2] = policyController.diskTempPolicy[moduleNum].redThres;
        return thres;
        break;
    }
}

void printPolicy(TempPolicy tempPolicy){
    std::cout<<"\nModule Name : "<<tempPolicy.moduleName<<"\nGreen Active : "<<tempPolicy.greenActive<<"\nYellow Active : "<<tempPolicy.yellowActive<<"\nOrange Active : "<<tempPolicy.orangeActive<<"\nRed Active : "<<tempPolicy.redActive<<"\n";
    std::cout<<"Yellow Thres: "<<tempPolicy.yellowThres<<"\nOrange thres : "<<tempPolicy.orangeThres<<"\nRed thres : "<<tempPolicy.redThres<<"\nCurrent State : "<<tempPolicy.state<<std::endl;
    return;
}


void getCurrentPolicy(){

    return;
}

void getPolicy(std::string json){
    rapidjson::Document doc;
    const char* jsonString = json.c_str();
    doc.Parse(jsonString);
    if(doc.HasParseError()){
        std::cerr<< "getPolicy DBUS Json Parsing Error\n\n\n\n"<<std::endl;
        return;
    }

    const char* moduleName = doc["CPU"]["MODULE_NAME"].GetString();
    bool activation = doc["CPU"]["ACTIVATION"].GetBool();
    const rapidjson::Value& temperatureArray_cpu = doc["CPU"]["TEMPERATURE"];
    const rapidjson::Value& alarmArray_cpu = doc["CPU"]["ALARM"];
    int moduleNum = getModuleNum(moduleName);
    policyController.cpuTempPolicy[moduleNum].moduleName = moduleName;
    policyController.cpuTempPolicy[moduleNum].activation = activation; 
    policyController.cpuTempPolicy[moduleNum].yellowThres = temperatureArray_cpu[0].GetInt();
    policyController.cpuTempPolicy[moduleNum].orangeThres = temperatureArray_cpu[1].GetInt();
    policyController.cpuTempPolicy[moduleNum].redThres = temperatureArray_cpu[2].GetInt();
    policyController.cpuTempPolicy[moduleNum].greenActive = alarmArray_cpu[0].GetBool();
    policyController.cpuTempPolicy[moduleNum].yellowActive = alarmArray_cpu[1].GetBool();
    policyController.cpuTempPolicy[moduleNum].orangeActive = alarmArray_cpu[2].GetBool();
    policyController.cpuTempPolicy[moduleNum].redActive = alarmArray_cpu[3].GetBool();
    const char* moduleName1 = doc["MEMORY"]["MODULE_NAME"].GetString();
    activation = doc["MEMORY"]["ACTIVATION"].GetBool();
    rapidjson::Value& temperatureArray_mem = doc["MEMORY"]["TEMPERATURE"];
    rapidjson::Value& alarmArray_mem = doc["MEMORY"]["ALARM"];
    moduleNum = getModuleNum(moduleName1);
    policyController.dimmTempPolicy[moduleNum].moduleName = moduleName1;
    policyController.dimmTempPolicy[moduleNum].activation = activation; 
    policyController.dimmTempPolicy[moduleNum].yellowThres = temperatureArray_mem[0].GetInt();
    policyController.dimmTempPolicy[moduleNum].orangeThres = temperatureArray_mem[1].GetInt();
    policyController.dimmTempPolicy[moduleNum].redThres = temperatureArray_mem[2].GetInt();
    policyController.dimmTempPolicy[moduleNum].greenActive = alarmArray_mem[0].GetBool();
    policyController.dimmTempPolicy[moduleNum].yellowActive = alarmArray_mem[1].GetBool();
    policyController.dimmTempPolicy[moduleNum].orangeActive = alarmArray_mem[2].GetBool();
    policyController.dimmTempPolicy[moduleNum].redActive = alarmArray_mem[3].GetBool();

    const char* moduleName2 = doc["DISK"]["MODULE_NAME"].GetString();
    activation = doc["DISK"]["ACTIVATION"].GetBool();
    rapidjson::Value& temperatureArray_disk = doc["DISK"]["TEMPERATURE"];
    rapidjson::Value& alarmArray_disk = doc["DISK"]["ALARM"];
    moduleNum = getModuleNum(moduleName2);
    policyController.diskTempPolicy[moduleNum].moduleName = moduleName2;
    policyController.diskTempPolicy[moduleNum].activation = activation; 
    policyController.diskTempPolicy[moduleNum].yellowThres = temperatureArray_disk[0].GetInt();
    policyController.diskTempPolicy[moduleNum].orangeThres = temperatureArray_disk[1].GetInt();
    policyController.diskTempPolicy[moduleNum].redThres = temperatureArray_disk[2].GetInt();
    policyController.diskTempPolicy[moduleNum].greenActive = alarmArray_disk[0].GetBool();
    policyController.diskTempPolicy[moduleNum].yellowActive = alarmArray_disk[1].GetBool();
    policyController.diskTempPolicy[moduleNum].orangeActive = alarmArray_disk[2].GetBool();
    policyController.diskTempPolicy[moduleNum].redActive = alarmArray_disk[3].GetBool();

    const char* moduleName3 = doc["CABINET"]["MODULE_NAME"].GetString();
    activation = doc["CABINET"]["ACTIVATION"].GetBool();
    rapidjson::Value& temperatureArray = doc["CABINET"]["TEMPERATURE"];
    rapidjson::Value& alarmArray = doc["CABINET"]["ALARM"];
    moduleNum = 0;
    policyController.cabinetTempPolicy[moduleNum].moduleName = moduleName3;
    policyController.cabinetTempPolicy[moduleNum].activation = activation; 
    policyController.cabinetTempPolicy[moduleNum].yellowThres = temperatureArray[0].GetInt();
    policyController.cabinetTempPolicy[moduleNum].orangeThres = temperatureArray[1].GetInt();
    policyController.cabinetTempPolicy[moduleNum].redThres = temperatureArray[2].GetInt();
    policyController.cabinetTempPolicy[moduleNum].greenActive = alarmArray[0].GetBool();
    policyController.cabinetTempPolicy[moduleNum].yellowActive = alarmArray[1].GetBool();
    policyController.cabinetTempPolicy[moduleNum].orangeActive = alarmArray[2].GetBool();
    policyController.cabinetTempPolicy[moduleNum].redActive = alarmArray[3].GetBool();

    printPolicy(policyController.cpuTempPolicy[1]);
    printPolicy(policyController.dimmTempPolicy[1]);
    printPolicy(policyController.diskTempPolicy[1]);
    printPolicy(policyController.cabinetTempPolicy[0]);
}

int getModuleNum(const char* module){
    int num = 0;
    int length = std::strlen(module);
    for(int i=length-1;i>=0;i--){
        if(std::isdigit(module[i])){
            num = module[i] - '0';
            break;
        }
    }
    return num;
}

bool* getActivation(int type, int moduleNum){
    bool *thres = nullptr;
    switch (type)
    {
    case 0:
        thres = new bool[4];
        thres[0] = policyController.cpuTempPolicy[moduleNum].greenActive;
        thres[1] = policyController.cpuTempPolicy[moduleNum].yellowActive;
        thres[2] = policyController.cpuTempPolicy[moduleNum].orangeActive;
        thres[3] = policyController.cpuTempPolicy[moduleNum].redActive;
        return thres;
        break;
    case 1:
        thres = new bool[4];
        thres[0] = policyController.dimmTempPolicy[moduleNum].greenActive;
        thres[1] = policyController.dimmTempPolicy[moduleNum].yellowActive;
        thres[2] = policyController.dimmTempPolicy[moduleNum].orangeActive;
        thres[3] = policyController.dimmTempPolicy[moduleNum].redActive;
        return thres;
        break;
    case 2:
        thres = new bool[4];
        thres[0] = policyController.cabinetTempPolicy[moduleNum].greenActive;
        thres[1] = policyController.cabinetTempPolicy[moduleNum].yellowActive;
        thres[2] = policyController.cabinetTempPolicy[moduleNum].orangeActive;
        thres[3] = policyController.cabinetTempPolicy[moduleNum].redActive;
        return thres;
        break;
    case 3:
        thres = new bool[4];
        thres[0] = policyController.diskTempPolicy[moduleNum].greenActive;
        thres[1] = policyController.diskTempPolicy[moduleNum].yellowActive;
        thres[2] = policyController.diskTempPolicy[moduleNum].orangeActive;
        thres[3] = policyController.diskTempPolicy[moduleNum].redActive;
        return thres;
        break;
    }
}

bool intToBool (int num){
    if(num == 1)
        return true;
    else
        return false;
}

void updatefoflJson(){
    std::string sourceFileName = "/conf/fofllist.json";
    std::string updateFileName = "/conf/fofl_update.json";

    std::ifstream sourceFile(sourceFileName, std::ios::binary);
    std::ofstream updateFile(updateFileName,std::ios::binary);

    if(!sourceFile.is_open() || !updateFile.is_open()){
        std::cerr<<"FAILED TO OPEN FOFL LIST"<<std::endl;
        return;
    }

    updateFile<<sourceFile.rdbuf();
    sourceFile.close();
    updateFile.close();

    std::ifstream inputFile("/conf/fofllist.json");
    if(!inputFile){
        std::cerr <<"FAILED TO UPDATE FOFL LIST"<<std::endl;
        return;
    }

    std::string jsonData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    rapidjson::Document doc;
    doc.Parse(jsonData.c_str());


    if(doc.HasMember("SENSOR_INFO") && doc["SENSOR_INFO"].HasMember("SENSOR") && doc["SENSOR_INFO"]["SENSOR"].IsArray()){
        rapidjson::Value& sensorArray = doc["SENSOR_INFO"]["SENSOR"];
        for (rapidjson::SizeType i =0;i<sensorArray.Size();i++){
            if(sensorArray[i].HasMember("STATE")){
                if(sensorArray[i]["NUMBER"] == 1){ // cpu
                    sensorArray[i]["STATE"].SetString(policyController.cpuTempPolicy[1].state.c_str(),doc.GetAllocator());

                    if(!sensorArray[i].HasMember("NEXT_STATE_TIME")){
                        rapidjson::Value value;
                        value.SetString(policyController.cpuTempPolicy[1].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("NEXT_STATE_TIME",value,doc.GetAllocator());
                    }
                    else{
                        sensorArray[i]["NEXT_STATE_TIME"].SetString(policyController.cpuTempPolicy[1].nextStateSecond.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("FEEDBACK_STATE")){
                        rapidjson::Value value;
                        value.SetString(policyController.cpuTempPolicy[1].feedbackState,doc.GetAllocator());
                        sensorArray[i].AddMember("FEEDBACK_STATE",value,doc.GetAllocator());
                    }
                    else{
                        sensorArray[i]["FEEDBACK_STATE"].SetString(policyController.cpuTempPolicy[1].feedbackState.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("RED_STATE_CAUSE")){
                        rapidjson::Value value;
                        value.SetString(policyController.cpuTempPolicy[1].redCause,doc.GetAllocator());
                        sensorArray[i].AddMember("RED_STATE_CAUSE",value,doc.GetAllocator());
                    }
                    else{
                        sensorArray[i]["RED_STATE_CAUSE"].SetString(policyController.cpuTempPolicy[1].redCause.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("SUGGEST_OPTION")){
                        rapidjson::Value value;
                        value.SetString(policyController.cpuTempPolicy[1].suggestOption,doc.GetAllocator());
                        sensorArray[i].AddMember("SUGGEST_OPTION",value,doc.GetAllocator());
                    }
                    else{
                        sensorArray[i]["SUGGEST_OPTION"].SetString(policyController.cpuTempPolicy[1].suggestOption.c_str(),doc.GetAllocator());
                    }
                }

                else if(sensorArray[i]["NUMBER"] == 10){ // disk
                    sensorArray[i]["STATE"].SetString(policyController.diskTempPolicy[0].state.c_str(),doc.GetAllocator());

                    if(!sensorArray[i].HasMember("NEXT_STATE_TIME")){
                        rapidjson::Value value;
                        value.SetString(policyController.diskTempPolicy[0].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("NEXT_STATE_TIME",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["NEXT_STATE_TIME"].SetString(policyController.diskTempPolicy[0].nextStateSecond.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("FEEDBACK_STATE")){
                        rapidjson::Value value;
                        value.SetString(policyController.diskTempPolicy[0].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("FEEDBACK_STATE",value,doc.GetAllocator());
                       
                    }
                    else{
                        sensorArray[i]["FEEDBACK_STATE"].SetString(policyController.diskTempPolicy[0].feedbackState.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("RED_STATE_CAUSE")){
                        rapidjson::Value value;
                        value.SetString(policyController.diskTempPolicy[0].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("RED_STATE_CAUSE",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["RED_STATE_CAUSE"].SetString(policyController.diskTempPolicy[0].redCause.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("SUGGEST_OPTION")){
                        rapidjson::Value value;
                        value.SetString(policyController.diskTempPolicy[0].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("SUGGEST_OPTION",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["SUGGEST_OPTION"].SetString(policyController.diskTempPolicy[0].suggestOption.c_str(),doc.GetAllocator());
                    }
                }


                else if(sensorArray[i]["NUMBER"] == 11){ // cabubte
                    sensorArray[i]["STATE"].SetString(policyController.cabinetTempPolicy[0].state.c_str(),doc.GetAllocator());

                    if(!sensorArray[i].HasMember("NEXT_STATE_TIME")){
                        rapidjson::Value value;
                        value.SetString(policyController.cabinetTempPolicy[0].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("NEXT_STATE_TIME",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["NEXT_STATE_TIME"].SetString(policyController.cabinetTempPolicy[0].nextStateSecond.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("FEEDBACK_STATE")){
                        rapidjson::Value value;
                        value.SetString(policyController.cabinetTempPolicy[0].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("FEEDBACK_STATE",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["FEEDBACK_STATE"].SetString(policyController.cabinetTempPolicy[0].feedbackState.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("RED_STATE_CAUSE")){
                        rapidjson::Value value;
                        value.SetString(policyController.cabinetTempPolicy[0].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("RED_STATE_CAUSE",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["RED_STATE_CAUSE"].SetString(policyController.cabinetTempPolicy[0].redCause.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("SUGGEST_OPTION")){
                        rapidjson::Value value;
                        value.SetString(policyController.cabinetTempPolicy[0].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("SUGGEST_OPTION",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["SUGGEST_OPTION"].SetString(policyController.cabinetTempPolicy[0].suggestOption.c_str(),doc.GetAllocator());
                    }
                }

                
                else{ // dimm
                    sensorArray[i]["STATE"].SetString(policyController.dimmTempPolicy[i-1].state.c_str(),doc.GetAllocator());

                    if(!sensorArray[i].HasMember("NEXT_STATE_TIME")){
                        rapidjson::Value value;
                        value.SetString(policyController.dimmTempPolicy[i-1].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("NEXT_STATE_TIME",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["NEXT_STATE_TIME"].SetString(policyController.dimmTempPolicy[i-1].nextStateSecond.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("FEEDBACK_STATE")){
                        rapidjson::Value value;
                        value.SetString(policyController.dimmTempPolicy[i-1].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("FEEDBACK_STATE",value,doc.GetAllocator());
                       
                    }
                    else{
                        sensorArray[i]["FEEDBACK_STATE"].SetString(policyController.dimmTempPolicy[i-1].feedbackState.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("RED_STATE_CAUSE")){
                        rapidjson::Value value;
                        value.SetString(policyController.dimmTempPolicy[i-1].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("RED_STATE_CAUSE",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["RED_STATE_CAUSE"].SetString(policyController.dimmTempPolicy[i-1].redCause.c_str(),doc.GetAllocator());
                    }

                    if(!sensorArray[i].HasMember("SUGGEST_OPTION")){
                        rapidjson::Value value;
                        value.SetString(policyController.dimmTempPolicy[i-1].nextStateSecond ,doc.GetAllocator());
                        sensorArray[i].AddMember("SUGGEST_OPTION",value,doc.GetAllocator());
                        
                    }
                    else{
                        sensorArray[i]["SUGGEST_OPTION"].SetString(policyController.dimmTempPolicy[i-1].suggestOption.c_str(),doc.GetAllocator());
                    }
                }
            }
            
            
        }
    }

    std::ofstream output("/conf/fofllist.json");
    if(!output){
        std::cerr << "FAILED TO UPDATE FOFL LIST" << std::endl;
        return;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    output<<buffer.GetString();
    output.close();
}

void updateState(int moduleName, int moduleNum, int state, bool ongoing, int time){
    if(moduleName == 0){ // cpu
    if(time == 0){
        policyController.cpuTempPolicy[moduleNum].nextStateSecond = "Stable";
    }
    else{
    policyController.cpuTempPolicy[moduleNum].nextStateSecond = timeToString(time);}

    if(ongoing == true){
        policyController.cpuTempPolicy[moduleNum].feedbackState = "Feedback Running";
        policyController.cpuTempPolicy[moduleNum].redCause = "Fan Module Missing";
        policyController.cpuTempPolicy[moduleNum].suggestOption = "Adjust Fan Policy, Repair Fan Module";
    }
    else if(ongoing == false){
        policyController.cpuTempPolicy[moduleNum].feedbackState = "Not Running";
        policyController.cpuTempPolicy[moduleNum].redCause = "-";
        policyController.cpuTempPolicy[moduleNum].suggestOption = "-";
    }

    if(state == 0){
        policyController.cpuTempPolicy[moduleNum].state = "GREEN";
    }
    else if(state == 1){
        policyController.cpuTempPolicy[moduleNum].state = "YELLOW";   
    }
    else if(state == 2){
        policyController.cpuTempPolicy[moduleNum].state = "ORANGE";
    }
    else{
        policyController.cpuTempPolicy[moduleNum].state = "RED";
    }
    }

    else if(moduleName == 1){ // memory
    if(time == 0){
        policyController.dimmTempPolicy[moduleNum].nextStateSecond = "Stable";
    }
    else{
    policyController.dimmTempPolicy[moduleNum].nextStateSecond = timeToString(time);
    }

    if(ongoing == true){
        policyController.dimmTempPolicy[moduleNum].feedbackState = "Feedback Running";
        policyController.dimmTempPolicy[moduleNum].redCause = "Fan Module Missing";
        policyController.dimmTempPolicy[moduleNum].suggestOption = "Adjust Fan Policy, Repair Fan Module";
    }
    else if(ongoing == false){
        policyController.dimmTempPolicy[moduleNum].feedbackState = "Not Running";
        policyController.dimmTempPolicy[moduleNum].redCause = "-";
        policyController.dimmTempPolicy[moduleNum].suggestOption = "-";
    }


    if(state == 0){
        policyController.dimmTempPolicy[moduleNum].state = "GREEN";
    }
    else if(state == 1){
        policyController.dimmTempPolicy[moduleNum].state = "YELLOW";
    }
    else if(state == 2){
        policyController.dimmTempPolicy[moduleNum].state = "ORANGE";
    }
    else{
        policyController.dimmTempPolicy[moduleNum].state = "RED";
    }
    }

    else if(moduleName == 2){ // storage
    if(time == 0){
        policyController.diskTempPolicy[moduleNum].nextStateSecond = "Stable";
    }
    else{
    policyController.diskTempPolicy[moduleNum].nextStateSecond = timeToString(time);
    }

    
    if(ongoing == true){
        policyController.diskTempPolicy[moduleNum].feedbackState = "Feedback Running";
        policyController.diskTempPolicy[moduleNum].redCause = "Fan Module Missing";
        policyController.diskTempPolicy[moduleNum].suggestOption = "Adjust Fan Policy, Repair Fan Module";
    }
    else if(ongoing == false){
        policyController.diskTempPolicy[moduleNum].feedbackState = "Not Running";
        policyController.diskTempPolicy[moduleNum].redCause = "-";
        policyController.diskTempPolicy[moduleNum].suggestOption = "-";
    }


    if(state == 0){
        policyController.diskTempPolicy[moduleNum].state = "GREEN";
    }
    else if(state == 1){
        policyController.diskTempPolicy[moduleNum].state = "YELLOW";
    }
    else if(state == 2){
        policyController.diskTempPolicy[moduleNum].state = "ORANGE";
    }
    else{
        policyController.diskTempPolicy[moduleNum].state = "RED";
    }
    }

    else if(moduleName == 4){ // cabinet
    if(time == 0){
        policyController.cabinetTempPolicy[moduleNum].nextStateSecond = "Stable";
    }
    else{
    policyController.cabinetTempPolicy[moduleNum].nextStateSecond = timeToString(time);
    }

    if(ongoing == true){
        policyController.cabinetTempPolicy[moduleNum].feedbackState = "Feedback Running";
        policyController.cabinetTempPolicy[moduleNum].redCause = "Fan Module Missing";
        policyController.cabinetTempPolicy[moduleNum].suggestOption = "Adjust Fan Policy, Repair Fan Module";
    }
    else if(ongoing == false){
        policyController.cabinetTempPolicy[moduleNum].feedbackState = "Not Running";
        policyController.cabinetTempPolicy[moduleNum].redCause = "-";
        policyController.cabinetTempPolicy[moduleNum].suggestOption = "-";
    }


    if(state == 0){
        policyController.cabinetTempPolicy[moduleNum].state = "GREEN";
    }
    else if(state == 1){
        policyController.cabinetTempPolicy[moduleNum].state = "YELLOW";  
    }
    else if(state == 2){
        policyController.cabinetTempPolicy[moduleNum].state = "ORANGE";
    }
    else{
        policyController.cabinetTempPolicy[moduleNum].state = "RED";
    }
    }

    return;
}

std::string timeToString(int time){
    int minutes = time / 60;
    int seconds = time % 60;
    std::string result = std::to_string(minutes) + " minutes " + std::to_string(seconds) + " seconds";
    return result;
}
