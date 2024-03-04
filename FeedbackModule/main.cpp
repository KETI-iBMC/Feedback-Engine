#include "sensorMonitoring.hpp" 
#include "feedbackDbus.hpp"
#include "policyController.hpp"
#include "restHandler.hpp"
#include <thread>
#include "stdafx.hpp"
#include "nvmeSmartManager.hpp"
#include "foflManager.hpp"
#include "foflPredict.hpp"

std::unique_ptr<Rest_Handler_Instance> restHandler;

int main(){

  utility::string_t port = U("40000");
  utility::string_t address = U("http://0.0.0.0:");
  address.append(port);
  web::uri_builder uri(address);  
  auto addr = uri.to_uri().to_string();
  restHandler = std::unique_ptr<Rest_Handler_Instance>(new Rest_Handler_Instance(addr));
  restHandler -> open().wait();
  //ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
  std::thread messageQueue_thread(messageQueueHandler);
  std::this_thread::sleep_for(std::chrono::seconds(5));
    std::thread saveJson_thread(InitDatabase);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::thread policy_thread(initPolicy);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::thread t_sensorReading(readingSensor);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::thread predict_thread(initPredictFOFL);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::thread rest_thread(checkSmartRequest);
    messageQueue_thread.join();
    saveJson_thread.join();
    policy_thread.join();
    t_sensorReading.join();
    predict_thread.join();
    //rest_thread.join();
}

