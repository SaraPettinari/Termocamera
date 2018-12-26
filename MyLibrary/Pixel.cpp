#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "Pixel.h"

Adafruit_AMG88xx amg; 
//float gesture[5];

Pixel :: Pixel (void){
}

void Pixel :: getPixel(float matrix[8][8]){
	bool status;
    // start reading pixels
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }
	
	float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
	
	//read all the pixels
    amg.readPixels(pixels);
	int k = 0;
	for(int i = 0; i < 8; i ++){
		for(int j = 0; j < 8; j++){
			matrix[i][j] = pixels[j];
		}
	}
    //stampa della matrice 8x8 con le temperature percepite
  /*  for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      Serial.print(pixels[i-1]);
	  matrix[k] = pixels[i-1];
      if(i%8 != 0) Serial.print(",");
      //dopo 8 pixel va a capo
      if( i%8 == 0 ){
		  Serial.println();
		  k ++;
	  }
    }*/
    Serial.println();

    //delay a second
    delay(50);
}

/*float* Pixel :: isGesture(float pixel){
	int length = sizeof(gesture);
	//se l'array è pieno shifta tutti a sx, eliminando il primo elemento
	if(length == 5){
		for(int i = 0; i < length; i++){
			gesture[i] = gesture[i++];
		}
	}
	//aggiunge l'ultimo blocco di pixel alla fine dell'array
	gesture[length] = pixel;
	Serial.print(gesture);
}
*/
/*
* un metodo che ritorna un bool true -> gesto riconosciuto
* uno ritorna l'associazione gesto - codice
*/