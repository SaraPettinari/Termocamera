#include <Arduino.h>
#include <Wire.h>
#include <Bluetooth_HID.h>
#include <Pixel.h>
#include <Adafruit_AMG88xx.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"


Bluetooth_HID bluehid;
Pixel p;

void setup() {
  Serial.begin(115200);
  bluehid.startBluetooth();
}

void loop() {
  bluehid.receiveData();
}
