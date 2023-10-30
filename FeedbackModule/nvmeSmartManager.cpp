
#include "nvmeSmartManager.hpp"
#include "foflManager.hpp"
#include "policyController.hpp"
#include <fstream>
#include <string>
NvmeSmartManager nvmeSmartManager;

void nvmeInit(){
}

void getSmartInfo(string json){
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value obj(rapidjson::kObjectType);
    
    string disk_char;
    rapidjson::Document document;
    rapidjson::Value stringValue;
    document.Parse(json.c_str());
    if (!document.IsObject()) {
        std::cerr << "JSON 파싱 실패" << std::endl;
        return;
    }
    const rapidjson::Value& device_value = document["device"];
    if(device_value.IsObject()){
        const rapidjson::Value& device_name_value = device_value["name"];
        if(device_name_value.IsString()){
            disk_char = device_name_value.GetString();
            std::string device_name = device_name_value.GetString();
            nvmeSmartManager.smartInfoMap[disk_char].device_name = device_name;
            std::cout<<"\nmodel device name : "<<device_name<<"\n";
        }

        const rapidjson::Value& device_type_value = device_value["type"];
        if(device_type_value.IsString()){
            std::string device_type = device_type_value.GetString();
            if(device_type == "nvme"){
                nvmeSmartManager.smartInfoMap[disk_char].diskType = nvme;
            }
            std::cout<<"model device type : "<<device_type<<"\n";
        }
    }

    const rapidjson::Value& model_name_value = document["model_name"];
    if(model_name_value.IsString()){
        std::string model_name = model_name_value.GetString();
        nvmeSmartManager.smartInfoMap[disk_char].model_name = model_name;

        stringValue.SetString(model_name.c_str(),model_name.length(),allocator);
        obj.AddMember("MODEL_NAME",stringValue,allocator);
        std::cout<<"model name : "<<model_name<<"\n";
    }
    const rapidjson::Value& serial_number_value = document["serial_number"];
    if(serial_number_value.IsString()){
        std::string serial_number = serial_number_value.GetString();
        nvmeSmartManager.smartInfoMap[disk_char].serial_number = serial_number;

        stringValue.SetString(serial_number.c_str(),serial_number.length(),allocator);
        obj.AddMember("SERIAL_NUMBER",stringValue,allocator);
        std::cout<<"serial_number : "<<serial_number<<"\n";
    }

    //임시
    nvmeSmartManager.smartInfoMap[disk_char].pci_vendor = "Innodisk";
    stringValue.SetString(nvmeSmartManager.smartInfoMap[disk_char].pci_vendor.c_str(),nvmeSmartManager.smartInfoMap[disk_char].pci_vendor.length(),allocator);
    obj.AddMember("VENDOR_NAME",stringValue,allocator);
    // const rapidjson::Value& pci_vendor_value = document["pci_vendor"];
    // if(pci_vendor_value.IsString()){
    //     std::string pci_vendor = pci_vendor_value.GetString();
    //     nvmeSmartManager.nvmeSmartInfo.pci_vendor = pci_vendor;
    //     std::cout<<"model pci_vendor : "<<pci_vendor<<"\n";
    // }

    const rapidjson::Value& user_capacity_value = document["user_capacity"];
    if(user_capacity_value.IsObject()){
        const rapidjson::Value& user_capacity_value_bytes = user_capacity_value["bytes"];
        if(user_capacity_value_bytes.IsUint64()){
        uint64_t user_capacity = user_capacity_value_bytes.GetUint64();
        double gigabytes = static_cast<double>(user_capacity) / (1024 * 1024 * 1024);
        nvmeSmartManager.smartInfoMap[disk_char].user_capacity = gigabytes;
        std::stringstream sliceStream;
        sliceStream << std::fixed << std::setprecision(2) << gigabytes;
        std::string cutValue = sliceStream.str();
        cutValue += " GB";

        stringValue.SetString(cutValue.c_str(),cutValue.length(),allocator);
        obj.AddMember("TOTAL",stringValue,allocator);
        std::cout<<"model user_capacity : "<<user_capacity<<"\n";
        }
    }
    const rapidjson::Value& smart_status_value = document["smart_status"];
    if(smart_status_value.IsObject()){
        const rapidjson::Value& smart_status_passed_value = smart_status_value["passed"];
        if(smart_status_passed_value.IsBool()){
        bool smart_status = smart_status_passed_value.GetBool();
        nvmeSmartManager.smartInfoMap[disk_char].smart_status = smart_status;
        if(smart_status == true){
            
            obj.AddMember("DISK_SELF_TEST","PASSED",allocator);
        }
        else{
            obj.AddMember("DISK_SELF_TEST","FAILED",allocator);
        }
        
        std::cout<<"model smart_status : "<<smart_status<<"\n";
        }
    }
    const rapidjson::Value& nvme_smart_info_log = document["nvme_smart_health_information_log"];
    if(nvme_smart_info_log.IsObject()){
        const rapidjson::Value& nvme_critical_warning = nvme_smart_info_log["critical_warning"];
        if(nvme_critical_warning.IsUint()){
            int cri_warning = nvme_critical_warning.GetUint();
            nvmeSmartManager.smartInfoMap[disk_char].critical_warning = cri_warning;

            stringValue.SetString(std::to_string(cri_warning).c_str(),std::to_string(cri_warning).length(),allocator);
            obj.AddMember("FATAL_ERROR_COUNT",stringValue,allocator);
            std::cout<<"model critical_warning : "<<cri_warning<<"\n";
        }
        
        const rapidjson::Value& nvme_temp_value = nvme_smart_info_log["temperature"];
        if(nvme_temp_value.IsUint64()){
            int temperature = nvme_temp_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].temperature = temperature;
            std::string tempString = std::to_string(temperature) + " °C";
            stringValue.SetString(tempString.c_str(),tempString.length(),allocator);
            obj.AddMember("DISK_TEMPERATURE",stringValue,allocator);
            std::cout<<"model temperature : "<<temperature<<"\n";
            
        }

        const rapidjson::Value& nvme_available_value = nvme_smart_info_log["available_spare"];
        if(nvme_available_value.IsUint64()){
            int avail_value = nvme_available_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].available_spare = avail_value;
            std::string availString = std::to_string(avail_value) + " %";
            stringValue.SetString(availString.c_str(),availString.length(),allocator);
            obj.AddMember("AMOUNT_OF_BLOCK_AVAILABLE",stringValue,allocator);
            std::cout<<"model available_spare : "<<avail_value<<"\n";
        }

        const rapidjson::Value& used_value = nvme_smart_info_log["percentage_used"];
        if(used_value.IsUint64()){
            int percent = used_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].percentage_used = percent;
            std::string percentString = std::to_string(percent) + " %";
            stringValue.SetString(percentString.c_str(),percentString.length(),allocator);
            obj.AddMember("DISK_USAGE",stringValue,allocator);
            std::cout<<"model percentage_used : "<<percent<<"\n";
        }

        const rapidjson::Value& data_units_read_value = nvme_smart_info_log["data_units_read"];
        if(data_units_read_value.IsUint64()){
            int data_units_read = data_units_read_value.GetUint64();
            data_units_read /= 2048;
            nvmeSmartManager.smartInfoMap[disk_char].data_units_read = data_units_read;
            std::string readString = std::to_string(data_units_read) + " GB";
            stringValue.SetString(readString.c_str(),readString.length(),allocator);
            obj.AddMember("READ_DATA_SIZE",stringValue,allocator);
            std::cout<<"model data_units_read : "<<data_units_read<<"\n";
        }

        const rapidjson::Value& data_units_written_value = nvme_smart_info_log["data_units_written"];
        if(data_units_written_value.IsUint64()){
            int data_units_written = data_units_written_value.GetUint64();
            data_units_written /= 2048;
            nvmeSmartManager.smartInfoMap[disk_char].data_units_written = data_units_written;
            std::string writtenString = std::to_string(data_units_written) + " GB";
            stringValue.SetString(writtenString.c_str(),writtenString.length(),allocator);
            obj.AddMember("WRITE_DATA_SIZE",stringValue,allocator);
            std::cout<<"model data_units_written : "<<data_units_written<<"\n";
        }

        const rapidjson::Value& controller_busy_time_value = nvme_smart_info_log["controller_busy_time"];
        if(controller_busy_time_value.IsUint64()){
            int controller_busy_time = controller_busy_time_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].controller_busy_time = controller_busy_time;
            int controller_month = controller_busy_time / 720;
            int controller_day = (controller_busy_time % 720) / 24;
            int controller_hour = (controller_busy_time % 720) % 24;
            std::string operateString;
            if(controller_month == 0){
                operateString = std::to_string(controller_day) + " days " + std::to_string(controller_hour) + " hours";
            }
            else{
                operateString = std::to_string(controller_month) + " months " + std::to_string(controller_day) + " days " + std::to_string(controller_hour) + " hours";
            }
            stringValue.SetString(operateString.c_str(),operateString.length(),allocator);
            obj.AddMember("OPERATION_TIME",stringValue,allocator);
            std::cout<<"model controller_busy_time : "<<controller_busy_time<<"\n";
        }

        const rapidjson::Value& power_on_cycles_value = nvme_smart_info_log["power_cycles"];
        if(power_on_cycles_value.IsUint64()){
            int power_on_cycles = power_on_cycles_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].power_on_cycles = power_on_cycles;

            std::cout<<"model power_on_cycles : "<<power_on_cycles<<"\n";
        }

        const rapidjson::Value& power_on_hours_value = nvme_smart_info_log["power_on_hours"];
        if(power_on_hours_value.IsUint64()){
            int power_on_hours = power_on_hours_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].power_on_hours = power_on_hours;
            int power_on_month = power_on_hours / 720;
            int power_on_day = (power_on_hours % 720) / 24;  
            std::string resultString = std::to_string(power_on_month) + " Months " + std::to_string(power_on_day) + " Days";
            stringValue.SetString(resultString.c_str(),resultString.length(),allocator);
            obj.AddMember("USAGE_TIME",stringValue,allocator);
            std::cout<<"model power_on_hours : "<<power_on_hours<<"\n";
        }

        const rapidjson::Value& unsafe_shutdowns_value = nvme_smart_info_log["unsafe_shutdowns"];
        if(unsafe_shutdowns_value.IsUint64()){
            int unsafe_shutdowns = unsafe_shutdowns_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].unsafe_shutdowns = unsafe_shutdowns;
            stringValue.SetString(std::to_string(unsafe_shutdowns).c_str(),std::to_string(unsafe_shutdowns).length(),allocator);
            obj.AddMember("NUMBER_OF_ABNORMAL_TERMINATIONS",stringValue,allocator);
            std::cout<<"model unsafe_shutdowns : "<<unsafe_shutdowns<<"\n";
        }

        const rapidjson::Value& media_errors_value = nvme_smart_info_log["media_errors"];
        if(media_errors_value.IsUint64()){
            int media_errors = media_errors_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].media_errors = media_errors;
            stringValue.SetString(std::to_string(media_errors).c_str(),std::to_string(media_errors).length(),allocator);
            obj.AddMember("DATA_INTEGRITY_ERROR",stringValue,allocator);
            std::cout<<"model media_errors : "<<media_errors<<"\n";
        }
    }
    if(nvmeSmartManager.smartInfoMap[disk_char].percentage_used == 0){
        nvmeSmartManager.smartInfoMap[disk_char].current_capacity =  nvmeSmartManager.smartInfoMap[disk_char].user_capacity * 0.01;
    }
    else{
        nvmeSmartManager.smartInfoMap[disk_char].current_capacity = nvmeSmartManager.smartInfoMap[disk_char].user_capacity * nvmeSmartManager.smartInfoMap[disk_char].percentage_used / 100;
    }
    std::cout<<"model current capacity : "<<nvmeSmartManager.smartInfoMap[disk_char].current_capacity<<"\n";

    nvmeSmartManager.smartInfoMap[disk_char].health_score = calcScore(nvmeSmartManager.smartInfoMap[disk_char].available_spare);
    std::cout<<"model current health_score : "<<nvmeSmartManager.smartInfoMap[disk_char].health_score<<"\n";
    if(nvmeSmartManager.smartInfoMap[disk_char].available_spare == 100){
        nvmeSmartManager.smartInfoMap[disk_char].lifespan = nvmeSmartManager.smartInfoMap[disk_char].power_on_hours * 99;
    }
    
    else{
        nvmeSmartManager.smartInfoMap[disk_char].lifespan = nvmeSmartManager.smartInfoMap[disk_char].power_on_hours*nvmeSmartManager.smartInfoMap[disk_char].available_spare;
    }
    std::cout<<"model current lifespan : "<<nvmeSmartManager.smartInfoMap[disk_char].lifespan<<"\n";
    nvmeSmartManager.smartInfoMap[disk_char].fault_chance = 0;
    std::cout<<"model current fault_chance : "<<nvmeSmartManager.smartInfoMap[disk_char].fault_chance<<"\n";
    double cur_capacity = nvmeSmartManager.smartInfoMap[disk_char].current_capacity;
    std::ostringstream ossStream;
    ossStream << std::fixed <<std::setprecision(2)<<cur_capacity;
    std::string str_curCapacity = ossStream.str();
    str_curCapacity += " GB";
    stringValue.SetString(str_curCapacity.c_str(),str_curCapacity.length(),allocator);
    obj.AddMember("USAGE",stringValue,allocator);
    std::cout<<"model current Usage : "<<str_curCapacity<<std::endl;

    int disk_lifeSpan = nvmeSmartManager.smartInfoMap[disk_char].lifespan;
    int lifespan_year = disk_lifeSpan / 8760;
    int lifespan_month = (disk_lifeSpan % 8760) / 720;
    std::string lifeSpane_str = std::to_string(lifespan_year) + " Years " + std::to_string(lifespan_month) + " Months"; 
    stringValue.SetString(lifeSpane_str.c_str(),lifeSpane_str.length(),allocator);
    obj.AddMember("ESTIMATED_DISK_LIFE",stringValue,allocator);
    std::cout<<"model estimated disk lifespan : "+lifeSpane_str+"\n";
    stringValue.SetString(std::to_string(nvmeSmartManager.smartInfoMap[disk_char].health_score).c_str(),std::to_string(nvmeSmartManager.smartInfoMap[disk_char].health_score).length(),allocator);
    obj.AddMember("STATUS_SCORE",stringValue,allocator);
    float disk_faultChance = nvmeSmartManager.smartInfoMap[disk_char].fault_chance;
    std::ostringstream ossStream1;
    ossStream1 << std::fixed<<std::setprecision(2) << disk_faultChance;
    std::string str_diskFault = ossStream1.str();
    str_diskFault += " %";
    stringValue.SetString(str_diskFault.c_str(),str_diskFault.length(),allocator);
    obj.AddMember("PROBABILITY_OF_ERROR_WITHIN_THE_YEAR",stringValue,allocator);
    std::cout<<"model Probability of error in year : "<<str_diskFault<<"\n";
    doc.Swap(obj);
    rapidjson::StringBuffer jsonBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(jsonBuffer);
    doc.Accept(writer);

    std::string jsonStr = jsonBuffer.GetString();
    std::string filePath = "/conf/smartInfo.json";
    std::ofstream _outputFile(filePath);
    if(_outputFile.is_open()){
        _outputFile<<jsonStr;
        _outputFile.close();
        std::cout<<"\nSMART INFORMATION SAVED\n"<<std::endl;
    }
    changeState(diskTemp, nvmeSmartManager.smartInfoMap[disk_char].temperature, 0);
    storeDiskHealth(disk_char);
}

