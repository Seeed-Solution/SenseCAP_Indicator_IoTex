# SenseCAP Indicator X IoTex (W3bstream)

This project integrates the [SenseCAP Indicator](https://www.seeedstudio.com/SenseCAP-Indicator-D1S-p-5645.html) with the [IoTex](https://iotex.io/) blockchain platform to enable sending sensor data to IoTex.

## Introduction

The SenseCAP Indicator is an industrial IoT development board featuring multiple sensors, WiFi/BLE connectivity, and programmability using C/C++ with the ESP-IDF framework. 

<!-- This project utilizes the SenseCAP's sensors and connectivity to read sensor data and transmit it to the IoTex blockchain network. IoTex provides services for trusted data and automation, making it useful for IoT solutions requiring provenance and security. -->

By integrating these two platforms, we can build reliable IoT solutions that leverage blockchain's decentralization, security, and immutability properties. Potential applications include supply chain monitoring, industrial automation, smart cities, etc.

## Getting Started

To use this project, you will need:

- SenseCAP Indicator development board
- IoTex account 
- Development environment set up for ESP-IDF 

### Hardware Setup

- Connect the SenseCAP Indicator to your computer via USB
- Ensure the board can connect to WiFi (required for Internet access)

### Software Setup

1. Install ESP-IDF development framework 
    - Follow instructions [here](https://docs.espressif.com/projects/esp-idf/en/release-v5.0/esp32/get-started/index.html)

2. Clone this repository
   ```bash
   git clone https://github.com/Seeed-Solution/SenseCAP_Indicator_IoTex
   ```

3. Sign up for an IoTex account at [https://iotex.io/](https://iotex.io/)

4. Configure your IoTex credentials in `main/iotex_config.h`

5. Build and deploy the project to your SenseCAP
    > more details on [SenseCAP_Indicator_How_To_Flash_The_Default_Firmware](https://wiki.seeedstudio.com/SenseCAP_Indicator_How_To_Flash_The_Default_Firmware/)

   ```bash
   idf.py build flash monitor
   ```

Now the SenseCAP should begin publishing sensor data to IoTex!

## Usage

<!-- By default this project will read the on-board temperature sensor every 5 seconds and publish the reading to IoTex.

The sensor reading frequency, IoTex endpoint, and other parameters can be configured in `main/iotex_config.h`.

See the [API documentation](https://developers.iotex.io/reference/api-reference) for details on integrating with IoTex services like storing data on-chain. -->

## References

- [W3bstream Doc](https://docs.w3bstream.com/introduction/readme)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)
- [Flash Firmware to SenseCAP Indicator](https://wiki.seeedstudio.com/SenseCAP_Indicator_How_To_Flash_The_Default_Firmware/)
- [W3bstream Component · GitHub](https://github.com/machinefi/w3bstream-client-esp32)