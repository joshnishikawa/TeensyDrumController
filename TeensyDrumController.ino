#include "MIDIcontroller.h"
#include "hats.h"

byte MIDIchannel = 10; // 10 commonly used for drums
MIDIdrum* Ds[8];
int FSRvalue = 0;
int PIEZOvalue = 0;
int hit = 0;
int pedal = 0;  // 0 = open, 1 = closed
elapsedMillis timer = 0; // for keeping LED lit long enough to see

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Ds[0] = new MIDIdrum(A0, 36);  //kick 35/36
  Ds[1] = new MIDIdrum(A1, 38);  //snare 38/40
  Ds[2] = new MIDIdrum(A2, 37);  //rimshot 37 / timbale 65/66
  Ds[3] = new MIDIdrum(A3, 48);  //tom 48/50
  Ds[4] = new MIDIdrum(A4, 41);  //floor 41/43
  Ds[5] = new MIDIdrum(A5, 49);  //crash1 49, 57
  Ds[6] = new MIDIdrum(A6, 51);  //ride 51/59 / bell 53
  Ds[7] = new MIDIdrum(A7, 55);  //splash 55 / china 52
  pinMode(A8, INPUT);            //MIDI note TBD by FSR value on pin A9 
  pinMode(A9, INPUT);            //Hi-Hats open 46, pedal 44, closed 42
}

void loop() {
  for (int i=0; i<8; i++){ // read all piezos
    hit = Ds[i]->send() > -1 ? 1 : hit;
  }

  PIEZOvalue = analogRead(A8);
  FSRvalue = analogRead(A9);
  pedal = FSRvalue >= onThreshold ? 1 : 0;
  hit = hatTrigger(PIEZOvalue, pedal) > 0 ? 1 : hit;
  hit = pedalTrigger(FSRvalue) > 0 ? 1 : hit;


  if (hit > 0){
    digitalWrite(13, HIGH);
    timer = 0;
  }
  if (timer >= 30){
    digitalWrite(13, LOW);
  }
  hit = 0;
}

/*TEENSY 3.2 PIN ASSIGNMENTS
      ______
 ____|      |____
| GND|      | 5V |
| 0  |______AGND |  Hats Pedal
| 1 (_)     3.3v | |
| 2 (_)    (_)A9 | + - (A9 can also be used as a drum)
| 3 (_)    (_)A8 | - - Hats
| 4 (_)    (_)A7 | - - Splash
| 5 (_)       A6 | - - Ride
| 6           A5 | - - Crash
| 7           A4 | - - Floor
| 8           A3 | - - Tom
| 9           A2 | - - Rim
| 10          A1 | - - Snare
| 11          A0 | - - Kick
| 12          13 |    LED 0 
|________________|

*/
