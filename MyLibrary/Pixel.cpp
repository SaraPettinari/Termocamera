#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include "Pixel.h"

Adafruit_AMG88xx amg; 
Bluetooth_HID bluetooth;

Pixel :: Pixel (void){
}

/**
	Metodo di lettura dei pixel letti dalla termocamera
*/
void Pixel :: getPixel(float matrix[ARRAY_SIZE][ARRAY_SIZE]){
	bool status;
    // avviata la lettura dei pixels
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }
	
	float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
	
	//lettura di tutti i pixel
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

    //delay di un secondo
    delay(1);
}


/**
	Metodo di stampa della matrice
*/
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
	getPixel(convertedMatrix); //lettura dei pixel
	//semplificazione della matrice
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

/**
	@matrix: matrice presa in input 8x8
	@return: true se la matrice in input contiene almeno un 3
*/
bool Pixel :: containsThree(int matrix[ARRAY_SIZE][ARRAY_SIZE]){
	for(int i = 0; i < ARRAY_SIZE; i++){
		for(int j = 0; j < ARRAY_SIZE; j++){
			if(matrix[i][j] == 3)
				return true;
		}
	}
		return false;
}

/**
	@matrix: matrice presa in input 2x8
	@return: true se la matrice in input contiene almeno un 3
*/
bool Pixel :: containsThreeSplitted(int matrix[2][ARRAY_SIZE]){
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < ARRAY_SIZE; j++){
			if(matrix[i][j] == 3)
				return true;
		}
	}
		return false;
}

/**
	@matrix: matrice in input
	@return: il numero di pixel = 3 letti	
*/
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
	@matrix: matrice iniziale
	@splittedMatrix: matrice divisa in righe o colonne da 2
	@c
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

/**
	Metodo di analisi della matrice
	@matrix: matrice da analizzare
	@gestureVal: valore iniziale della gesture
*/
int Pixel :: matrixAnalysis(int matrix[ARRAY_SIZE][ARRAY_SIZE], int gestureVal){
	int splittedMatrixLeft[2][ARRAY_SIZE];	//matrice splittata a sx
	int splittedMatrixRight[2][ARRAY_SIZE];	//matrice splittata a dx
	int splittedMatrixBottom[2][ARRAY_SIZE];//matrice splittata dal basso
	int splittedMatrixTop[2][ARRAY_SIZE];	//matrice splittata dall'alto
	matrixSplit(matrix, splittedMatrixLeft, 'l');
	matrixSplit(matrix, splittedMatrixRight, 'r');
	matrixSplit(matrix, splittedMatrixBottom, 'b');
	matrixSplit(matrix, splittedMatrixTop, 't');
	//non è ancora stato ipotizzato un gesto
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
		//se sembra dal basso all'alto
		else if(containsThreeSplitted(splittedMatrixBottom) && !containsThreeSplitted(splittedMatrixTop) && numThreeSplitted(splittedMatrixBottom)){
			Serial.println("BOT");
			return 3;
		}
		//se sembra dall'alto al basso
		else if(containsThreeSplitted(splittedMatrixTop) && !containsThreeSplitted(splittedMatrixBottom) && numThreeSplitted(splittedMatrixTop)){
			Serial.println("TOP");
			return 4;
		}
		//non è stato riconosciuto nessun gesto
		else
			return 0;
	}
	//sembra essere riconosciuto da SX a DX
	else if(gestureVal == 1){
		//se il calore si è spostato a DX
		if(containsThreeSplitted(splittedMatrixRight) && !containsThreeSplitted(splittedMatrixLeft)){
			Serial.println("SX-Recognized");
			//viene inviato al bluetooth il comando corrispondente
			bluetooth.sendCommand("MEDIANEXT");
			return 11;
		}
		//non si è ancora certi del gesto
		else 
			return 1;
	}
	//sembra essere riconosciuto da DX a SX
	else if(gestureVal == 2){
		//se il calore si è spostato a SX
		if(containsThreeSplitted(splittedMatrixLeft) && !containsThreeSplitted(splittedMatrixRight)){
			Serial.println("DX-Recognized");
			//viene inviato al bluetooth il comando corrispondente
			bluetooth.sendCommand("MEDIAPREVIOUS");
			return 22;
		}
		//non si è ancora certi del gesto
		else 
			return 2;
	}
	//sembra essere riconosciuto da BOT a TOP
	else if(gestureVal == 3){
		//se il calore si è spostato a TOP
		if(containsThreeSplitted(splittedMatrixTop) && !containsThreeSplitted(splittedMatrixBottom)){
			Serial.println("BOT-Recognized");
			//viene inviato al bluetooth il comando corrispondente
			bluetooth.sendCommand("VOLUME+");
			return 33;
		}
		//non si è ancora certi del gesto
		else 
			return 3;
	}
	//sembra essere riconosciuto da TOP a BOT
	else if(gestureVal == 4){
		//se il calore si è spostato a BOT
		if(containsThreeSplitted(splittedMatrixBottom) && !containsThreeSplitted(splittedMatrixTop)){
			Serial.println("TOP-Recognized");
			//viene inviato al bluetooth il comando corrispondente
			bluetooth.sendCommand("VOLUME-");
			return 44;
		}
		//non si è ancora certi del gesto
		else 
			return 4;
	}
	//non è stato riconosciuto nessun gesto
	else 
		return 0;
}
