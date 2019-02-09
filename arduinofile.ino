#include <Arduino.h>
#include <Wire.h>
#include <Bluetooth_HID.h>
#include <Pixel.h>
#include <Adafruit_AMG88xx.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"


Bluetooth_HID bluehid;
Pixel pixelib;
bool movimento = false;
int matrix[8][8];
int splittedMatrix[2][8];
int gesture = 0;

void setup() {
  Serial.begin(115200);
  bluehid.startBluetooth();
}

void loop() {
  pixelib.matrixConverter(matrix);
  pixelib.printMatrix(matrix);  
 
  //non è stato ancora riconosciuto un movimento
  if(movimento == false){
    if(pixelib.containsThree(matrix)){
      movimento = true;
      }
    }
    //è stata riconosciuta una gesture
    if(movimento == true){
      //TODO: metodo di riconoscimento
      Serial.println(gesture);
      int temp = pixelib.matrixAnalysis(matrix, gesture);
      gesture = temp;
      Serial.println(gesture);
      //l'ultima matrice letta non ha percepito calore
      if(!pixelib.containsThree(matrix)){
        movimento = false;
        gesture = 0;
        }
      }
}
