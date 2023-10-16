#include <iostream>
#include <string>
#include <unordered_map>
#include "nvmeSmartManager.hpp"

NvmeSmartManager nvmeSmartManager;

void nvmeInit(){

}

void getSmartInfo(string json){
    string disk_char;
    rapidjson::Document document;
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
            std::cout<<"model device name : "<<device_name<<"\n";
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
        std::cout<<"model name : "<<model_name<<"\n";
    }
    const rapidjson::Value& serial_number_value = document["serial_number"];
    if(serial_number_value.IsString()){
        std::string serial_number = serial_number_value.GetString();
        nvmeSmartManager.smartInfoMap[disk_char].serial_number = serial_number;
        std::cout<<"serial_number : "<<serial_number<<"\n";
    }

    //임시
    nvmeSmartManager.smartInfoMap[disk_char].pci_vendor = "Innodisk Coporation";
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
        std::cout<<"model user_capacity : "<<user_capacity<<"\n";
        }
    }
    const rapidjson::Value& smart_status_value = document["smart_status"];
    if(smart_status_value.IsObject()){
        const rapidjson::Value& smart_status_passed_value = smart_status_value["passed"];
        if(smart_status_passed_value.IsBool()){
        bool smart_status = smart_status_passed_value.GetBool();
        nvmeSmartManager.smartInfoMap[disk_char].smart_status = smart_status;
        std::cout<<"model smart_status : "<<smart_status<<"\n";
        }
    }
    const rapidjson::Value& nvme_smart_info_log = document["nvme_smart_health_information_log"];
    if(nvme_smart_info_log.IsObject()){
        const rapidjson::Value& nvme_critical_warning = nvme_smart_info_log["critical_warning"];
        if(nvme_critical_warning.IsUint()){
            int cri_warning = nvme_critical_warning.GetUint();
            nvmeSmartManager.smartInfoMap[disk_char].critical_warning = cri_warning;
            std::cout<<"model critical_warning : "<<cri_warning<<"\n";
        }
        
        const rapidjson::Value& nvme_temp_value = nvme_smart_info_log["temperature"];
        if(nvme_temp_value.IsUint64()){
            int temperature = nvme_temp_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].temperature = temperature;
            std::cout<<"model temperature : "<<temperature<<"\n";
        }

        const rapidjson::Value& nvme_available_value = nvme_smart_info_log["available_spare"];
        if(nvme_available_value.IsUint64()){
            int avail_value = nvme_available_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].available_spare = avail_value;
            std::cout<<"model available_spare : "<<avail_value<<"\n";
        }

        const rapidjson::Value& used_value = nvme_smart_info_log["percentage_used"];
        if(used_value.IsUint64()){
            int percent = used_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].percentage_used = percent;
            std::cout<<"model percentage_used : "<<percent<<"\n";
        }

        const rapidjson::Value& data_units_read_value = nvme_smart_info_log["data_units_read"];
        if(data_units_read_value.IsUint64()){
            int data_units_read = data_units_read_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].data_units_read = data_units_read;
            std::cout<<"model data_units_read : "<<data_units_read<<"\n";
        }

        const rapidjson::Value& data_units_written_value = nvme_smart_info_log["data_units_written"];
        if(data_units_written_value.IsUint64()){
            int data_units_written = data_units_written_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].data_units_written = data_units_written;
            std::cout<<"model data_units_written : "<<data_units_written<<"\n";
        }

        const rapidjson::Value& controller_busy_time_value = nvme_smart_info_log["controller_busy_time"];
        if(controller_busy_time_value.IsUint64()){
            int controller_busy_time = controller_busy_time_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].controller_busy_time = controller_busy_time;
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
            std::cout<<"model power_on_hours : "<<power_on_hours<<"\n";
        }

        const rapidjson::Value& unsafe_shutdowns_value = nvme_smart_info_log["unsafe_shutdowns"];
        if(unsafe_shutdowns_value.IsUint64()){
            int unsafe_shutdowns = unsafe_shutdowns_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].unsafe_shutdowns = unsafe_shutdowns;
            std::cout<<"model unsafe_shutdowns : "<<unsafe_shutdowns<<"\n";
        }

        const rapidjson::Value& media_errors_value = nvme_smart_info_log["media_errors"];
        if(media_errors_value.IsUint64()){
            int media_errors = media_errors_value.GetUint64();
            nvmeSmartManager.smartInfoMap[disk_char].media_errors = media_errors;
            std::cout<<"model media_errors : "<<media_errors<<"\n";
        }
    }
    if(nvmeSmartManager.smartInfoMap[disk_char].percentage_used == 0){
        nvmeSmartManager.smartInfoMap[disk_char].current_capacity =  nvmeSmartManager.smartInfoMap[disk_char].user_capacity * 0.01;
    }
    else{
        nvmeSmartManager.smartInfoMap[disk_char].current_capacity = nvmeSmartManager.smartInfoMap[disk_char].user_capacity * nvmeSmartManager.smartInfoMap[disk_char].percentage_used / 100;
    }
    std::cout<<"model current capacity "<<nvmeSmartManager.smartInfoMap[disk_char].current_capacity<<"\n";

    nvmeSmartManager.smartInfoMap[disk_char].health_score = nvmeSmartManager.smartInfoMap[disk_char].available_spare;
    std::cout<<"model current health_score "<<nvmeSmartManager.smartInfoMap[disk_char].health_score<<"\n";
    if(nvmeSmartManager.smartInfoMap[disk_char].available_spare == 100){
        nvmeSmartManager.smartInfoMap[disk_char].lifespan = nvmeSmartManager.smartInfoMap[disk_char].power_on_hours * 99;
    }

    else{
        nvmeSmartManager.smartInfoMap[disk_char].lifespan = nvmeSmartManager.smartInfoMap[disk_char].power_on_hours*nvmeSmartManager.smartInfoMap[disk_char].available_spare;
    }
    std::cout<<"model current lifespan "<<nvmeSmartManager.smartInfoMap[disk_char].lifespan<<"\n";
    nvmeSmartManager.smartInfoMap[disk_char].fault_chance = 0;
    std::cout<<"model current fault_chance "<<nvmeSmartManager.smartInfoMap[disk_char].fault_chance<<"\n";
    

}
