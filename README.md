# Zoho SETU: Zephyr OS Temperature Monitor (ESP32)

A professional "out-of-tree" Zephyr RTOS application for the ESP32. This project uses a Finite State Machine (FSM) to monitor ambient temperature via a BMP280 sensor and provides visual feedback using status LEDs.

## 🚀 Features
- **Deterministic FSM:** Robust state management for sensor polling and LED control.
- **I2C Integration:** Real-time data acquisition from Bosch BMP280.
- **Devicetree Overlays:** Hardware-agnostic logic using aliases for easy porting to other boards.

---

## 🔌 Hardware Connections (ESP32)

| Component | ESP32 Pin | Connection Type |
| :--- | :--- | :--- |
| **BMP280 SCL** | GPIO 22 | I2C Clock |
| **BMP280 SDA** | GPIO 21 | I2C Data |
| **Red LED** | GPIO 25 | Active High (Alarm) |
| **Yellow LED** | GPIO 26 | Active High (Normal) |
| **Power** | 3.3V | Power Rail |
| **Ground** | GND | Common Ground |

---

## 🛠️ Setup & Build

### Prerequisites
- [Zephyr RTOS Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting-started/index.html) completed.
- Zephyr SDK installed.
- ESP32 toolchain installed (`west espressif install`).

### 1. Environment Configuration
Activate your Zephyr virtual environment and export your Zephyr base directory.

**Example (based on my setup):**
```bash
# Activate your python venv
source /home/knownperson/zephyr-venv/bin/activate

# Set your Zephyr base path
export ZEPHYR_BASE=/home/knownperson/zephyrproject/zephyr
```

### 2. Build and Flash
Run these commands from the project root:

```bash
# Build for ESP32 DevKitC
west build -p always -b esp32_devkitc/esp32/procpu .

# Flash to hardware
west flash

# Monitor serial output
west espressif monitor
```

---

## 🧠 Project Architecture (FSM)
The application logic follows a four-state Finite State Machine:
1.  **`STATE_INIT`**: Hardware readiness check (Sensor + GPIO).
2.  **`STATE_READ_TEMP`**: Fetches ambient temperature.
3.  **`STATE_TEMP_HIGH`**: Temperature > 30.0°C. Red LED active.
4.  **`STATE_TEMP_LOW`**: Temperature <= 30.0°C. Yellow LED active.

---

## 📝 License
This project was created by **[Mohit-0305](https://github.com/Mohit-0305)**. It is open-source, and anyone is free to use, modify, and distribute it for their own projects or learning!
