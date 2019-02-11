#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "Bluetooth_HID.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Pixel.h"

// Creazione dell'oggetto bluefruit object usando la connessione con il MODE Pin
Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

Bluetooth_HID :: Bluetooth_HID (void){
}

/**
	Metodo di avvio del bluetooth
*/
void Bluetooth_HID :: startBluetooth (void) {

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  if ( FACTORYRESET_ENABLE )
  {
    if ( ! ble.factoryReset() ){
      error(F("Factory reset failed!"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  // This demo only works with firmware 0.6.6 and higher!
  // Request the Bluefruit firmware rev and check if it is valid
  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    error(F("This sketch requires firmware version " MINIMUM_FIRMWARE_VERSION " or higher!"));
  }

  /* Enable HID Service */
  if (! ble.sendCommandCheckOK(F( "AT+BLEHIDEN=On"  ))) {
    error(F("Failed to enable HID (firmware >=0.6.6?)"));
  }

  /* Adding or removing services requires a reset */
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }
 // Serial.println();
}

/**
	Metodo per inviare il comando riconosciuto alla tastiera del dispositivo
	@command: comando corrispondente alla gesture letta
*/
void Bluetooth_HID :: sendCommand(char *command){
	ble.print("AT+BleHidControlKey=");
	//invio del comando in input
    ble.println(command);
    ble.print("\n");

    if( ble.waitForOK() )
    {
      Serial.println( F("OK!") );
    }else
    {
      Serial.println( F("FAILED!") );
      // Errore: nessun dispositivo è accoppiato col bluetooth
      Serial.println( F("Please make sure Bluefruit is paired and try again") );
    }
}

/**
	"- PLAYPAUSE" "\n"
    "- MEDIANEXT" "\n"
    "- MEDIAPREVIOUS" "\n"
    "- MEDIASTOP" "\n"
	"- VOLUME+" "\n"
    "- VOLUME-" "\n"
    "- MUTE" "\n"
*/