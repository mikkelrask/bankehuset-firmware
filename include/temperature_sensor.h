#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266Wifi.h>

const int DS18B20_PIN = D4;
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

void initTemperatureSensor() {
  sensors.begin();
}

float getTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void sendTemperatureToAPI(float temperature, const char *apiEndpoint, const char *sha1Fingerprint) {
  WiFiClientSecure client;
  client.setFingerprint(sha1Fingerprint);

  if (!client.connect(apiEndpoint, 443)) {
    Serial.println("Connection to API failed!");
    return;
  }

  String payload = "{\"temperature\": " + String(temperature) + "}";

  // Send the POST request to the API
  HTTPClient http;
  http.begin(client, apiEndpoint);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(payload);
  http.end();

  if (httpCode > 0) {
    Serial.print("Temperature sent to API, response code: ");
    Serial.println(httpCode);
  } else {
    Serial.print("Failed to send temperature to API, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }
}

#endif