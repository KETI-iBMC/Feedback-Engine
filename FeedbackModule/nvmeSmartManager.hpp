enum diskType{
    hdd,
    sdd,
    nvme
};

struct critical_Warning{
    int total_count;
    int available_spare;
    int temp_threshold;
    int nvme_subsystem;
    int read_only;
    int volatile_mem_backup;
    int persistent_mem;
};

class nvmeSmartInfo{
    public:
    diskType diskType;
    int diskNum;
    critical_Warning critical_Warning;
    int temperature;
    int available_spare;
    int available_spare_threshold;
    int percentage_used;
    int data_units_read;
    int data_units_written;
    int host_read_commands;
    int host_write_commands;
    int controller_busy_time;
    int power_cycles;
    int power_on_hours;
    int unsafe_shutdowns;
    int media_error;
    int num_err_log_entries;
    int warning_temperature_time;
    int critical_composite_temperature_time;
    int temperature_sensor1;
    int temperature_sensor2;
    int temperature_sensor3;
    int temperature_sensor4;
};

class nvmeSmartManager{
    public: 
};