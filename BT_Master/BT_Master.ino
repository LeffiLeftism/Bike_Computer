#include "config.h"
#include "Sensor.h"

void setup() {
#if serial_ausgabe
  Serial.begin(115200);
#else if enable_receiveDatapack_output
  Serial.begin(115200);
#endif
  mySerial.begin(38400);
}


void loop() {
  sensor newSensor;
  newSensor.addr = 15;
  newSensor.data = random(0,250);
  newSensor.error = 5;
  newSensor.requested = random(0,50);
  NewSensorData(0, newSensor);
  printOut(newSensor);
}
