# ESP32 Mini Weather Station with BME280

  Mini Weather Station using ESP32C3 dev kit 2 and a BME280 Sensor.

  This program was originally written using a esp32 C3 dev kit 2 so pins may be different.
  It collects temperature, humidity, and pressure then uploads its reading to ThingSpeak for remote viewing.


  LED Status:
    LED -> Green:            Everything working
    LED -> Flashing RED:     Failed WiFi Connection
    LED -> Flashing Yellow:  Failed BME280 Connection