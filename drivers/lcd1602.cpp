#include "../include/lcd1602.h"
#include "esp_rom_sys.h"

LCD1602::LCD1602(gpio_num_t rs, gpio_num_t enable)
{
    rs_pin = rs;
    enable_pin = enable;
}

void LCD1602::init()
{
    gpio_num_t pins[] =
    {
        rs_pin,
        enable_pin,
        D4,
        D5,
        D6,
        D7
    };

    for (gpio_num_t pin : pins)
    {
        gpio_reset_pin(pin);
        gpio_set_direction(pin, GPIO_MODE_OUTPUT);
        gpio_set_level(pin, 0);
    }

    vTaskDelay(pdMS_TO_TICKS(50));

    sendNibble(0x03);
    vTaskDelay(pdMS_TO_TICKS(5));

    sendNibble(0x03);
    esp_rom_delay_us(200);

    sendNibble(0x03);
    esp_rom_delay_us(200);

    sendNibble(0x02);
    esp_rom_delay_us(200);

    command(0x28);
    command(0x0C);
    clear();
    command(0x06);

    esp_rom_delay_us(50);
}

void LCD1602::sendNibble(uint8_t nibble)
{
    gpio_set_level(D4, nibble & 0x01);
    gpio_set_level(D5, nibble & 0x02);
    gpio_set_level(D6, nibble & 0x04);
    gpio_set_level(D7, nibble & 0x08);

    gpio_set_level(enable_pin, 1);
    esp_rom_delay_us(5);

    gpio_set_level(enable_pin, 0);
    esp_rom_delay_us(50);
}

void LCD1602::sendByte(uint8_t value, bool rs)
{
    gpio_set_level(rs_pin, rs);

    sendNibble(value >> 4);
    esp_rom_delay_us(5000);

    sendNibble(value & 0x0F);
    esp_rom_delay_us(5000);
}

void LCD1602::command(uint8_t command)
{
    sendByte(command, false);

    if (command == 0x01 || command == 0x02)
    {
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

void LCD1602::writeChar(char character)
{
    sendByte(character, true);
}

void LCD1602::print(const std::string& text)
{
    for (char c : text)
    {
        writeChar(c);
    }
}

void LCD1602::clear()
{
    command(0x01);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void LCD1602::home()
{
    command(0x02);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void LCD1602::setCursor(uint8_t column, uint8_t row)
{
    uint8_t address = 0x00;

    if (row == 0)
    {
        address = 0x00 + column;
    }
    else
    {
        address = 0x40 + column;
    }

    command(0x80 | address);
}