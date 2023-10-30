#include "feedbackDbus.hpp"
#include "policyController.hpp"
#include <cstddef>
#include <cstdint>
#include <dbus-c++-1/dbus-c++/types.h>



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

using namespace std;

DBus::BusDispatcher dispatcher;

Feedback_Adaptor::Feedback_Adaptor(DBus::Connection &connection)
    : DBus::ObjectAdaptor(connection, FEEDBACK_SERVER_PATH) {}

int32_t Feedback_Adaptor::feedback_ibmc() {
  cout << "ibmc에서 온 요청 수행 " << endl;
  return 1;
}

int32_t Feedback_Adaptor::feedback_policy() {
  cout << "policy에서 온 요청 수행 : " << endl;
  return 1;
}
int32_t Feedback_Adaptor::feedback_monitor() {
  cout << "monitor에서 온 요청 수행 : " << endl;
  return 1;
}
int32_t Feedback_Adaptor::feedback_energy() {
  cout << "energy에서 온 요청 수행 : " << endl;
  return 1;
}
int32_t Feedback_Adaptor::feedback_ssp() {
  cout << "ssp에서 온 요청 수행 : " << endl;
  return 1;
}

void Feedback_Adaptor::postFOFL(const std::string& url, const std::string& FOFLjson){

  if(url == "/faultAnalysisFoflPolicy"){
    getPolicy(FOFLjson);
  }
}

//-----------------------------Feedback_Adaptor클래스끝-----------------------------
// Ibmc_Proxy::Ibmc_Proxy(DBus::Connection &connection, const char *path,
//                        const char *name)
//     : DBus::ObjectProxy(connection, path, name) {}
//-----------------------------Ibmc_Proxy클래스끝-----------------------------
Policy_Proxy::Policy_Proxy(DBus::Connection &connection, const char *path,
                       const char *name)
    : DBus::ObjectProxy(connection, path, name) {}
//-----------------------------Policy_Proxy클래스끝-----------------------------
Monitor_Proxy::Monitor_Proxy(DBus::Connection &connection, const char *path,
                       const char *name)
    : DBus::ObjectProxy(connection, path, name) {}
//-----------------------------Monitor_Proxy클래스끝-----------------------------
Energy_Proxy::Energy_Proxy(DBus::Connection &connection, const char *path,
                       const char *name)
    : DBus::ObjectProxy(connection, path, name) {}
//-----------------------------Energy_Proxy클래스끝-----------------------------
Ssp_Proxy::Ssp_Proxy(DBus::Connection &connection, const char *path,
                       const char *name)
    : DBus::ObjectProxy(connection, path, name) {}
//-----------------------------Ssp_Proxy클래스끝-----------------------------
void run_feedback_server(){
  std::cout << "feedback Server Start " << std::endl;
  DBus::default_dispatcher = &dispatcher;

  DBus::Connection conn = DBus::Connection::SystemBus();
  conn.request_name(FEEDBACK_SERVER_NAME);

  Feedback_Adaptor server(conn);
  // 서버 유지
  dispatcher.enter();
}
void feedback(){
  string option="";
  /*
  while(true){
    cout << "[ ibmc | get_policy| set_policy | feedback | energy | ssp ] : " << endl;
    cin >> option ;
    if(option =="ibmc"){
      cout << "ibmc에 연결을 요청합니다." << endl;
      connect_to_ibmc_server();
    }
    else if (option == "get_policy" || option == "set_policy_temp" || option == "set_policy_algo" || option == "policy_green" 
    || option == "set_feedback_string"){
      cout << "policy에 연결을 요청합니다." << endl;
      connect_to_policy_server(option);
    }
    else if(option == "feedback"){
      cout << "monitor에 연결을 요청합니다." << endl;
      connect_to_monitor_server();
    }
    else if(option == "energy"){
      cout << "energy에 연결을 요청합니다." << endl;
      connect_to_energy_server();
    }
    else if(option == "ssp"){
      cout << "sspy에 연결을 요청합니다." << endl;
      connect_to_ssp_server();
    }
    else {
      cout << "다시 입력하세요" << endl;
    }
    
  }
  */
}
// void connect_to_ibmc_server(){
//     DBus::BusDispatcher dispatcher;
//   DBus::default_dispatcher = &dispatcher;
//   DBus::Connection conn_n = DBus::Connection::SystemBus();
//   Ibmc_Proxy dbus_adap_test =
//       Ibmc_Proxy(conn_n, IBMC_SERVER_PATH, IBMC_SERVER_NAME);
//   cout << "-------------------------------" << endl;    
//   cout << "ibmc 서버 연결 요청" << endl;
//   cout << "-------------------------------" << endl;
//   dbus_adap_test.ibmc_feedback();
// }


 TempPolicy getTempPolicy(PolicyList option){
  DBus::BusDispatcher dispatcher;
  DBus::default_dispatcher = &dispatcher;
  DBus::Connection conn_n = DBus::Connection::SystemBus();
  Policy_Proxy dbus_adap_test =
      Policy_Proxy(conn_n, POLICY_SERVER_PATH, POLICY_SERVER_NAME);

  TempPolicy returnPolicy;
  ::DBus::Struct<std::string, bool, bool, int32_t, bool, int32_t, bool, int32_t> dbusPolicy;
  dbusPolicy._1 = "none";
  int32_t optionNum = int32_t(option);
  dbusPolicy = dbus_adap_test.getFeedbackPolicy(optionNum);
  if(dbusPolicy._1 == "none"){
    cout<<"Failed to Connect Dbus"<<endl;
  }
  dbusPolicy._2 = returnPolicy.greenActive;
  dbusPolicy._3 = returnPolicy.yellowActive;
  dbusPolicy._4 = returnPolicy.yellowThres;
  dbusPolicy._5 = returnPolicy.orangeActive;
  dbusPolicy._6 = returnPolicy.orangeThres;
  dbusPolicy._7 = returnPolicy.redActive;
  dbusPolicy._8 = returnPolicy.redThres;
  return returnPolicy;
}

