#pragma once

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string>

class LCD1602
{
private:
    gpio_num_t rs_pin;
    gpio_num_t enable_pin;

    static constexpr gpio_num_t D4 = GPIO_NUM_18;
    static constexpr gpio_num_t D5 = GPIO_NUM_19;
    static constexpr gpio_num_t D6 = GPIO_NUM_23;
    static constexpr gpio_num_t D7 = GPIO_NUM_5;

    void sendNibble(uint8_t nibble);
    void sendByte(uint8_t value, bool rs);

public:
    LCD1602(gpio_num_t rs, gpio_num_t enable);

    void init();

    void command(uint8_t command);
    void writeChar(char character);
    void print(const std::string& text);

    void clear();
    void home();

    void setCursor(uint8_t column, uint8_t row);
};