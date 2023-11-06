#include "kernelPanicCheck.hpp"
#include "dbManager.hpp"

bool kernel = false;
bool smart = false;

void checkKernel(){
    int panicCount = 2;
    kernel = false;
    smart =false;
    while(true){
        std::this_thread::sleep_for(30);
        std::string timestamp = getTime();
        if(smart == true && kernel ==false){ // 정상작동중
            smart = false;
            panicCount = 2;
        }
        else if(smart ==true && kernel == true){ // 호스트 복구
            smart = false;
            kernel = false;
            std::cout<<"HOST KERNEL PANIC RECOVER";
            DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp,6,0,8,1);
            panicCount = 2;
        }
        else{ //호스트 죽음
            std::cout<<"\nHOST KERNEL PANIC\n";
            if(panicCount >= 2){
                DatabaseHandler::getInstance("/conf/feedback.db").insertData(timestamp,6,404,8,4);
                panicCount = 0;
            }
            panicCount += 1;
            kernel = true;
            smart = false;
        }
    }
}



std::string getTime(){
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    char timestamp[20];  // 충분한 공간을 확보합니다.

    // 형식 지정을 통해 timestamp 문자열 생성
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    return std::string(timestamp);
}

void setSmart(bool status){
    smart = status;
}

void setKernel(bool status){
    kernel = status;
}