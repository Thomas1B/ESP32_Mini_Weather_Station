/*
  Mini Weather Station using ESP32C3 dev kit 2 and a BME280 Sensor.

  This program was originally written using a esp32 C3 dev kit 2 so pins may be different.
  It collects temperature, humidity, and pressure then uploads its reading to ThingSpeak for remote viewing.

  LED Status:
    LED -> Green:            Everything working
    LED -> Flashing RED:     Failed WiFi Connection
    LED -> Flashing Yellow:  Failed BME280 Connection

  By Thomas Bourgeois
*/

//  **************** Libraries ****************
#include "LibPrintf.h"
#include "secrets.h"
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>
#include "ThingSpeak.h"

//  **************** User Variables ****************
#define PIN_RGB 8
#define I2C_SDA 1
#define I2C_SCL 0
#define BME_ADR 0x76  // BME address

const unsigned long wifiTimeout = 5 * 1000;  // must be milliseconds

//  **************** Program Variables ****************

// WiFi Parameters:
unsigned long wifiStartAttemptTime = millis();
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

// Thingspeak Parameters:
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

// RGB Object:
Adafruit_NeoPixel rgbLED(1, PIN_RGB, NEO_GRB + NEO_KHZ800);
Adafruit_BME280 bme;  // bme sensor object

//  **************** Function Declaration ****************
void setRGB(byte r, byte g, byte b);

//  **************** Set up ****************
void setup() {
  setRGB(0, 0, 0);  // LED off to start
  Serial.begin(115200);
  printf("\n*** Program Started ***\n");
  delay(1000);

  // --- I2C ---
  Wire.begin(I2C_SDA, I2C_SCL);

  // Connecting to WiFi, timeout after some time.
  unsigned long prevTime = 0;
  bool ledState = false;
  printf("Connecting to %s", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStartAttemptTime < wifiTimeout) {
    unsigned long now = millis();  // flashing led while connecting
    if (now - prevTime >= 100) {
      prevTime = now;
      ledState = !ledState;
      if (ledState) {
        setRGB(50, 0, 0);
      } else {
        setRGB(0, 0, 0);
      }
    }
  }

  // Checking WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    printf(" -> Connected!\n");
  } else {
    printf(" -> Failed!\n");
    while (1) {
      setRGB(50, 0, 0);
      delay(500);
      setRGB(0, 0, 0);
      delay(500);
    }
  }

  // Checking BME status
  if (bme.begin(BME_ADR)) {
    printf("BME280 Sensor Connection Succesful!\n");
  } else {
    printf("BME280 Sencer Connection Failed...\n");
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
  /*
  Function to set color and update rgb LED.

  Parameters:
    r: 0-255 value for red.
    g: 0-255 value for green.
    b: 0-255 value for blue.
  */
  rgbLED.setPixelColor(0, rgbLED.Color(r, g, b));
  rgbLED.show();
}
