#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "Pixel.h"

Adafruit_AMG88xx amg; 
Bluetooth_HID bluetooth;

Pixel :: Pixel (void){
}

void Pixel :: getPixel(float matrix[ARRAY_SIZE][ARRAY_SIZE]){
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
	for(int i = 0; i < ARRAY_SIZE; i ++){
		for(int j = 0; j < ARRAY_SIZE; j++){
			matrix[i][j] = pixels[k];
		//	Serial.print(matrix[i][j]);
		//	if((j+1)%ARRAY_SIZE != 0)	Serial.print(", ");
			k++;
		}
	//	Serial.println();
	}
    Serial.println();

    //delay a second
    delay(1);
}

void Pixel :: printMatrix(int matrix [ARRAY_SIZE][ARRAY_SIZE]){
	//stampa della matrice 8x8 con le temperature percepite
    for(int i = 0; i < ARRAY_SIZE; i++){
		for(int j = 0; j < ARRAY_SIZE; j ++){
			Serial.print(matrix[i][j]);
			if((j+1)%ARRAY_SIZE != 0)	Serial.print(", ");
	  }
	  Serial.println();       //dopo 8 pixel va a capo
    }
}

/**
La matrice di pixel letti viene semplificata nel seguente modo:
	-	Alla temperatura dell'ambiente viene associato 1;
	-	Quando viene percepito l'avvicinamento della mano si assocer? il valore 2;
	-	Il passaggio della mano viene indicato con il 3;
	-	Un valore di temperatura esageratamente grande viene identificato col numero 4.
*/
void Pixel :: matrixConverter(int matrix[ARRAY_SIZE][ARRAY_SIZE]){
	float convertedMatrix[ARRAY_SIZE][ARRAY_SIZE];
	getPixel(convertedMatrix);
	for(int i = 0; i < ARRAY_SIZE; i++){
		for(int j = 0; j < ARRAY_SIZE; j++){
			float appoggio = convertedMatrix[i][j];
			if(appoggio <= 22.0)
				matrix[i][j] = 1;
			else if(appoggio > 22.0 && appoggio < 24.0)
				matrix[i][j] = 2;
			else if(appoggio >= 24.0 && appoggio < 38.0)
				matrix[i][j] = 3;
			else
				matrix[i][j] = 4;
		}
	}
}

bool Pixel :: containsThree(int matrix[ARRAY_SIZE][ARRAY_SIZE]){
	for(int i = 0; i < ARRAY_SIZE; i++){
		for(int j = 0; j < ARRAY_SIZE; j++){
			if(matrix[i][j] == 3)
				return true;
		}
	}
		return false;
}

bool Pixel :: containsThreeSplitted(int matrix[2][ARRAY_SIZE]){
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < ARRAY_SIZE; j++){
			if(matrix[i][j] == 3)
				return true;
		}
	}
		return false;
}

bool Pixel :: numThreeSplitted(int matrix[2][ARRAY_SIZE]){
	int three = 0;
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < ARRAY_SIZE; j++){
			if(matrix[i][j] == 3)
				three++;
		}
	}
	if(three >= 10)
		return true;
	else
		return false;
}

/**
matrix: matrice iniziale
splitted matrix: matrice divisa in righe o colonne da 2
c: 
	= t: matrice divisa per righe dall'alto
	= b: matrice divisa per righe dal basso
	= l: matrice divisa per colonne da sinistra
	= r: matrice divisa per colonne da destra
*/
void Pixel :: matrixSplit(int matrix[ARRAY_SIZE][ARRAY_SIZE], int splittedMatrix[2][ARRAY_SIZE], char c){
	int i = 0;	//indice per matrice iniziale
	if(c == 't'){
		for(int j = 0; j < ARRAY_SIZE; j ++){
		splittedMatrix[0][j] = matrix[0][j];
		splittedMatrix[1][j] = matrix[1][j];
		}
		//prende le prime due righe della matrice
	}
	else if(c == 'b'){
		//prende le ultime due righe della matrice
		i = ARRAY_SIZE - 1;
		for(int j = 0; j < ARRAY_SIZE; j ++){
		splittedMatrix[0][j] = matrix[i - 1][j];
		splittedMatrix[1][j] = matrix[i][j];
		}
	}
	else if(c == 'l'){
		//prende le prime due colonne da sx della matrice
		for(int j = 0; j < ARRAY_SIZE; j ++){
			splittedMatrix[0][j] = matrix[j][0];
			splittedMatrix[1][j] = matrix[j][1];
		}
	}
	else if (c == 'r'){
		//prende le prime due colonne da dx della matrice
		//prende le prime due colonne da sx della matrice
		i = ARRAY_SIZE - 1;
		for(int j = 0; j < ARRAY_SIZE; j ++){
			splittedMatrix[0][j] = matrix[j][i - 1];
			splittedMatrix[1][j] = matrix[j][i];
		}
	}
}

