# Bluetooth gateway Library

### Introduction
Bluetooth gateway enables developers to create mesh gateway application.
The library enables application developers to run embedded bluetooth mesh applications through Cypress WICED HCI Protocol.
Bluetooth gateway provides Interfaces to connect, send and recieve mesh data through a configured IoT protocol such as REST or MQTT.
  
### Features 
* Supports transmission and reception of mesh data through REST or MQTT.
* Allows application developers to run cypress ble embedded application through WICED HCI Protocol.  

### **Supported Platforms**
This code example can be run using the following Cypress kits.
* CY8CKIT_062S2_43012 Kit

### Dependencies
This section provides the list of dependency libraries required for this middleware library to work.
* [ARM mbed-os stack version 5.15.0](https://os.mbed.com/mbed-os/releases)   
  **NOTE:** The Bluetooth mesh gateway library uses the embedded Bluetooth stack present in the CYW43012 Bluetooth chip, and bypasses the Cordio stack.
        Therefore Mbed Cordio Stack APIs should not be used with this library
* The Bluetooth mesh gateway library uses the embedded Bluetooth stack present in the CYW43012 Bluetooth chip, and bypasses the Cordio stack
   Therefore Cordio Stack APIs should not be used with this library
* [ARM mbed-os stack version 5.15.0 and above](https://os.mbed.com/mbed-os/releases)
* [Cypress Connectivity Utilities Library](https://github.com/cypresssemiconductorco/connectivity-utilities)
* [Cypress AWS IoT library](https://github.com/cypresssemiconductorco/Aws-IoT)
* [Cypress HTTP server library](https://github.com/cypresssemiconductorco/HTTP-Server)

Note : This library cannot be used in conjunction with Cordio BLE stack functionality.

### Additional Information
* [Bluetooth gateway RELEASE.md](./RELEASE.md)
* [Bluetooth gateway API reference guide](https://cypresssemiconductorco.github.io/bluetooth-gateway/api_reference_manual/html/index.html)
* [Bluetooth gateway version](./version.txt)