void connect_to_policy_server(string option){
  DBus::BusDispatcher dispatcher;
  DBus::default_dispatcher = &dispatcher;
  DBus::Connection conn_n = DBus::Connection::SystemBus();
  Policy_Proxy dbus_adap_test =
      Policy_Proxy(conn_n, POLICY_SERVER_PATH, POLICY_SERVER_NAME);
  cout << "-------------------------------" << endl;    
  cout << "policy 서버 연결 요청" << endl;
  cout << "-------------------------------" << endl;

  // if(option == "get_policy"){
  //   policyInfo policySetting;
  //   ::DBus::Struct< std::string, std::string, std::string, int32_t, std::string > dbusPolicy;
  //   string policyName = "";
  //   cin>> policyName;
  //   dbusPolicy = dbus_adap_test.getFanPolicy(policyName);
  //   policySetting.policyName =dbusPolicy._1;
  //   policySetting.description = dbusPolicy._2;
  //   policySetting.algorithm = dbusPolicy._3;
  //   policySetting.desiredTemp =dbusPolicy._4;
  //   policySetting.sensorSource = dbusPolicy._5;
  //   cout<<policySetting.policyName<<"\n"<<policySetting.description<<"\n"<<policySetting.algorithm<<"\n";
  //   cout<<policySetting.desiredTemp<<"\n"<<policySetting.sensorSource<<"\n";
  // }
  // else if (option == "set_policy_algo"){
  //   string policyName = "", attribute = "", attributeName = "";
  //   cin>> policyName;
  //   cin>> attribute;
  //   cin>>attributeName;
  //   cout<<policyName<<" "<<attribute<<" "<<attributeName<<endl;
  //   int status = 0;
  //   status = dbus_adap_test.setFanPolicyString(policyName, attribute, attributeName);
  //   cout<<"Status : "<<status<<endl;
  // }
  // else if (option == "set_policy_temp"){
  //   string policyName = "", attribute = "";
  //   int attributeValue = 0;
  //   cin>>policyName;
  //   cin>>attribute;
  //   cin>>attributeValue;
  //   cout<<policyName<<" "<<attribute<<" "<<attributeValue<<endl;
  //   int status = 0;
  //   status = dbus_adap_test.setFanPolicyInt(policyName, attribute, attributeValue);
  //   cout<<"Status : "<<status<<endl;
  // }
  // else if (option == "policy_green"){
  //   string policyName ="", tableName =""; 
  //   cin>>tableName;
  //   cin>>policyName;
  //   dbus_adap_test.getFeedbackPolicyGreen(tableName, policyName);
  // }
  // else if (option == "set_feedback_string"){
  //   string tableName = "", policyName ="", attribute ="UpperThresholdUser", attributeName ="80";
  //   cin>>tableName;
  //   cin>>policyName;
  //   dbus_adap_test.setFeedbackPolicyString(tableName, policyName, attribute, attributeName);
  // }
}


void connect_to_monitor_server(){
    DBus::BusDispatcher dispatcher;
  DBus::default_dispatcher = &dispatcher;
  DBus::Connection conn_n = DBus::Connection::SystemBus();
  Monitor_Proxy dbus_adap_test =
      Monitor_Proxy(conn_n, MONITOR_SERVER_PATH, MONITOR_SERVER_NAME);
  cout << "-------------------------------" << endl;    
  cout << "monitor 서버 연결 요청" << endl;
  cout << "-------------------------------" << endl;
  dbus_adap_test.monitor_feedback();
}
void connect_to_energy_server(){
    DBus::BusDispatcher dispatcher;
  DBus::default_dispatcher = &dispatcher;
  DBus::Connection conn_n = DBus::Connection::SystemBus();
  Energy_Proxy dbus_adap_test =
      Energy_Proxy(conn_n, ENERGY_SERVER_PATH, ENERGY_SERVER_NAME);
  cout << "-------------------------------" << endl;    
  cout << "energy 서버 연결 요청" << endl;
  cout << "-------------------------------" << endl;
  dbus_adap_test.emm_feedback();
}
void connect_to_ssp_server(){
    DBus::BusDispatcher dispatcher;
  DBus::default_dispatcher = &dispatcher;
  DBus::Connection conn_n = DBus::Connection::SystemBus();
  Ssp_Proxy dbus_adap_test =
      Ssp_Proxy(conn_n, SSP_SERVER_PATH, SSP_SERVER_NAME);
  cout << "-------------------------------" << endl;    
  cout << "ssp 서버 연결 요청" << endl;
  cout << "-------------------------------" << endl;
  dbus_adap_test.ssp_feedback();
}