int Pixel :: matrixAnalysis(int matrix[ARRAY_SIZE][ARRAY_SIZE], int gestureVal){
	int splittedMatrixLeft[2][ARRAY_SIZE];
	int splittedMatrixRight[2][ARRAY_SIZE];
	int splittedMatrixBottom[2][ARRAY_SIZE];
	int splittedMatrixTop[2][ARRAY_SIZE];
	matrixSplit(matrix, splittedMatrixLeft, 'l');
	matrixSplit(matrix, splittedMatrixRight, 'r');
	matrixSplit(matrix, splittedMatrixBottom, 'b');
	matrixSplit(matrix, splittedMatrixTop, 't');
	//non ? ancora stato ipotizzato un gesto
	if(gestureVal == 0){
		//se sembra da sx a dx
		if(containsThreeSplitted(splittedMatrixLeft) && !containsThreeSplitted(splittedMatrixRight) && numThreeSplitted(splittedMatrixLeft)){
			Serial.println("SX");
			return 1;
		}
		//se sembra da dx a sx
		else if(containsThreeSplitted(splittedMatrixRight) && !containsThreeSplitted(splittedMatrixLeft) && numThreeSplitted(splittedMatrixRight)){
			Serial.println("DX");
			return 2;
		}
		else if(containsThreeSplitted(splittedMatrixBottom) && !containsThreeSplitted(splittedMatrixTop) && numThreeSplitted(splittedMatrixBottom)){
			Serial.println("BOT");
			return 3;
		}
		else if(containsThreeSplitted(splittedMatrixTop) && !containsThreeSplitted(splittedMatrixBottom) && numThreeSplitted(splittedMatrixTop)){
			Serial.println("TOP");
			return 4;
		}
		//non ? stato riconosciuto nessun gesto
		else
			return 0;
	}
	//sembra essere riconosciuto da SX a DX
	else if(gestureVal == 1){
		//se il calore si ? spostato a DX
		if(containsThreeSplitted(splittedMatrixRight) && !containsThreeSplitted(splittedMatrixLeft)){
			Serial.println("SX-Recognized");
			bluetooth.sendCommand("MEDIANEXT");
			return 11;
		}
		//non si ? ancora certi del gesto
		else 
			return 1;
	}
	//sembra essere riconosciuto da DX a SX
	else if(gestureVal == 2){
		//se il calore si ? spostato a SX
		if(containsThreeSplitted(splittedMatrixLeft) && !containsThreeSplitted(splittedMatrixRight)){
			Serial.println("DX-Recognized");
			bluetooth.sendCommand("MEDIAPREVIOUS");
			return 22;
		}
		//non si ? ancora certi del gesto
		else 
			return 2;
	}
	//sembra essere riconosciuto da BOT a TOP
	else if(gestureVal == 3){
		//se il calore si ? spostato a TOP
		if(containsThreeSplitted(splittedMatrixTop) && !containsThreeSplitted(splittedMatrixBottom)){
			Serial.println("BOT-Recognized");
			bluetooth.sendCommand("VOLUME+");
			return 33;
		}
		//non si ? ancora certi del gesto
		else 
			return 3;
	}
	//sembra essere riconosciuto da TOP a BOT
	else if(gestureVal == 4){
		//se il calore si ? spostato a BOT
		if(containsThreeSplitted(splittedMatrixBottom) && !containsThreeSplitted(splittedMatrixTop)){
			Serial.println("TOP-Recognized");
			bluetooth.sendCommand("VOLUME-");
			return 44;
		}
		//non si ? ancora certi del gesto
		else 
			return 4;
	}
	//non ? stato riconosciuto nessun gesto
	else 
		return 0;
}