#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Wiring (ESP32 DevKit, 30-pin assumed):
// LM35: OUT -> GPIO34, VCC -> 3V3, GND -> GND
// ADXL345: SDA -> GPIO21, SCL -> GPIO22, VCC -> 3V3, GND -> GND
// Buzzer signal -> GPIO25, other pin -> GND
// Use a 3.3V-compatible ADXL345 module. Do not connect sensor outputs to 5V.

constexpr int LM35_PIN = 34;
constexpr int BUZZER_PIN = 25;
constexpr int I2C_SDA_PIN = 21;
constexpr int I2C_SCL_PIN = 22;

// Adjust these after observing normal readings for your hardware.
constexpr float TEMP_ALERT_C = 50.0f;
constexpr float VIBRATION_ALERT_MS2 = 18.0f;

Adafruit_ADXL345_Unified accelerometer = Adafruit_ADXL345_Unified(12345);

float readTemperatureC() {
  // ESP32 ADC default attenuation is set below for a wider input range.
  const int raw = analogRead(LM35_PIN);
  const float voltage = (raw / 4095.0f) * 3.3f;
  // LM35 output is 10 mV per degree C.
  return voltage * 100.0f;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  analogReadResolution(12);
  analogSetPinAttenuation(LM35_PIN, ADC_11db);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  if (!accelerometer.begin()) {
    Serial.println("ERROR: ADXL345 not detected. Check I2C wiring.");
    while (true) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(150);
      digitalWrite(BUZZER_PIN, LOW);
      delay(850);
    }
  }
  accelerometer.setRange(ADXL345_RANGE_4_G);
  Serial.println("Transformer health monitor started.");
}

void loop() {
  const float temperatureC = readTemperatureC();

  sensors_event_t event;
  accelerometer.getEvent(&event);
  const float ax = event.acceleration.x;
  const float ay = event.acceleration.y;
  const float az = event.acceleration.z;
  const float magnitude = sqrtf(ax * ax + ay * ay + az * az);
  // Remove gravity (approximately 9.81 m/s^2) to estimate dynamic vibration.
  const float vibration = fabsf(magnitude - 9.81f);

  const bool alarm = temperatureC >= TEMP_ALERT_C ||
                     vibration >= VIBRATION_ALERT_MS2;
  digitalWrite(BUZZER_PIN, alarm ? HIGH : LOW);

  Serial.print("Temperature: ");
  Serial.print(temperatureC, 1);
  Serial.print(" C | Vibration: ");
  Serial.print(vibration, 2);
  Serial.print(" m/s^2 | Alert: ");
  Serial.println(alarm ? "YES" : "NO");

  delay(1000);
}
