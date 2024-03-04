#pragma once
#include "stdafx.hpp"
#include <iostream>
#include "nvmeSmartManager.hpp"
#include <chrono>
#include <climits>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#define MAX_REQ 98
#define MAX_RES 200

struct real_data {
  int cmd;
  unsigned char data[MAX_RES];
};
struct rq_real_data {
  int cmd;
  unsigned char data[MAX_REQ];
};

struct OEM_RES_buf {
  long msgtype;
  real_data data;
};

struct OEM_REQ_buf {
  long msgtype;
  real_data data;
};


//#include "kernelPanicCheck.hpp"
using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class Rest_Handler_Instance{
    public:
        Rest_Handler_Instance(utility::string_t url);

        pplx::task<void>open(){return m_listener.open();}
        pplx::task<void>close(){return m_listener.close();}

    private:
        void handle_get(http_request message);
        void handle_put(http_request message);
        void handle_post(http_request message);
        void handle_delete(http_request message);
        void handle_error(pplx::task<void>& t);
        http_listener m_listener;
};

void messageQueueHandler();
void getMessageData(OEM_RES_buf resData);
void messageQueueRequest(int msgType, unsigned char* data);
// void checkSmartRequest();
// std::string getCurrentTime();