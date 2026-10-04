#pragma once
#include "esp_http_client.h"
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <cJSON.h>
class httpClient
{
private:
    std::string json_response;

    static esp_err_t http_event_handler(esp_http_client_event_t *evt);
public:
    httpClient();


    std::string get_request(const std::string& api_endpoint);

    std::string get_json_result(const std::string& api_endpoint);

   
};




