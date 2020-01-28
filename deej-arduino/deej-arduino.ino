/*
 * Adapted from  omriharel's code to support esp32 and mqtt
 * Credit to him for the project idea 
 * See EspMQTT library for details on how to use it as a client like below
 */

#include "EspMQTTClient.h"

const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {39, 34, 35, 32, 33}; // GPIO pins, only use ADC1
const int wifiLED = 21;

int analogSliderValues[NUM_SLIDERS];

const String topic = "home/volume";

EspMQTTClient client(
  "", // WIFI SSID
  "", // WIFI PASSWORD
  "192.168.0.26",  // MQTT Broker server ip
  "",
  "",
  "esp32", // Client name
  1883
);

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  pinMode(wifiLED, OUTPUT);
  
  Serial.begin(115200);
}

void onConnectionEstablished()
{
//  client.subscribe("home/volume", [](const String & payload) {
//    Serial.println(payload); // debugging
//  });

  // client.publish(topic, "Connected");
  digitalWrite(wifiLED, HIGH);
}

void loop() {
  client.loop(); // needed for EspMQTTClient
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  //printSliderValues(); // For debug
  delay(20);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  // Serial.println(builtString);
  client.publish(topic, builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
