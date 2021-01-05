#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    Serial.begin(115200);
    WiFiManager wm;
    //reset settings - wipe credentials for testing
    wm.resetSettings();
    // set dark theme
    wm.setClass("invert");
    wm.autoConnect("password"); // auto generated AP name from chipid with password
 }

void loop() {
    // put your main code here, to run repeatedly:

}
