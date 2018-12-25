/*----- Libraries -----*/
#include "Stepper.h"
#include "IRremote.h"

/*----- Variables, Pins -----*/
#define STEPS  32   // Number of steps per revolution of Internal shaft
int  Steps2Take = 0;  // 2048 = 1 Revolution
int receiver = 12; // Signal Pin of IR receiver to Arduino Digital Pin 6
int buttonApin = 2;
int buttonBpin = 3;

/*-----( Declare objects )-----*/
// Setup of proper sequencing for Motor Driver Pins
// In1, In2, In3, In4 in the sequence 1-3-2-4

Stepper small_stepper(STEPS, 8, 10, 9, 11);
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

void setup()
{ 
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  
  pinMode(buttonApin, INPUT_PULLUP);
  pinMode(buttonBpin, INPUT_PULLUP);

  Serial.println("Start !");
}

void rotate(int value){
  if (value != 0){
    //Serial.print("value: ");
    //Serial.println(value);
    
    small_stepper.setSpeed(500); //Max seems to be 500
    small_stepper.step(value);

    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);   
  }
}

void loop()
{
  // have we received an IR signal?
  if (irrecv.decode(&results)) 
  {
    //Serial.print("IR signal: ");
    //Serial.println(results.value);
    
      switch(results.value)
      {
        case 0xFFA857: // VOL- button pressed
          Serial.println("Rorate +");
          Steps2Take  =  32;  // Rotate CW
          break;
        case 0xFF629D: // VOL+ button pressed
          Serial.println("Rorate -");
          Steps2Take  =  -32;  // Rotate CCW
          break;
        case 16712445:
          Serial.println("Rorate stop");
          Steps2Take = 0;
          break;
      }
      irrecv.resume(); // receive the next value
  }

  if (digitalRead(buttonApin) == LOW)
  {
    Serial.println("Rorate break A");
    Steps2Take = 0;
  }

  if (digitalRead(buttonBpin) == LOW)
  {
    Serial.println("Rorate break B");
    Steps2Take = 0;
  }

  //Serial.print("Steps2Take: ");
  //Serial.println(Steps2Take);
  rotate(Steps2Take);

}/* --end main loop -- */
