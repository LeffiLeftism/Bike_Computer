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
  NewSensorData(0, newSensor);
  printOut(newSensor);
  delay(3000);
}
