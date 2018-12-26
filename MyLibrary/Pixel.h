// ensure this library description is only included once
#ifndef Pixel_h
#define Pixel_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

// library interface description
class Pixel
{
  // user-accessible "public" interface
  public:
    Pixel(void);
    void getPixel(float matrix[8][8]);
	//legge tot matrici e riconosce un'eventuale gesture.
	//int convertPixel(float*);
//	float* isGesture(float);
};

#endif

