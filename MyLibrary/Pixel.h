// ensure this library description is only included once
#ifndef Pixel_h
#define Pixel_h
#define	ARRAY_SIZE	8

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

// library interface description
class Pixel
{
  // user-accessible "public" interface
  public:
    Pixel(void);
    void getPixel(float matrix[ARRAY_SIZE][ARRAY_SIZE]);
	void printMatrix(int matrix[ARRAY_SIZE][ARRAY_SIZE]);
	void matrixConverter(int matrix[ARRAY_SIZE][ARRAY_SIZE]);
	//ritorna true se viene letto almeno un valore = 3
	bool containsThree(int matrix[ARRAY_SIZE][ARRAY_SIZE]);
	bool containsThreeSplitted(int matrix[2][ARRAY_SIZE]);
	void matrixSplit(int matrix[ARRAY_SIZE][ARRAY_SIZE], int splittedMatrix[2][ARRAY_SIZE], char c);
	int matrixAnalysis(int matrix[ARRAY_SIZE][ARRAY_SIZE], int gestureVal);
	//legge tot matrici e riconosce un'eventuale gesture.
	//int convertPixel(float*);
//	float* isGesture(float);
};

#endif