void storeDiskHealth(std::string chardisk){
    rapidjson::Value stringValue;
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    std::string disk_name;
    disk_name = nvmeSmartManager.smartInfoMap[chardisk].pci_vendor + " NVME0";
    rapidjson::Value name;
    name.SetString(disk_name.c_str(), static_cast<int>(disk_name.length()),allocator);
    document.AddMember("NAME",name,allocator);
    document.AddMember("SCORE",nvmeSmartManager.smartInfoMap[chardisk].health_score,allocator);
    // int disk_lifeSpan = nvmeSmartManager.smartInfoMap[chardisk].lifespan;
    // int lifespan_year = disk_lifeSpan / 8760;
    // int lifespan_month = (disk_lifeSpan % 8760) / 720;
    // std::string lifeSpane_str = std::to_string(lifespan_year) + " Years " + std::to_string(lifespan_month) + " Months"; 
    // stringValue.SetString(lifeSpane_str.c_str(),lifeSpane_str.length(),allocator);
    // document.AddMember("DISK LIFE",stringValue,allocator);

    // document.AddMember("CRITICAL WARNING",nvmeSmartManager.smartInfoMap[chardisk].critical_warning,allocator);
    // float disk_faultChance = nvmeSmartManager.smartInfoMap[chardisk].fault_chance;
    // std::ostringstream ossStream1;
    // ossStream1 << std::fixed<<std::setprecision(2) << disk_faultChance;
    // std::string str_diskFault = ossStream1.str();
    // str_diskFault += " %";
    // stringValue.SetString(str_diskFault.c_str(),str_diskFault.length(),allocator);
    // document.AddMember("ERROR WIRHIN YEAR",stringValue, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string jsonStr = buffer.GetString();
    std::string filePath = "/conf/diskscore.json";
    std::ofstream outputFile(filePath);
    if(outputFile.is_open()){
        outputFile<<jsonStr;
        outputFile.close();
        std::cout<<"\nSMART HEALTH SCORE SAVED"<<std::endl;
    }
    else{
        std::cout<<"\nSTORE HEALTH DISK FAILED"<<std::endl;
    }
}

int calcScore(int value){
    int score = value + ( 100 - value ) * log2(1 + ((value - 1)/ 99));
    return score;
}


