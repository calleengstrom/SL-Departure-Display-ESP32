#include "../include/WifiSta.h"

#include <string.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

static const char* TAG = "WifiSta";

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static EventGroupHandle_t wifiEventGroup = nullptr;

WifiSta::WifiSta(
    const std::string& ssid,
    const std::string& password
)
    : ssid(ssid),
      password(password)
{
}

bool WifiSta::connect()
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();

    wifiEventGroup = xEventGroupCreate();

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&config);

    esp_event_handler_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &WifiSta::eventHandler,
        this
    );

    esp_event_handler_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &WifiSta::eventHandler,
        this
    );

    wifi_config_t wifiConfig = {};

    strcpy(
        reinterpret_cast<char*>(wifiConfig.sta.ssid),
        ssid.c_str()
    );

    strcpy(
        reinterpret_cast<char*>(wifiConfig.sta.password),
        password.c_str()
    );

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifiConfig);

    esp_wifi_start();

    EventBits_t bits = xEventGroupWaitBits(
        wifiEventGroup,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdTRUE,
        pdFALSE,
        pdMS_TO_TICKS(10000)
    );

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "WiFi connection successful");
        return true;
    }

    ESP_LOGW(TAG, "WiFi connection failed");
    return false;
}

void WifiSta::eventHandler(
    void* arg,
    esp_event_base_t eventBase,
    int32_t eventId,
    void* eventData
)
{
    auto* wifi = static_cast<WifiSta*>(arg);

    wifi->handleEvent(
        eventBase,
        eventId,
        eventData
    );
}

void WifiSta::handleEvent(
    esp_event_base_t eventBase,
    int32_t eventId,
    void* eventData
)
{
    if (eventBase == WIFI_EVENT &&
        eventId == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }

    if (eventBase == WIFI_EVENT &&
        eventId == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGW(TAG, "WiFi disconnected");

        xEventGroupSetBits(
            wifiEventGroup,
            WIFI_FAIL_BIT
        );
    }

    if (eventBase == IP_EVENT &&
        eventId == IP_EVENT_STA_GOT_IP)
    {
        auto* event =
            static_cast<ip_event_got_ip_t*>(eventData);

        ESP_LOGI(
            TAG,
            "Connected! IP: " IPSTR,
            IP2STR(&event->ip_info.ip)
        );

        ESP_LOGI(
            TAG,
            "SSID: %s",
            ssid.c_str()
        );

        xEventGroupSetBits(
            wifiEventGroup,
            WIFI_CONNECTED_BIT
        );
    }
}