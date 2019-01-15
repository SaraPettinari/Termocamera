#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "Bluetooth_HID.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Pixel.h"

// Create the bluefruit object using connection with MODE Pin
Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

Bluetooth_HID :: Bluetooth_HID (void){
}

void Bluetooth_HID :: startBluetooth (void) {

 // Serial.println(F("Adafruit Bluefruit HID Control Key"));
 // Serial.println(F("---------------------------------------"));

  /* Initialise the module */
 // Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
 // Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
  //  Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Factory reset failed!"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  //Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
 // ble.info();

  // This demo only works with firmware 0.6.6 and higher!
  // Request the Bluefruit firmware rev and check if it is valid
  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    error(F("This sketch requires firmware version " MINIMUM_FIRMWARE_VERSION " or higher!"));
  }

  /* Enable HID Service */
 // Serial.println(F("Enable HID Services (including Control Key): "));
  if (! ble.sendCommandCheckOK(F( "AT+BLEHIDEN=On"  ))) {
    error(F("Failed to enable HID (firmware >=0.6.6?)"));
  }

  /* Adding or removing services requires a reset */
//  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }
 // Serial.println();
}

/**
This method sends pixel read by thermal camera
*/
void Bluetooth_HID :: receiveData(){
  Pixel p;
  float pixel[8][8];
  p.getPixel(pixel);

  for(int i = 0; i < 8; i++){
	  for(int j = 0; j < 8; j++){
		Serial.print(pixel[i][j]);
		if(j != 7 )
			Serial.print(", ");
	  }
	  Serial.println();
  }
}

void Bluetooth_HID :: sendCommand(){
  
}


/**
TODO: riconoscimento gesture e creazione stringa corrispondente
	"- PLAYPAUSE" "\n"
    "- MEDIANEXT" "\n"
    "- MEDIAPREVIOUS" "\n"
    "- MEDIASTOP" "\n"
	"- VOLUME+" "\n"
    "- VOLUME-" "\n"
    "- MUTE" "\n"
*/
void Bluetooth_HID :: getUserInput(char buffer[], uint8_t maxSize)
{
    memset(buffer, 0, maxSize);
	while(Serial.available() == 0 ) {
		delay(1);
  }

  uint8_t count=0;

  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && !(Serial.available() == 0) );
}

