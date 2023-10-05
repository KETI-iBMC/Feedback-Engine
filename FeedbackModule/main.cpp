#include "sensorMonitoring.hpp" 
#include "feedbackDbus.hpp"
#include "policyController.hpp"
#include <thread>
int main(){
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