#include "restHandler.hpp"
#include <sys/ipc.h>
#include <thread>
#include "dbManager.hpp"
#include <chrono>


using namespace rapidjson;
bool isSmartRequest = false;
bool isKernelPanic = false;
int panicCount = 6;
Rest_Handler_Instance::Rest_Handler_Instance(utility::string_t url):m_listener(url){
    cout << "url: " << url << endl;
    m_listener.support(methods::GET, std::bind(&Rest_Handler_Instance::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&Rest_Handler_Instance::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&Rest_Handler_Instance::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&Rest_Handler_Instance::handle_delete, this, std::placeholders::_1));
}
//
void Rest_Handler_Instance::handle_get(http_request message){

}

void Rest_Handler_Instance::handle_put(http_request message){

}
//123
void getMessageData(OEM_RES_buf resData){

    if(resData.msgtype == 1){
        for(int i=0;i<MAX_RES;i++){
            if(resData.data.data[i] != 0)
            std::cout<<resData.data.data[i]<<std::endl;
        }
    }
    else if(resData.msgtype == 2){
        for(int i=0;i<MAX_RES;i++){
            if(resData.data.data[i] != 0)
            std::cout<<resData.data.data[i]<<std::endl;
        }
    }
    else if(resData.msgtype == 3){
        for(int i=0;i<MAX_RES;i++){
            if(resData.data.data[i] != 0)
            std::cout<<resData.data.data[i]<<std::endl;
        }
    }
    else{
        perror("UNVAILED MESSAGE TYPE\n");
    }
}

void messageQueueHandler(){
    while(1){
        key_t key = 7002;
        int msqid ;
        OEM_RES_buf msg;
        if((msqid = msgget(key, IPC_CREAT | 0666)) == -1){
            perror("MESSAGE RECEIVE FAILED12\n");
        }

        if(msgrcv(msqid, &msg, sizeof(struct real_data),0,0) == -1){
            perror("MESSAGE RECEIVE FAILED\n");
        }

        getMessageData(msg);
    }
}

void messageQueueRequest(int msgType, unsigned char* data){

}


std::string getCurrentTime(){
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    char timestamp[20];  // 충분한 공간을 확보합니다.

    // 형식 지정을 통해 timestamp 문자열 생성
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    return std::string(timestamp);
}

void Rest_Handler_Instance::handle_post(http_request message){
    std::string uri_path = message.relative_uri().to_string();
    cout<<uri_path<<endl;

    auto body_json = message.extract_string();
    std::string json = utility::conversions::to_utf8string(body_json.get());
    cout << json << endl;

    if(uri_path == "/smartfan"){
        std::string filePath = "/conf/lfc.json";
        std::ofstream outputFile(filePath);
        if(outputFile.is_open()){
           outputFile << json;
            outputFile.close();
           std::cout<<"Success Store lfc.json Data"<<std::endl;
        }
        else{
           std::cerr<<"Failed to Store lfc.json Data"<<std::endl;
        }
        cout<<"lfc post request"<<endl;
    }

    else if(uri_path == "/lstm"){
        std::string filePath = "/conf/lstm.json";
        std::ofstream outputFile(filePath);
        if(outputFile.is_open()){
            outputFile<<json;
            outputFile.close();
            std::cout<<"Sucess Store lstm.json Data"<<std::endl;
        }
        else{
           std::cerr<<"Failed to Store lstm.json Data"<<std::endl;
        }
        cout<<"lstm post request"<<endl;
    }
    else if(uri_path == "/diskGraph"){
        std::string filePath = "/conf/diskGraph.json";
        std::ofstream outputFile(filePath);
        if(outputFile.is_open()){
            outputFile<<json;
            outputFile.close();
            std::cout<<"Success store diskGraph.json Data"<<std::endl;
        }
        else{
            std::cout<<"Failed store diskGraph.json Data"<<std::endl;
        }
    }

    else if(uri_path == "/smart"){
    //파일 저장
        //setSmart(true);

        std::string filePath = "/conf/smart.json";
        std::ofstream outputFile(filePath);

        if(outputFile.is_open()){
           outputFile << json;
            outputFile.close();
           std::cout<<"\nSuccess Store Json Data"<<std::endl;
        }
        else{
           std::cerr<<"Failed to Store Smart Data"<<std::endl;
        }
        getSmartInfo(json);
    }


    //Document document;
    //document.Parse(json.c_str());
    cout<<"\nRECEIVE SMART DATA"<<endl;
    message.reply(status_codes::OK);
}

 void Rest_Handler_Instance::handle_delete(http_request message){
 }
