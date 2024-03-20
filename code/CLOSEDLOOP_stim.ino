//CLOSED LOOP CONTROL

//AD9833 Libraries
#include <AD9833.h>     // Include the library
#define FNC_PIN 10       // Can be any digital IO pin
//Distance Sensor Libraries
#include <Wire.h>
#include "SparkFun_VL53L1X.h"

//Moving Average Filter
#define WINDOW_SIZE 5
int INDEX = 0;
int avg_distance = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;

//Digital Pot Library
#include <DigiPotX9Cxxx.h>

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);
 
AD9833 gen(FNC_PIN);       // Defaults to 25MHz internal reference frequency
 
//DigiPot pot(6,7,8);

int frequency = 40;
void setup() {
   //WAVEGEN
    gen.Begin(); 
    //function specifying waveform here:             
    gen.ApplySignal(SINE_WAVE,REG0,frequency);  
    gen.EnableOutput(true);   

    //DISTANCE SENSOR
    Wire.begin();

    Serial.begin(9600);
     Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.init() == false)
    Serial.println("Sensor online!");
}

//With the VOUT pin of the AD9833 to A0 pin of Arduino, we can view the waveforms from the device.
void loop(void) {
  gen.ApplySignal(SINE_WAVE,REG0,frequency);
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.stopRanging();
 float distanceInches = distance * 0.0393701;
  float distanceFeet = distanceInches / 12.0;
  
  //MOVING AVERAGE FILTER
  SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
  avg_distance = distance;        // Read the next sensor value
  READINGS[INDEX] = avg_distance;           // Add the newest reading to the window
  SUM = SUM + avg_distance;                 // Add the newest reading to the sum
  INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  AVERAGED = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
  
  Serial.println();
  Serial.print("Filtered ");
  Serial.println(AVERAGED);
  Serial.print("Distance(ft): ");
  Serial.println(distance);
  delay(25); 

  if (AVERAGED<600)
    frequency = 40;
  else if (AVERAGED>600 && AVERAGED<3000)
    frequency = (3800-AVERAGED)/80.0;
  else if (AVERAGED>3000)
    frequency = 10; 
}
