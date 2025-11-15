/*
  Mini Weather Station using ESP32C3 dev kit 2 and a BME280 Sensor.

  This program was originally written using a esp32 C3 dev kit 2 so pins may be different.
  It collects temperature, humidity, and pressure then uploads its reading to ThingSpeak for remote viewing.

  By Thomas Bourgeois
*/

#include "secrets.h"
#include "ThingSpeak.h"
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "LibPrintf.h"


// WiFi Parameters:
unsigned long wifiStartAttemptTime = millis();
const unsigned long wifiTimeout = 30 * 1000;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

// Thingspeak Parameters:
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

// I2C Parameters
#define I2C_SDA 1
#define I2C_SCL 0
#define BME_ADR 0x76;

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  printf("***Program Started***\n");
  delay(1000);

  // --- I2C ---
  Wire.begin(I2C_SDA, I2C_SCL);

  // Connecting to WiFi, timeout after some time.
  printf("Connecting to WiFi: %s\n", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStartAttemptTime < wifiTimeout) {
    printf(".");
    delay(50);
  }

  // Checking WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    printf("\nWiFi connected!\n");
  }

  // Checking BME status
  if (bme.begin(BME_ADR)) {
    printf("BME280 detected!\n");
  }

  // ThingSpeak.begin(client); // connecting to thingspeak
}

void loop() {
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float pres = bme.readPressure() / 100.0;

  printf("T = %0.1f, H = %0.1f, P = %0.2f\n", temp, hum, pres);

  delay(1500);  // 20 seconds
}
