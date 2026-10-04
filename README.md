

# SL Departure Display – ESP32

A small **hobby MVP project** using an ESP32 to fetch departure data from SL and display the next departure on a 1602A LCD.

The project connects to Wi-Fi, sends an HTTP GET request, parses the JSON response with cJSON, and displays selected departure information.

## SL API

The departure API is available through Trafiklab:

https://www.trafiklab.se/sv/api/other-apis/sl/transport/#/default/Departures

Create or configure your API request there.

Example used in this project:

```text
https://transport.integration.sl.se/v1/sites/9001/departures?transport=METRO&direction=1&line=14&forecast=20
```

Important parameters:

```text
site:       9001
transport:  METRO
direction:  1
line:       14
forecast:   20
```

`direction` is used to select departures travelling in a specific direction and may need to be changed depending on the station and line.

## Hardware

- ESP32
- LCD 1602A / HD44780
- Jumper wires
- Potentiometer for LCD contrast

LCD connections used in this project:

```text
RS -> GPIO 21
E  -> GPIO 22

D4 -> GPIO 18
D5 -> GPIO 19
D6 -> GPIO 23
D7 -> GPIO 5

RW  -> GND
VSS -> GND
```

## Project Flow

```text
ESP32
  ↓
Wi-Fi
  ↓
SL HTTP API
  ↓
JSON
  ↓
cJSON parser
  ↓
LCD 1602A
```

## Built With

- C++
- ESP-IDF
- ESP32
- cJSON
- SL Transport API

This is mainly a small hobby MVP for experimenting with ESP32, HTTP requests, JSON parsing and LCD communication.

<img width="1440" height="687" alt="sl_lcd" src="https://github.com/user-attachments/assets/6d623402-3066-4e8a-8a19-d280964e74f9" />
