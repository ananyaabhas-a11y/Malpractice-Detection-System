# Malpractice Detection System

A low-cost, portable, and privacy-preserving embedded system that detects unauthorized wireless devices in examination halls using ESP32, Wi-Fi, Bluetooth Low Energy (BLE), and RF signal analysis. The system identifies active wireless communication in real time and alerts invigilators through visual and audible indicators, helping maintain examination integrity without relying on cameras, microphones, or internet connectivity.

<img width="1280" height="577" alt="Project Prototype" src="https://github.com/user-attachments/assets/a504484f-65f2-470c-8e73-f42f907e1a7f" />


## Overview

Traditional methods of preventing examination malpractice, such as manual inspection, metal detectors, and AI-based proctoring, either lack accuracy, raise privacy concerns, or require expensive infrastructure.

This project proposes a hardware-based solution that continuously monitors the surrounding RF environment to detect wireless communication from smartphones, smartwatches, wireless earbuds, and similar devices. By combining **Wi-Fi scanning**, **BLE detection**, and **RF signal sensing**, the system can identify suspicious wireless activity while minimizing false alarms through intelligent filtering techniques.

## Features

*  Real-time Wi-Fi device detection
*  Bluetooth Low Energy (BLE) scanning
*  RF signal monitoring (800 MHz – 2.5 GHz)
*  RSSI and threshold-based detection
*  MAC address whitelisting for authorized devices
*  LED and buzzer alerts
*  LCD status display
*  Low-cost and portable hardware
*  Privacy-preserving (no cameras or microphones)
* Scalable for deployment in large examination halls


## Hardware Used

* ESP32 Development Board
* RF Detector Module ( for simulation purposes, wirewound potentiometer was used )
* SMA Antenna
* 16×2 LCD ( without I2C display)
* LEDs
* Buzzers
* Tactile Switches
* Breadboard & Connecting Wires
* Power Supply ( min 5V, from our pc)


## Software & Tools

* Arduino IDE
* ESP32 Arduino Libraries
* MATLAB (Signal Analysis & Simulation)
* Wokwi Simulator
* C/C++ ( Embedded C )


## Working Principle

1. The ESP32 continuously scans for nearby Wi-Fi and BLE devices.
2. The RF sensor monitors electromagnetic activity in the 800 MHz–2.5 GHz frequency range.
3. Signal strength (RSSI) and RF intensity are compared against configurable threshold values.
4. MAC address whitelisting ignores authorized devices (e.g., invigilators' phones).
5. Persistent signals exceeding the threshold trigger:

   * LED indication
   * Buzzer alarm
   * LCD notification
6. The system continues monitoring in real time.

## System architecture

<img width="880" height="398" alt="System Architecture" src="https://github.com/user-attachments/assets/3630b8ec-1286-46a8-8297-96796f2ab7be" />


## Detection Capabilities

The system is capable of detecting:

* Smartphones
* Smartwatches
* Bluetooth Earbuds
* Fitness Bands
* Other Wi-Fi/BLE-enabled devices
* Cellular RF transmissions


## Advantages

* Real-time wireless activity detection
* Low-cost implementation
* Portable design
* No internet connectivity required
* No privacy-invasive surveillance
* Reduced false positives through filtering and whitelisting
* Easy deployment in educational institutions


## Current Limitations

* Cannot detect powered-off or radio-silent devices
* Limited detection range
* Cannot precisely locate detected devices ( limited Localization )
* Environmental RF noise may affect accuracy
* Nearby rooms may generate interference


## Future Improvements

* Machine Learning-based signal classification
* Device identification
* Distance estimation using RSSI
* Multi-node deployment
* IoT-based centralized monitoring dashboard
* Improved RF sensitivity
* Zone-based alerts


## Repository Structure

<img width="233" height="145" alt="workflow" src="https://github.com/user-attachments/assets/baef3318-8208-4265-b196-f047c4e7187c" />


## Applications

* Educational Institutions
* Competitive Examination Centers
* Corporate Certification Tests
* Secure Examination Facilities
* Confidential Meeting Rooms
* Restricted Research Laboratories


## Authors

Gaurav
Department of Electronics & Communication Engineering
Indian Institute of Information Technology, Kota

Ananya Singh
Department of Electronics & Communication Engineering
Indian Institute of Information Technology, Kota


## License

This project is intended for educational and research purposes. Feel free to fork, modify, and build upon it with appropriate attribution.

