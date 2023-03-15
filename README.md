### TEENSY DRUM CONTROLLER
This is basically a sketch for reading 9 Piezos and an FSR with a Teensy 3.x.
Eight of the Piezos simply use the MIDIcontroller library to send MIDI. The 9th
Piezo works in conjunction with the FSR to emulate (as closely as possible) the
behavior of hi-hats.

* The Piezo will read hits form drum sticks and the FSR is will determine what
those hits should sound like based on whether it's being pressed or not
(note #46 for open hats, note #42 for closed).

* The FSR and Piezo for the hats should be assigned to the same 'choke group' in 
the DAW. That way, when the FSR is pressed, the open hat sound will be silenced.

* In addition, when the FSR goes from unpressed to pressed, a separate footpedal 
sound is sent (note #44). The velocity of this sound will usually be determined
by how hard you press the FSR. However, if an open hat strike is sounding and
the FSR is pressed lightly, the low velocity will sound unnatural. So a higher
velocity is used in this case.

Ideally, multiple closed hat sound banks would be used. The bank would be
determined by how hard the FSR is being pressed. The velocity, of course,
being determined by the strength of the strike.