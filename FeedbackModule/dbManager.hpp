#pragma once
#include <sqlite3.h>
#include <iostream>
#include <fstream> // <fstream> 헤더를 추가합니다.
#include <string>
#include <sstream>
#include <type_traits>
#include <chrono>
#include <thread>
#include <vector>

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

enum ModuleName{
    CPUModule =0,
    MemoryModule= 1,
    StorageModule= 2,
    FanModule= 3,
    CabinetModule= 4,
    OtherModule= 5,
    //Chassis = 6,
};

enum Cause{
    Analyzing=0,
    Temperature_Red=1,
    Usage=2,
    Capacity=3,
    Unknown=4,
    Temperature_Oragnge=5,
    Temperature_Yellow=6,
    Temperature_Green=7,
    //Kernel_Panic = 8,
};

enum Progress{
    Proceeding=0,
    Completed=1,
    Failed=2,
    Alarm=3,
    //SelfRepair = 4,
};




class DatabaseHandler {
public:

    static DatabaseHandler& getInstance(const std::string& dbFile){
        static DatabaseHandler instance(dbFile);
        return instance;
    }


    bool isOpen() const {
        return db != nullptr;
    }

    void createTable(std::string tableName) {
        if (!isOpen()) {
            std::cerr << "Database is not open." << std::endl;
            return;
        }
        const char* createTableSQL = "create table if not exists LogMonitoring (Time TIMESTAMP, ModuleName INTEGER, ModuleNo INTEGER, Cause INTEGER, Progress INTEGER);";
        char* errorMsg = nullptr;

        int rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errorMsg << std::endl;
            sqlite3_free(errorMsg);
        }
        else{
            std::cout<<"\nDatabase Create table"<<std::endl;
        }
    }

    void insertData(const std::string& time, int moduleName, int moduleNo, int cause, int progress) {
        deleteData();
        if (!isOpen()) {
            std::cerr << "\nDatabase is not open." << std::endl;
            return;
        }

        std::stringstream insertSQL;
        insertSQL << "INSERT INTO LogMonitoring (time, ModuleName, ModuleNo, Cause, Progress) VALUES ('" << time << "', "
                  << moduleName << ", " << moduleNo << ", " << cause << ", " << progress << ");";
        char* errorMsg = nullptr;

        int rc = sqlite3_exec(db, insertSQL.str().c_str(), nullptr, nullptr, &errorMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errorMsg << std::endl;
            sqlite3_free(errorMsg);
        }
        else{
            std::cout<<"\nDatabase Insert Data Completed"<<std::endl;
        }
    }

    void deleteData(){
        if (!isOpen()) {
            std::cerr << "\nDatabase is not open." << std::endl;
            return;
        }

        std::string countQuery = "select count(*) from LogMonitoring";
        int rowCount = 0;
        char* errMsg = 0;
        int rc = sqlite3_exec(db, countQuery.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
        if (argc > 0)
            *static_cast<int*>(data) = std::stoi(argv[0]);
        return 0;
    }, &rowCount, &errMsg);

    if(rc != SQLITE_OK){
        std::cerr << "\nSQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return;
    }

    if(rowCount >= 255){
        const char* deleteQuery = "DELETE FROM LogMonitoring WHERE time = (SELECT MIN(time) FROM LogMonitoring)";
        rc = sqlite3_exec(db, deleteQuery, 0, 0, &errMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "\nDeleted the oldest row." << std::endl;
        }
    }
    }

    void updateProgress(int targetModule, int targetNum, int updateValue){
        if (!isOpen()) {
            std::cerr << "\nDatabase is not open." << std::endl;
            return;
        }

        std::stringstream updateSQL;
        updateSQL << "UPDATE LogMonitoring SET progress = "<<updateValue<<" Where Progress = 0 and ModuleName = "<<targetModule<<" and ModuleNo = "<<targetNum<<";";
        char* errorMsg = nullptr;

        int rc = sqlite3_exec(db,updateSQL.str().c_str(),nullptr,nullptr,&errorMsg);
        if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        } else {
            std::cout << "\nDatabase Rows updated successfully." << std::endl;
        }
    }

    std::string lookupData(std::string tableName, std::string condition, std::string order){
        if (!isOpen()) {
            std::cerr << "\nDatabase is not open." << std::endl;
            return nullptr;
        }
        int number = 1;
        rapidjson::Document jsonDoc;
        jsonDoc.SetArray();
        std::stringstream lookupSQL;

        if(condition == "Latest"){
            if(order == "desc"){
                lookupSQL << "select * from "<< tableName <<" order by time desc;";
            }
            else{
                lookupSQL << "select * from "<< tableName <<" order by time;";
            }
        }
        else if(condition == "Module Name"){
            if(order == "desc"){
                lookupSQL << "select * from "<< tableName <<" order by ModuleName desc;";
            }
            else{
                lookupSQL << "select * from "<< tableName <<" order by ModuleName;";
            }
        }

        else if(condition == "Cause of Occurrence"){
            if(order == "desc"){
                lookupSQL << "select * from "<< tableName <<" order by Cause desc;";
            }
            else{
                lookupSQL << "select * from "<< tableName <<" order by Cause;";
            }
        }

        else if(condition == "Result Status"){
            if(order == "desc"){
                lookupSQL << "select * from "<< tableName <<" order by Progress desc;";
            }
            else{
                lookupSQL << "select * from "<< tableName <<" order by Progress;";
            }
        }

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db,lookupSQL.str().c_str(),-1,&stmt,nullptr);

        while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
            rapidjson::Value jsonObj(rapidjson::kObjectType);
            jsonObj.AddMember("NO", number, jsonDoc.GetAllocator());

            const char* timestampStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            rapidjson::Value timestampValue;
            timestampValue.SetString(timestampStr, jsonDoc.GetAllocator()); // 이것으로 변환된 문자열을 저장
            jsonObj.AddMember("TIME", timestampValue, jsonDoc.GetAllocator());
            
            rapidjson::Value moduleValue;
            moduleValue.SetString(getModuleName(sqlite3_column_int(stmt,1)).c_str(), jsonDoc.GetAllocator());
            jsonObj.AddMember("MODULE_NAME",moduleValue,jsonDoc.GetAllocator());

            jsonObj.AddMember("MODULE_NO",sqlite3_column_int(stmt,2),jsonDoc.GetAllocator());

            rapidjson::Value causeValue;
            causeValue.SetString(getCause(sqlite3_column_int(stmt,3)).c_str(), jsonDoc.GetAllocator());
            jsonObj.AddMember("CAUSE", causeValue,jsonDoc.GetAllocator());

            rapidjson::Value alarmValue;
            alarmValue.SetString(getProgress(sqlite3_column_int(stmt,4)).c_str(), jsonDoc.GetAllocator());
            jsonObj.AddMember("PROGRESS", alarmValue,jsonDoc.GetAllocator());

            jsonDoc.PushBack(jsonObj, jsonDoc.GetAllocator());
            number++;
        }

        rapidjson::StringBuffer jsonBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
        jsonDoc.Accept(jsonWriter);
        std::string jsonString = jsonBuffer.GetString();

        // JSON 문자열 출력
        //std::cout << jsonString << std::endl;

        sqlite3_finalize(stmt);   
        return jsonString;     
    }

