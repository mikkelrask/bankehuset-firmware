#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

void configureWiFi()
{
  WiFiManager wifiManager;

  // Set timeout for configuration portal
  wifiManager.setConfigPortalTimeout(180);

  // Automatically connect using saved credentials, if they exist
  if (!wifiManager.autoConnect("your-home-network", "wifi-passkey"))
  {
    Serial.println("Kunne ikke tilslutte - Genstarter");
    ESP.restart();
  }

  Serial.println("Tilsluttede til WiFi!");
}

void startWebPortal()
{
  WiFiManager wifiManager;

  // Start the configuration portal (BankehusetAP = SSID, solskin=passkey)
  wifiManager.startConfigPortal("BankehusetAP", "solskin");
}

#endif
