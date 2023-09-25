#include "sensorMonitoring.hpp" 
#include "feedbackDbus.hpp"
#include <thread>
int main(){
    std::thread t_sensorReading(readingSensor);
    std::thread feedback_server_thread(run_feedback_server);
    std::thread feedback_thread(feedback);
  //feedback서버 연결 함수 
    feedback_server_thread.join();
    feedback_thread.join();
    t_sensorReading.join();
}