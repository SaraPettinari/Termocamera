// ensure this library description is only included once
#ifndef Bluetooth_UART_h
#define Bluetooth_UART_h

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


// library interface description
class Bluetooth_UART
{
  // user-accessible "public" interface
  public:
    Bluetooth_UART(void);
	void startBluetooth(void);
	void getUserInput(char[], uint8_t);
	void writeCommand();
	void printDefinedControlKey(void);
	//invia quello che ? stato letto dalla thermocam.
	void sendData();
};

#endif

