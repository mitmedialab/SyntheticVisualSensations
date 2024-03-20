//OPEN LOOP CONTROL

#include <AD9833.h>     // Include the library
#define FNC_PIN 10       // Can be any digital IO pin

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

AD9833 gen(FNC_PIN);       // Defaults to 25MHz internal reference frequency

int frequency = 10;
void setup() {
   //WAVEGEN
    gen.Begin(); 
    //function specifying waveform here:             
    gen.ApplySignal(SINE_WAVE,REG0,frequency);  
    gen.EnableOutput(true);   

    Serial.begin(9600);
     Serial.println("VL53L1X Qwiic Test");
}

void loop() {
}
