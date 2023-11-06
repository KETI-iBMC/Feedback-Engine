#pragma once
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" 
#include <map>
#include <iostream>
#include <string>
#include <unordered_map>
#include <cmath>

using namespace std;

enum diskType{
    hdd,
    sdd,
    nvme
};

struct nvmeSmartInfo{
    diskType diskType = nvme;
    string device_name = "";
    string model_name = "";
    string serial_number ="";
    string pci_vendor = "";
    double user_capacity = 0;
    bool smart_status = 0;
    int critical_warning = 0;
    int temperature = 0;
    int available_spare = 0;
    int percentage_used = 0;
    uint64_t data_units_read = 0;
    uint64_t data_units_written = 0;
    int controller_busy_time = 0;
    int power_on_cycles = 0;
    int power_on_hours = 0;
    int unsafe_shutdowns = 0;
    int media_errors = 0;
    double current_capacity = 0;
    int health_score = 0;
    int lifespan = 0;
    float fault_chance = 0;
};

class NvmeSmartManager{
    private:
    public:
        map<string,nvmeSmartInfo> smartInfoMap;
        map<string,nvmeSmartInfo>::iterator p;
        NvmeSmartManager(){}
};

void getSmartInfo(string document);
void storeDiskHealth(string diskchar);
int calcScore(int value);