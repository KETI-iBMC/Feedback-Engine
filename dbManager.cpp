#include "dbManager.hpp"


void InitDatabase(){
    DatabaseHandler::getInstance("/conf/feedback.db").createTable("LogMonitoring");

    while(1){
        std::string latestLog;
        std::string moduleNameLog;
        std::string causeLog;
        std::string proceedLog;
        latestLog = DatabaseHandler::getInstance("/conf/feedback.db").lookupData("LogMonitoring","Latest","desc");
        moduleNameLog = DatabaseHandler::getInstance("/conf/feedback.db").lookupData("LogMonitoring","Module Name","asc");
        causeLog = DatabaseHandler::getInstance("/conf/feedback.db").lookupData("LogMonitoring","Cause of Occurrence","asc");
        proceedLog = DatabaseHandler::getInstance("/conf/feedback.db").lookupData("LogMonitoring","Result Status","asc");
        
        
        std::string moduleFilePath = "/conf/feedbackLog_module.json";
        std::string causeFilePath = "/conf/feedbackLog_cause.json";
        std::string proceedFilePath = "/conf/feedbackLog_proceed.json";
        std::string latestFilePath = "/conf/feedbackLog_latest.json";
        std::ofstream outputFile1(latestFilePath);

        if(outputFile1.is_open()){
            outputFile1 << latestLog;
            outputFile1.close();
            std::cout<<"\nSUCCESS TO STORE LATEST_LOG DATA"<<std::endl;
        }
        else{
            std::cerr<<"\nFAILED TO STORE LATEST_LOG DATA"<<std::endl;
        }        

        std::ofstream outputFile2(moduleFilePath);

        if(outputFile2.is_open()){
            outputFile2 << moduleNameLog;
            outputFile2.close();
            std::cout<<"\nSUCCESS TO STORE MODULE_LOG DATA"<<std::endl;
        }
        else{
            std::cerr<<"\nFAILED TO STORE MODULE_LOG DATA"<<std::endl;
        }        


        std::ofstream outputFile3(causeFilePath);

        if(outputFile3.is_open()){
            outputFile3 << causeLog;
            outputFile3.close();
            std::cout<<"\nSUCCESS TO STORE CAUSE_LOG DATA"<<std::endl;
        }
        else{
            std::cerr<<"\nFAILED TO STORE CAUSE_LOG DATA"<<std::endl;
        }        


        std::ofstream outputFile4(proceedFilePath);

        if(outputFile4.is_open()){
            outputFile4 << proceedLog;
            outputFile4.close();
            std::cout<<"\nSUCCESS TO STORE PROCEED_LOG DATA"<<std::endl;
        }
        else{
            std::cerr<<"\nFAILED TO STORE PROCEED_LOG DATA"<<std::endl;
        }        
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
}

