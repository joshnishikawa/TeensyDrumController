#include "MIDIcontroller.h"
#include "hats.h"

byte MIDIchannel = 10;
MIDIdrum* Ds[8];
int FSRvalue = 0;
int PIEZOvalue = 0;
int hit = 0;
int pressure = 0;  // 0=open, 1=light, 2=heavy
elapsedMillis timer = 0;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Ds[0] = new MIDIdrum(A0, 56);  //kick 35/36
  Ds[1] = new MIDIdrum(A1, 38);  //snare 38/40
  Ds[2] = new MIDIdrum(A2, 37);  //rimshot 37 / timbale 65/66
  Ds[3] = new MIDIdrum(A3, 48);  //tom 48/50
  Ds[4] = new MIDIdrum(A4, 41);  //floor 41/43
  Ds[5] = new MIDIdrum(A5, 49);  //crash1 49, 57
  Ds[6] = new MIDIdrum(A6, 51);  //ride 51/59 / bell 53
  Ds[7] = new MIDIdrum(A7, 36);  //splash 55 / china 52
  pinMode(A8, INPUT);            //MIDI note to be determined by FSR value on pin A9 
  pinMode(A9, INPUT);            //Hi-Hats open 52, loose 53, closed 54, pedal 55 (not general MIDI numbers)
}

void loop() {
  PIEZOvalue = analogRead(A8);
  FSRvalue = analogRead(A9);
  
  if      (FSRvalue >= heavyThreshold){pressure = 2;}
  else if (FSRvalue >= lightThreshold){pressure = 1;}
  else                                {pressure = 0;};
  
  hit = hatTrigger(PIEZOvalue, pressure) >0?1:hit;
  hit = pedalTrigger(FSRvalue) >0?1:hit;

  for (int i=0; i<8; i++){
    hit = Ds[i]->send() >-1?1:hit;
  }

  if (hit > 0){
    digitalWrite(13, HIGH);
    timer = 0;
  }
  if (timer >= 30){
    digitalWrite(13, LOW);
  }
  hit = 0;
}