/*
    std::string lookupData(std::string tableName,int num,int page, std::string condition, std::string order){
        if (!isOpen()) {
            std::cerr << "Database is not open." << std::endl;
            return nullptr;
        }

        rapidjson::Document jsonDoc;
        jsonDoc.SetArray();
        int number = ((page-1) * num)+1;
        std::stringstream lookupSQL;
        int limit = num;
        int offset = num * (page-1);
        if(condition == "Latest"){
            if(order == "desc"){
                lookupSQL << "select * from "<< tableName <<" order by time desc limit "<<num<<" offset "<<offset<<";";
            }
            else{
                lookupSQL << "select * from "<< tableName <<" order by time limit "<<num<<" offset "<<offset<<";";
            }
        }
        else if(condition == "Module Name"){
            if(order == "desc"){
                lookupSQL << "select * from "<< tableName <<" order by ModuleName desc limit "<<num<<" offset "<<offset<<";";
            }
            else{
                lookupSQL << "select * from "<< tableName <<" order by ModuleName limit "<<num<<" offset "<<offset<<";";
            }
        }

        else if(condition == "Cause of Occurrence"){
            if(order == "desc"){
                lookupSQL << "select * from "<< tableName <<" order by Cause desc limit "<<num<<" offset "<<offset<<";";
            }
            else{
                lookupSQL << "select * from "<< tableName <<" order by Cause limit "<<num<<" offset "<<offset<<";";
            }
        }

        else if(condition == "Result Status"){
            if(order == "desc"){
                lookupSQL << "select * from "<< tableName <<" order by Progress desc limit "<<num<<" offset "<<offset<<";";
            }
            else{
                lookupSQL << "select * from "<< tableName <<" order by Progress limit "<<num<<" offset "<<offset<<";";
            }
        }
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db,lookupSQL.str().c_str(),-1,&stmt,nullptr);

        while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
            rapidjson::Value jsonObj(rapidjson::kObjectType);
            jsonObj.AddMember("NO", number, jsonDoc.GetAllocator());


            const char* timestampStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            rapidjson::Value timestampValue;
            timestampValue.SetString(timestampStr, jsonDoc.GetAllocator()); // 이것으로 변환된 문자열을 저장
            jsonObj.AddMember("TIME", timestampValue, jsonDoc.GetAllocator());
            

            jsonObj.AddMember("MODULE_NAME", getModuleName(sqlite3_column_int(stmt,1)),jsonDoc.GetAllocator());
            jsonObj.AddMember("MODULE_NO",sqlite3_column_int(stmt,2),jsonDoc.GetAllocator());
            jsonObj.AddMember("CAUSE", getCause(sqlite3_column_int(stmt,3)),jsonDoc.GetAllocator());
            jsonObj.AddMember("PROGRESS", getProgress(sqlite3_column_int(stmt,4)),jsonDoc.GetAllocator());

            jsonDoc.PushBack(jsonObj, jsonDoc.GetAllocator());
            number++;
        }

        rapidjson::StringBuffer jsonBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
        jsonDoc.Accept(jsonWriter);
        std::string jsonString = jsonBuffer.GetString();

        // JSON 문자열 출력
        std::cout << jsonString << std::endl;

        sqlite3_finalize(stmt);   
        return jsonString;     
    }
    */

    std::string getModuleName(int value){
        std::string return_str;
        switch(value){
            case 0:
                return_str = "CPU";
                break;
            case 1:
                return_str = "Memory";
                break;
            case 2:
                return_str = "Storage";
                break;
            case 3:
                return_str = "Fan";
                break;
            case 4:
                return_str = "Cabintet";
                break;
            case 5:
                return_str = "Others";
                break;
            // case 6:
            //     return_str = "Chassis";
            //     break;
        }
        return return_str;
    }

    std::string getCause(int value){
        std::string return_str;
        switch(value){
            case 0:
                return_str = "Analyzing";
                break;
            case 1:
                return_str = "Temperature_Red";
                break;
            case 2:
                return_str = "Usage";
                break;
            case 3:
                return_str = "Capacity";
                break;
            case 4:
                return_str = "Unknown";
                break;
            case 5:
                return_str = "Temperature_Orange";
                break;
            case 6:
                return_str = "Temperature_Yellow";
                break;
            case 7:
                return_str = "Temperature_Green";
                break;
            // case 8:
            //     return_str = "Kernel_Panic";
            //     break;
        }
        return return_str;
    }
    
    std::string getProgress(int value){
        std::string return_str;
        switch(value){
            case 0:
                return_str = "Proceeding";
                break;
            case 1:
                return_str = "Completed";
                break;
            case 2:
                return_str = "Failed";
                break;
            case 3:
                return_str = "Alarm";
                break;
            // case 4:
            //     return_str = "Self_Repair";
            //     break;
        }
        return return_str;
    }


private:
    DatabaseHandler(const std::string& dbFile) : DB_FILE(dbFile), db(nullptr) {
        // DB 연결 초기화 로직
        int rc = sqlite3_open(DB_FILE.c_str(), &db);
        if (rc) {
            std::cerr << "\nCan't open database: " << sqlite3_errmsg(db) << std::endl;
            std::cout << "\nDatabase Handler Create Database"<<std::endl;
            const std::string command = "sqlite3 "+ DB_FILE;
            std::system(command.c_str());
            int check = sqlite3_open(DB_FILE.c_str(), &db);
            if(check){
                std::cerr << "\nCan't open database: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    }

    DatabaseHandler(const DatabaseHandler&) = delete;
    DatabaseHandler& operator=(const DatabaseHandler&) = delete;

    ~DatabaseHandler() {
        if (db) {
            sqlite3_close(db);
        }
    }

    const std::string DB_FILE;
    sqlite3* db;
};

void InitDatabase();

