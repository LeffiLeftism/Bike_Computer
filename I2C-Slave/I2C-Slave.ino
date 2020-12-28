#define __AVR_ATtiny84__

#include <Arduino.h>
#include "detectRPM.h"

#if defined(__AVR_ATtiny841__)
#define F_CPU 16000000                          // clock speed: 16MHz (external crystal)
#include "WireS.h"                              // I2C library for ATtiny841 (and other modern ATtinys)
#else
#define F_CPU 20000000                          // clock speed: 20MHz (external crystal)
#include "TinyWireS.h"                          // I2C library for ATtiny84A (and other older ATtinys)
#endif

#define COUNT_TIMINGS 10
#define S_HALL 3;

const byte SLAVE_ADDR = 100;

RPM_Detector pedal;

void setup() {
    TinyWireS.begin(SLAVE_ADDR);
    TinyWireS.onRequest(requestISR);
    pinMode(S_HALL,INPUT_PULLUP);
    pinMode(1,OUTPUT);
}

void loop() {
  pedal.checkForError();
  if(digitalRead(S_HALL)) {
    pedal.addRPM();
    while(digitalRead(S_HALL)) {
      pedal.checkForError();
      digitalWrite(1,HIGH);
    }
    digitalWrite(1,LOW);
  }
}

void requestISR() {
  TinyWireS.write(pedal.rpm);
}
