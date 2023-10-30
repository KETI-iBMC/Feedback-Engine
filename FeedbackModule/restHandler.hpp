#pragma once
#include "stdafx.hpp"
#include <iostream>
#include "nvmeSmartManager.hpp"
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

// void checkSmartRequest();
// std::string getCurrentTime();