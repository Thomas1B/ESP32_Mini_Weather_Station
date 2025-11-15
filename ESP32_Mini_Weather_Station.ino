
/*
  Mini Weather Station using ESP32C3 dev kit 2 and a BME280 Sensor.

  This program was originally written using a esp32 C3 dev kit 2 so pins may be different.
  It collects temperature, humidity, and pressure then uploads its reading to ThingSpeak for remote viewing.

  Note:
    LED -> Flashing RED:     Failed WiFi Connection
    LED -> Flashing Yellow:  Failed BME280 Connection

  By Thomas Bourgeois
*/

//  **************** Libraries ****************
#include "secrets.h"
#include "ThingSpeak.h"
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "LibPrintf.h"
#include <Adafruit_NeoPixel.h>

//  **************** User Variables ****************
#define I2C_SDA 1
#define I2C_SCL 0
#define PIN_RGB 8
// #define BME_ADR 0x76  // bme address
#define BME_ADR 0x90  // bme address test

const unsigned long wifiTimeout = 30 * 1000;

//  **************** Variables ****************

// WiFi Parameters:
unsigned long wifiStartAttemptTime = millis();
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

// Thingspeak Parameters:
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;


Adafruit_NeoPixel rgbLED(1, PIN_RGB, NEO_GRB + NEO_KHZ800);
Adafruit_BME280 bme;  // bme sensor object

//  **************** Function Declaration ****************
void setRGB(byte r, byte g, byte b);

//  **************** Set up ****************
void setup() {
  setRGB(0, 0, 0); // turnning of LED to start
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
    delay(250);
  }

  // Checking WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    printf("\nWiFi connected!\n");
  } else {
    printf("\nWiFi connection failed...\n");
    while (1) {
      setRGB(50, 0, 0);
      delay(500);
      setRGB(0, 0, 0);
      delay(500);
    }
  }

  // Checking BME status
  if (bme.begin(BME_ADR)) {
    printf("BME280 detected!\n");
  } else {
    printf("\nBME280 connection failed...\n");
    while (1) {
      setRGB(50, 50, 0);
      delay(500);
      setRGB(0, 0, 0);
      delay(500);
    }
  }

  // ThingSpeak.begin(client); // connecting to thingspeak

  printf("Everthing is connected!\n\n");
  setRGB(0, 50, 0);
}


//  **************** Main Loop ****************
void loop() {
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float pres = bme.readPressure() / 100.0;

  printf("T = %0.1f, H = %0.1f, P = %0.2f\n", temp, hum, pres);

  delay(1500);  // 20 seconds
}


void setRGB(byte r, byte g, byte b) {
  rgbLED.setPixelColor(0, rgbLED.Color(r, g, b));
  rgbLED.show();
}
