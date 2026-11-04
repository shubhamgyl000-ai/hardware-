# ESP32 Transformer Health Monitor

Starter firmware for ESP32 + LM35 temperature sensor + ADXL345 accelerometer + buzzer.

## VS Code setup

1. Install the **PlatformIO IDE** extension in VS Code.
2. Open this repository folder in VS Code.
3. Connect the ESP32 using a USB data cable and select the detected serial port.
4. In PlatformIO, build the project, then upload it. Open the Serial Monitor at **115200 baud**.

## Wiring (30-pin ESP32 DevKit assumed)

| Part | Pin | ESP32 |
|---|---|---|
| LM35 | VCC | 3V3 |
| LM35 | GND | GND |
| LM35 | OUT | GPIO34 |
| ADXL345 | VCC | 3V3 (module must support 3.3V) |
| ADXL345 | GND | GND |
| ADXL345 | SDA | GPIO21 |
| ADXL345 | SCL | GPIO22 |
| Buzzer | signal/+ | GPIO25 |
| Buzzer | GND/- | GND |

**Important:** Check the pin labels on your actual modules before powering them. This wiring assumes a 30-pin ESP32 DevKit with standard GPIO21/22 I2C pins. Never feed 5V into ESP32 GPIO pins.

## What it does

- Reads LM35 temperature on GPIO34.
- Reads ADXL345 acceleration over I2C and estimates vibration by subtracting gravity from acceleration magnitude.
- Turns the buzzer on if temperature is at least 50°C or estimated vibration is at least 18 m/s².
- Prints readings once per second.

These are example alert thresholds, not calibrated transformer safety limits. Validate sensor readings and set thresholds for your actual setup. LM35 readings depend on ADC calibration and wiring; this starter code is not a certified protection system.
