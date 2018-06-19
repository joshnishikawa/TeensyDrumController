#ifndef hats_h
#define hats_h

#include "Arduino.h"

extern byte MIDIchannel;
extern int hit;

////////// USER ASSIGNABLE /////////////////////////////////////////////////////
unsigned int waitTime = 36; // in millis to filter aftershock triggers
int PIEZOthreshold = 30;    // lowest value that will trigger a hit
int offThreshold   = 50;   // to eliminate jittery triggers of the FSR
int lightThreshold = 200;   // FSR value that indicates 'loose' hats
int heavyThreshold = 950;   // FSR value that indicates 'closed' hats

////////// NOT USER ASSIGNABLE /////////////////////////////////////////////////
int PIEZOstate = 0;             // 0=idle, 1=looking for peak, 2=ignoring aftershock
int PIEZOpeak = 0;
elapsedMillis PIEZOtimer = 0;
unsigned int hatsLevel = 0;
int FSRstate = 0;               // 0=idle, 1=looking for peak, 2=ignoring aftershock
int FSRpeak = 0;
elapsedMillis FSRtimer = 0;
elapsedMillis velocityMatchingTimer = 0; //so pedal won't trigger sounds much lower than the hits

int pedalTrigger(int v){/////////////////////////////////////////////////
  if (FSRstate == 0) {
    if (v > lightThreshold) {
      FSRstate = 1;
      FSRpeak = v;
      FSRtimer = 0;
    }
    v = 0; //still just listening
  }
  else if (FSRstate == 1) {
    if (v > FSRpeak) {
      FSRpeak = v;
      v = 0;
    }
    else if (FSRtimer >= 10) {
      v = map(FSRpeak, lightThreshold, 950, 1, 127);
      usbMIDI.sendNoteOn(55, max(v, hatsLevel), MIDIchannel); // note on
      FSRstate = 2;
      FSRtimer = 0;
    }
    else{v = 0;}
  }
  else {
    if (v > offThreshold) {
      FSRtimer = 0; // keep resetting timer if above offThreshold
    }
    else if (FSRtimer > waitTime) {
      usbMIDI.sendNoteOn(55, 0, MIDIchannel); // note off
      FSRstate = 0; // back to idle after pedal's released & waitTime has passed
    }
    v = 0;
  }
  return v;
};


int hatTrigger(int v, int p){///////////////////////////////////////
  static int prsr; // to send the correct 'note off' regardless of pressure change
  if(hatsLevel != 0 && velocityMatchingTimer >= 30){hatsLevel -= 1; velocityMatchingTimer = 0;}
  if (PIEZOstate == 0) {
    if (v > PIEZOthreshold) {
      prsr = p;
      PIEZOstate = 1;
      PIEZOpeak = v;
      PIEZOtimer = 0;
    }
    v = 0;
  }
  else if (PIEZOstate == 1) {
    if (v > PIEZOpeak) {
      PIEZOpeak = v;
      v = 0;
    }
    else if (PIEZOtimer >= 10) {
      v = map(PIEZOpeak, PIEZOthreshold, 1023, 1, 127);
      usbMIDI.sendNoteOn(52+prsr, v, MIDIchannel); // note on
      hatsLevel = v;
      PIEZOstate = 2;
      PIEZOtimer = 0;
      velocityMatchingTimer = 0;
    }
    else{v = 0;}
  }
  else {
    if (v > PIEZOthreshold) {
      PIEZOtimer = 0; // keep resetting timer if above threshold
    }
    else if (PIEZOtimer > waitTime) {
      PIEZOstate = 0; //go back to idle after a certain interval below threshold
      usbMIDI.sendNoteOn(52+prsr, 0, MIDIchannel); // note off
    }
    v = 0;
  }
  return v;
};


#endif
