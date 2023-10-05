#ifndef __DEMO_ECHO_SERVER_H
#define __DEMO_ECHO_SERVER_H

#include <cstdint>
#include <dbus-c++-1/dbus-c++/dbus.h>
#include <dbus-c++-1/dbus-c++/message.h>

#include <dbus-c++-1/dbus-c++/interface.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <thread>
#include <iostream>


#include <string>
#include "../server_path"
#include "feedback_adaptor.h"
#include "../KETI-IBMC-YJ/ibmc_proxy.h"
#include "./include/dbus/policy_proxy.h"
#include "../Monitor/monitor_proxy.h"
#include "./include/dbus/energy_proxy.hpp"
#include "../SSP/ssp_proxy.h"
#include "policyController.hpp"


struct policyInfo{
  std::string policyName;
  std::string description;
  std::string algorithm;
  int32_t desiredTemp;
  std::string sensorSource;
};
class Feedback_Adaptor : public org::freedesktop::keti::bmc::feedback_adaptor,
                         public DBus::IntrospectableAdaptor,
                         public DBus::ObjectAdaptor

{
 public:
  Feedback_Adaptor(DBus::Connection& connection);

 public:
  int32_t feedback_policy();
  int32_t feedback_ibmc();
  int32_t feedback_monitor();
  int32_t feedback_energy();
  int32_t feedback_ssp();

};

class Ibmc_Proxy : public org::freedesktop::keti::bmc::ibmc_proxy,
                   public DBus::IntrospectableProxy,
                   public DBus::ObjectProxy {
 public:
  Ibmc_Proxy(DBus::Connection &connection, const char *path,
                   const char *name);
  ~Ibmc_Proxy(){};
};

class Policy_Proxy : public org::freedesktop::keti::bmc::policy_proxy,
                   public DBus::IntrospectableProxy,
                   public DBus::ObjectProxy {
 public:
  Policy_Proxy(DBus::Connection &connection, const char *path,
                   const char *name);
  ~Policy_Proxy(){};
};
class Monitor_Proxy : public org::freedesktop::keti::bmc::monitor_proxy,
                   public DBus::IntrospectableProxy,
                   public DBus::ObjectProxy {
 public:
  Monitor_Proxy(DBus::Connection &connection, const char *path,
                   const char *name);
  ~Monitor_Proxy(){};
};
class Energy_Proxy : public org::freedesktop::keti::bmc::energy_proxy,
                   public DBus::IntrospectableProxy,
                   public DBus::ObjectProxy {
 public:
  Energy_Proxy(DBus::Connection &connection, const char *path,
                   const char *name);
  ~Energy_Proxy(){};
};
class Ssp_Proxy : public org::freedesktop::keti::bmc::ssp_proxy,
                   public DBus::IntrospectableProxy,
                   public DBus::ObjectProxy {
 public:
  Ssp_Proxy(DBus::Connection &connection, const char *path,
                   const char *name);
  ~Ssp_Proxy(){};
};
void connect_to_ibmc_server();
void connect_to_policy_server(int option1, int option2);
void connect_to_monitor_server();
void connect_to_energy_server();
void connect_to_ssp_server();

TempPolicy getTempPolicy(PolicyList option);

void run_feedback_server();
void feedback();

#endif  //__DEMO_ECHO_SERVER_H