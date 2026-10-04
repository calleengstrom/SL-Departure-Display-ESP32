#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "../include/wifiSta.h"
#include "../include/httpClient.h"
#include "../include/departureParser.h"
#include "../include/lcd1602.h"
#include "../utills/depature_model.h"


std::string testFunc(std::string word);

WifiSta wifi("{ssid}","{passowrd}");

extern "C" void app_main(void)
{
    LCD1602 lcd(GPIO_NUM_21, GPIO_NUM_22);
    lcd.init();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Starting");
    lcd.setCursor(0, 1);
    lcd.print("Connecting");


    while(!wifi.connect())
    {
        
    }
    lcd.clear();
    lcd.print("Connected !");


    depature_model lcd_text;
    while (1)
    {
        httpClient client;
        std::string url = "https://transport.integration.sl.se/v1/sites/9001/departures?transport=METRO&direction=1&line=14&forecast=20";
        std::string jsonString = client.get_request(url);
        vTaskDelay(pdMS_TO_TICKS(5000));
        if(!jsonString.empty()){
            parseDepartures(jsonString,lcd_text);
        }
        if (lcd_text.bad_name_bool)
        {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Linje "+lcd_text.line +":"+lcd_text.departure);
            lcd.setCursor(0,1);
            lcd.print("Tid : "+lcd_text.expected);
        }
        lcd_text.bad_name_bool = 0;
        lcd_text.temp_string = "";
        lcd_text.departure = "";
        lcd_text.expected = ";";

       vTaskDelay(pdMS_TO_TICKS(30000));
    }
    

}

