#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "Bluetooth_UART.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Pixel.h"

// Create the bluefruit object using connection with MODE Pin
Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

Bluetooth_UART :: Bluetooth_UART (void){
}

void Bluetooth_UART :: startBluetooth (void) {
  delay(500);

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // Set module to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
}

/**
This method sends pixel read by thermal camera
*/
void Bluetooth_UART :: sendData(){
  Pixel p;
  float pixel[8][8];
  p.getPixel(pixel);
  //ble sends read pixel
  ble.print("[ ");
  for(int i = 0; i < 8; i++){
	  for(int j = 0; j < 8; j++){
		ble.print(pixel[i][j]);
		ble.print(", ");
	  }
  }
  ble.print(" ]");
  ble.println();
  //ble.print(pixel);

  // Echo received data --> non va in sendData
  while ( ble.available() )
  {
    int c = ble.read();

    Serial.print((char)c);

    Serial.print(c);
  }
}

void Bluetooth_UART :: writeCommand(){
  /* Enable HID Service */
  Serial.println(F("Enable HID Services (including Control Key): "));
  if (! ble.sendCommandCheckOK(F( "AT+BLEHIDEN=On"  ))) {
    error(F("Failed to enable HID (firmware >=0.6.6?)"));
  }

  /* Adding or removing services requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }

  Serial.println();
 /* Serial.println(F("**********************************************************"));
  Serial.println(F("Go to your phone's Bluetooth settings to pair your device"));
  Serial.println(F("Some Control Key works system-wide: mute, brightness ..."));
  Serial.println(F("Some are application specific: Media play/pause"));
  Serial.println(F("**********************************************************"));

  // Print pre-defined control keys
  printDefinedControlKey();
*/
  Serial.println();
  
  // Display prompt
  Serial.print(F("Control (? for help) > "));

  // Check for user input and echo it back if anything was found
  char keys[BUFSIZE+1];
  getUserInput(keys, BUFSIZE);

  Serial.println(keys);

  if ( keys[0] == '?')
  {
    printDefinedControlKey();
  }else
  {
    ble.print("AT+BleHidControlKey=");
    ble.println(keys);

    if( ble.waitForOK() )
    {
      Serial.println( F("OK!") );
    }else
    {
      Serial.println( F("FAILED!") );
      // Failed, probably pairing is not complete yet
      Serial.println( F("Please make sure Bluefruit is paired and try again") );
    }
  }
}

void Bluetooth_UART :: getUserInput(char buffer[], uint8_t maxSize)
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

/**************************************************************************/
/*!
    @brief  Print pre-defined control keys
*/
/**************************************************************************/
void Bluetooth_UART :: printDefinedControlKey(void)
{
  Serial.println();
  Serial.println(F("You can send a raw 16-bit (e.g 0x1234) usage key from the USB" "\n"
                    "HID Consumer Control Page or use one of the the following keys:"));

  Serial.println(F("List of pre-defined control keys:"));
  Serial.print(F(
    "- BRIGHTNESS+" "\n"
    "- BRIGHTNESS-" "\n"
    "- PLAYPAUSE" "\n"
    "- MEDIANEXT" "\n"
    "- MEDIAPREVIOUS" "\n"
    "- MEDIASTOP" "\n"
    "- VOLUME" "\n"
    "- MUTE" "\n"
    "- BASS" "\n"
    "- TREBLE" "\n"
    "- BASS_BOOST" "\n"
    "- VOLUME+" "\n"
    "- VOLUME-" "\n"
    "- BASS+" "\n"
    "- BASS-" "\n"
    "- TREBLE+" "\n"
    "- TREBLE-" "\n"
    "- EMAILREADER" "\n"
    "- CALCULATOR" "\n"
    "- FILEBROWSER" "\n"
    "- SEARCH" "\n"
    "- HOME" "\n"
    "- BACK" "\n"
    "- FORWARD" "\n"
    "- STOP" "\n"
    "- REFRESH" "\n"
    "- BOOKMARKS" "\n"
  ));
}


