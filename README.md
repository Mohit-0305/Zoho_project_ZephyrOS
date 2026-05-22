# Zoho SETU: Zephyr OS Temperature Monitor (ESP32)

A Zephyr RTOS application that monitors ambient temperature using a BMP280 sensor and controls status LEDs via a Finite State Machine (FSM).

## 1. Hardware Connections (ESP32-WROOM-32)

| Component | ESP32 Pin | Connection Type |
| :--- | :--- | :--- |
| **BMP280 SCL** | GPIO 22 | I2C Clock |
| **BMP280 SDA** | GPIO 21 | I2C Data |
| **Red LED** | GPIO 25 | Active High (Status High) |
| **Yellow LED** | GPIO 26 | Active High (Status Normal) |
| **Sensor VCC** | 3.3V | Power (Use 3.3V rail) |
| **Common GND** | GND | Ground |

---

## 2. Environment Setup

Before running any commands, ensure your Python virtual environment is active and the Zephyr base path is exported.

```bash
# 1. Activate Python Virtual Environment
source /home/knownperson/zephyr-venv/bin/activate

# 2. Export Zephyr Base Path
export ZEPHYR_BASE=/home/knownperson/zephyrproject/zephyr

# 3. (Optional) Initialize Zephyr Environment variables
source $ZEPHYR_BASE/zephyr-env.sh
```

---

## 3. Development Commands

Run these commands from the project root (`~/Desktop/zoho`).

### Build the Project
Compiles the code for the ESP32 DevKitC. The `-p always` flag ensures a clean build.
```bash
west build -p always -b esp32_devkitc/esp32/procpu .
```

### Flash the Firmware
Uploads the compiled binary to the ESP32 via USB.
```bash
west flash
```

### Serial Monitor
View the temperature readings and FSM state transitions in real-time.
```bash
west espressif monitor
```
*Note: To exit the monitor, use `Ctrl + ]`.*

---

## 4. Project Logic (FSM)
* **STATE_INIT**: Validates that the BMP280 and GPIOs are ready.
* **STATE_READ_TEMP**: Fetches ambient temperature via I2C.
* **STATE_TEMP_HIGH**: (> 30.0°C) Turns on Red LED, turns off Yellow.
* **STATE_TEMP_LOW**: (<= 30.0°C) Turns on Yellow LED, turns off Red.

---

## 5. Troubleshooting
* **Unknown Command "build"**: Ensure `ZEPHYR_BASE` is exported or you are inside the virtual environment.
* **Devicetree Error**: Ensure `app.overlay` labels match the `main.c` aliases.
* **I2C Init Failed**: Check wiring and ensure the BMP280 is at address `0x76`.
