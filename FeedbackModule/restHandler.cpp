#include "restHandler.hpp"


#include "thread"

using namespace rapidjson;

Rest_Handler_Instance::Rest_Handler_Instance(utility::string_t url):m_listener(url){
    cout << "url: " << url << endl;
    m_listener.support(methods::GET, std::bind(&Rest_Handler_Instance::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&Rest_Handler_Instance::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&Rest_Handler_Instance::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&Rest_Handler_Instance::handle_delete, this, std::placeholders::_1));
}

void Rest_Handler_Instance::handle_get(http_request message){

}

void Rest_Handler_Instance::handle_put(http_request message){

}

void Rest_Handler_Instance::handle_post(http_request message){
    std::string uri_path = message.relative_uri().to_string();
    cout<<uri_path<<endl;

    auto body_json = message.extract_string();
    std::string json = utility::conversions::to_utf8string(body_json.get());
    cout << json << endl;

    //Document document;
    //document.Parse(json.c_str());

    if(uri_path == "/smart"){
        cout<<"smart post request"<<endl;
        getSmartInfo(json);
    }
    cout<<"Sending ok"<<endl;
    message.reply(status_codes::OK);
}

 void Rest_Handler_Instance::handle_delete(http_request message){
 }
