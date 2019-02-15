#ifndef Bluetooth_HID_h
#define Bluetooth_HID_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"

#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output
#define BLUEFRUIT_HWSERIAL_NAME      Serial1	//Serial1 -> si riferisce ai pins 19 (RX) and 18 (TX)
#define BLUEFRUIT_UART_MODE_PIN        12

#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"

#define	VOLUME_UP				VOLUME+
#define	VOLUME_DOWN				VOLUME-


class Bluetooth_HID
{
  public:
    Bluetooth_HID(void);
	//avvio del bluetooth
	void startBluetooth(void);
	//invio del comando alla tastiera
	void sendCommand(char*);
};

#endif

