# ESP8266 Parking System

An **ESP8266-based parking/entrance access system** using RFID cards, IR sensors, and relays to control a shutter. This system supports **entry and exit validation**, **automatic shutter control**, **buzzer feedback**, and **web-based logging**.



## Features

- Entry and exit control using **RDM6300 RFID readers**
- Vehicle detection using **IR sensor**
- Automatic shutter control (**Up, Down, Stop** relays)
- Buzzer feedback on card scan
- Web-based logging using **HTTP POST** requests
- OTA updates using **AsyncElegantOTA**
- Real-time serial monitoring via **WebSerial**

---

## Components

- ESP8266 (D1 Mini / NodeMCU)
- 2 × RDM6300 RFID Readers
- IR Sensor
- 3 × Relays (Up, Down, Stop)
- Buzzer
- Connecting wires and power supply



## Pin Configuration

| Component        | ESP8266 Pin |
|-----------------|-------------|
| RDM6300 IN       | D1 (GPIO5) |
| RDM6300 OUT      | D2 (GPIO4) |
| IR Sensor        | D5 (GPIO14)|
| Relay Up         | D7 (GPIO13)|
| Relay Down       | D6 (GPIO12)|
| Relay Stop       | D8 (GPIO15)|
| Buzzer           | A0         |


## Wiring Diagram

![Wiring Diagram](diagram/wiring_diagram.png)

*(Connect relays, IR sensor, buzzer, and RFID readers as shown in the diagram.)*



Usage

1. Connect all components according to the wiring diagram.
2. Open the Arduino IDE.
3. Install the following libraries:
   - ESP8266WiFi
   - rdm6300
   - WebSerial
   - ESPAsyncTCP
   - AsyncElegantOTA
   - ESPAsyncWebServer
   - ESPAsyncWiFiManager
   - ESP8266HTTPClient
4. Update the `serverName2` variable with your server URL.
5. Upload the code to ESP8266.
6. Access the WebSerial via your browser to monitor logs.
7. Use **AsyncElegantOTA** for over-the-air updates at `http://<ESP_IP>/update`.


## How it Works

1. Vehicle approaches → IR sensor detects presence → system ready.
2. Scan card at entry (RDM6300 IN):
   - Checks with server
   - Opens shutter if valid
3. Vehicle passes → shutter closes automatically.
4. Scan card at exit (RDM6300 OUT):
   - Checks with server
   - Opens shutter if valid
5. All logs sent to server for tracking.



## License

This project is **open-source**. You can freely use and modify it.

