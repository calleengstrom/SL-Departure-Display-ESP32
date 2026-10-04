#include "../include/departureParser.h"

#include "cJSON.h"
#include "esp_log.h"
#include <sstream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "../include/lcd1602.h"
static const char *TAG = "SL";

// void check_line(depature_model &lcd_text)
// {
//     std::stringstream ss(lcd_text.temp_string);
//     lcd_text.temp_string = "";

//     std::string departure;
//     std::string expected;
//     std::string line;
//     std::getline(ss, departure, ';');
//     std::getline(ss, expected, ';');
//     std::getline(ss, line, ';');

//     std::stringstream ss_time(expected);
//     std::string year;
//     std::string time;
//     std::getline(ss_time, year, 'T');
//     std::getline(ss_time, time, '\0');

//     lcd_text.departure = departure;
//     lcd_text.expected = time;
//     lcd_text.line = line;
//     lcd_text.bad_name_bool = 1;
// }

void parseDepartures(const std::string &jsonString, depature_model &lcd_text)
{

    cJSON *root =
        cJSON_Parse(jsonString.c_str());

    if (root == nullptr)
    {
        ESP_LOGE(TAG, "JSON parse error");
        return;
    }

    cJSON *departures =
        cJSON_GetObjectItem(root, "departures");

    if (!cJSON_IsArray(departures))
    {
        ESP_LOGE(TAG, "departures är inte en array");

        cJSON_Delete(root);
        return;
    }

    cJSON *departure = nullptr;

    cJSON_ArrayForEach(departure, departures)
    {
        cJSON *destination =
            cJSON_GetObjectItem(departure, "destination");

        cJSON *display =
            cJSON_GetObjectItem(departure, "display");

        cJSON *expected =
            cJSON_GetObjectItem(departure, "expected");

        cJSON *line =
            cJSON_GetObjectItem(departure, "line");

        cJSON *designation = nullptr;

        if (cJSON_IsString(destination))
        {
            ESP_LOGI(
                TAG,
                "Destination: %s",
                destination->valuestring);
        }

        if (cJSON_IsObject(line))
        {
            designation =
                cJSON_GetObjectItem(line, "designation");

            if (cJSON_IsString(designation))
            {
                ESP_LOGI(
                    TAG,
                    "Linje: %s",
                    designation->valuestring);
            }
        }

        if (cJSON_IsString(display))
        {
            ESP_LOGI(
                TAG,
                "Avgår: %s",
                display->valuestring);
        }

        if (cJSON_IsString(expected))
        {
            ESP_LOGI(
                TAG,
                "Expected: %s",
                expected->valuestring);
        }

        ESP_LOGI(TAG, "--------------------");
        
        lcd_text.departure = display->valuestring;
        lcd_text.line = designation->valuestring;
        std::stringstream ss_time(expected->valuestring);
        std::string year;
        std::string time;
        std::getline(ss_time, year, 'T');
        std::getline(ss_time, time, '\0');
        lcd_text.expected = time;

        break;
    }
    cJSON_Delete(root);
}
