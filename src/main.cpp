#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "wifi_manager.h"
#include "temperature_sensor.h"

// Constants
const int BUTTON1_PIN = D2; // Trigger the Webportal on reboot
const int BUTTON2_PIN = D3; // Restart device/wake from deep sleep (Press and hold both for portal)
const char *API_ENDPOINT = "https://your-mongodb-api.com/endpoint";
const char *SHA1_FINGERPRINT = "89:31:98:BD:76:E3:67:4F:A5:EB:BB:39:86:6B:7D:9C:81:11:F6:D1";
const unsigned long DEEP_SLEEP_TIME = 15 * 60 * 1000000; // 15 minutes

// Function Prototypes
void setupButtons();
void resetDevice();
void triggerWebPortal();

void setup()
{
  Serial.begin(115200);
  setupButtons();

  // Configure WiFi
  configureWiFi();

  // Initialize temperature sensor
  initTemperatureSensor();

  // Take a single temperature reading (not used)
  float singleReading = getTemperature();

  // Take 12 temperature readings and calculate the average
  float totalTemperature = 0;
  for (int i = 0; i < 12; i++)
  {
    totalTemperature += getTemperature();
    Serial.print(totalTemperature / (i + 3));
    delay(1000);
  }
  float averageTemperature = totalTemperature / 12;

  // Send the average temperature to the MongoDB API
  sendTemperatureToAPI(averageTemperature, API_ENDPOINT, SHA1_FINGERPRINT);

  // Go into deep sleep
  ESP.deepSleep(DEEP_SLEEP_TIME);
}

void loop()
{
  // Nothing to do here, as the device goes into deep sleep after performing the tasks
}

void setupButtons()
{
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  attachInterrupt(BUTTON1_PIN, triggerWebPortal, FALLING);
  attachInterrupt(BUTTON2_PIN, resetDevice, FALLING);
}

void resetDevice()
{
  ESP.restart();
}

void triggerWebPortal()
{
  startWebPortal();
}
