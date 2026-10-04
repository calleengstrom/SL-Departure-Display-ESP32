#pragma once
#include <esp_wifi.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <esp_log.h>
#include <string>
#include "esp_event.h"


class WifiSta
{
public:
    WifiSta(const std::string& ssid, const std::string& password);

    bool connect();

private:
    std::string ssid;
    std::string password;

    static void eventHandler(
        void* arg,
        esp_event_base_t eventBase,
        int32_t eventId,
        void* eventData
    );

    void handleEvent(
        esp_event_base_t eventBase,
        int32_t eventId,
        void* eventData
    );
};
