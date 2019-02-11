#include <Arduino.h>
#include <Wire.h>
#include <Bluetooth_HID.h>
#include <Pixel.h>
#include <Adafruit_AMG88xx.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"


Bluetooth_HID bluehid;
Pixel pixelib;
bool movimento = false; //controllo del movimento
int matrix[8][8]; //matrice dei pixel letti
int splittedMatrix[2][8]; //matrice suddivisa
int gesture = 0;  //controllo del gesto visto

void setup() {
  Serial.begin(115200);
  //avviato il bluetooth
  bluehid.startBluetooth();
}

void loop() {
  //creazione della matrice di pixel letti
  pixelib.matrixConverter(matrix);
  pixelib.printMatrix(matrix);  
 
  //non è stato ancora riconosciuto un movimento
  if(movimento == false){
    //ipotizzato un gesto
    if(pixelib.containsThree(matrix)){
      movimento = true;
      }
    }
    //è stata ipotizzata una gesture
    if(movimento == true){
      int temp = pixelib.matrixAnalysis(matrix, gesture);
      gesture = temp;
      //l'ultima matrice letta non percepisce più calore 
      if(!pixelib.containsThree(matrix)){
        movimento = false;
        gesture = 0;
        }
      }
}
