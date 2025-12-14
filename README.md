# ESP8266 Parking System

[![License](https://img.shields.io/github/license/hasanarmanabir93-eng/ESP8266-Parking-System)](https://github.com/hasanarmanabir93-eng/ESP8266-Parking-System/blob/main/LICENSE)
[![Stars](https://img.shields.io/github/stars/hasanarmanabir93-eng/ESP8266-Parking-System)](https://github.com/hasanarmanabir93-eng/ESP8266-Parking-System/stargazers)

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Required](#hardware-required)
- [Software / Libraries](#software--libraries)
- [Circuit Diagram](#circuit-diagram)
- [How to Use](#how-to-use)
- [Screenshots](#screenshots)
- [Future Improvements](#future-improvements)
- [License](#license)



## Project Overview
This is an **IoT-based Smart Parking System** using **ESP8266**, designed to monitor and manage parking slots efficiently. The system can detect vehicle entry/exit and update a web interface in real-time.


## Features
- Real-time vehicle detection
- Displays available parking slots
- Web interface for monitoring
- Easy integration with IoT dashboards
- Compact and low-cost setup


## Hardware Required
- ESP8266 (NodeMCU / D1 Mini)
- Ultrasonic Sensor (HC-SR04) for vehicle detection
- LEDs / Display (optional)
- Wires and breadboard
- Power supply (5V)

---

## Software / Libraries
- Arduino IDE
- ESP8266 Board package
- Libraries:
  - `ESP8266WiFi.h`
  - `ESPAsyncWebServer.h`
  - `ESPAsyncTCP.h`
  - Other supporting libraries in the `lib/` folder


## Circuit Diagram
![Circuit Diagram](hardware/Schematic.png)


## How to Use
1. Connect the ESP8266 and sensors as per the schematic.
2. Upload the code using Arduino IDE.
3. Connect ESP8266 to your Wi-Fi.
4. Open the provided web interface to monitor parking slots in real-time.


## Screenshots
![System Screenshot](hardware/Dgithub_ptojectESP8266-Parking-System.png)


## Future Improvements
- Mobile app integration
- Firebase or cloud database support
- Multiple slot management
- SMS/Notification for full parking alerts

## License
This project is licensed under the **MIT License** – see the [LICENSE](LICENSE) file for details.

**Author:** Hasan Arman Abir  
**GitHub:** [hasanarmanabir93-eng](https://github.com/hasanarmanabir93-eng)  
**Contact:** hasanarman.abir93@gmail.com

