#include "sensorMonitoring.hpp" 
#include "feedbackDbus.hpp"
#include "policyController.hpp"
#include "restHandler.hpp"
#include <thread>
#include "stdafx.hpp"
#include "nvmeSmartManager.hpp"

std::unique_ptr<Rest_Handler_Instance> restHandler;

int main(){

  utility::string_t port = U("40000");
  utility::string_t address = U("http://0.0.0.0:");
  address.append(port);
  web::uri_builder uri(address);  
  auto addr = uri.to_uri().to_string();
  restHandler = std::unique_ptr<Rest_Handler_Instance>(new Rest_Handler_Instance(addr));
  restHandler -> open().wait();
  ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

    std::thread t_sensorReading(readingSensor);
    std::thread feedback_server_thread(run_feedback_server);
    std::thread policy_thread(readPolicy);
    //feedback서버 연결 함수 
    feedback_server_thread.join();
    std::cout<<"Thread 1 Start\n";
    t_sensorReading.join();
    std::cout<<"Thread 3 Start\n";
    policy_thread.join();
    std::cout<<"Thread 4 Start\n";
}