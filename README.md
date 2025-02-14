# Smart Clock with ESP32  

This project implements a smart clock using **ESP32**, featuring **WiFi, MQTT, IR control, sensors, an LCD display, and a buzzer**. It uses **PlatformIO** for compiling and **Wokwi** for simulation.  

## Features  
- **Real-Time Clock (RTC)** for accurate timekeeping  
- **MQTT Support** for remote updates  
- **Infrared (IR) Control** for remote operation  
- **Neopixel LED Integration** for visual alerts  
- **Multiple Sensors** (CO₂, DHT22, PIR) for environmental monitoring  
- **Buzzer** for alarms and notifications  
- **LCD Display** with timezone handling  

## File Structure  
- `lib/IR/` – Infrared remote handling  
- `buzzer.cpp/.hpp` – Alarm sound management  
- `mqtt.cpp/.hpp` – MQTT communication  
- `rtc.cpp/.hpp` – Real-time clock logic  
- `screen.cpp/.hpp` – LCD screen control  
- `sensors.cpp/.hpp` – General sensor integration  
- `wifi.cpp/.hpp` – WiFi connectivity  

## Setup  

### 1. Install Dependencies  
- **PlatformIO** in VS Code  
- **Wokwi API** for ESP32 simulation ([Wokwi Extension](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode))  

### 2. Clone and Open in VS Code  
```sh
git clone https://github.com/your-repo/smart-clock.git  
cd smart-clock  
code .
```

### 3. Run Simulation  
Modify `.wokwi-project.json` if needed, then:  
```sh
pio run -e wokwi
```

### 4. Compile and Upload to ESP32  
```sh
pio run -e esp32dev -t upload
```

## License  
This project is open-source under the **MIT License**.  
