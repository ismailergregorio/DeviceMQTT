#include <WiFi.h>
#include <Arduino.h>

String ConexaoWifi(String ssid,String password)
{
    Serial.println();
    Serial.print("Conectando no WiFi: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado!");
    Serial.print("IP do ESP: ");
    Serial.println(WiFi.localIP());

    return WiFi.localIP().toString();
}