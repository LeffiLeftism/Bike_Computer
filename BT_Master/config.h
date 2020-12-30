#define Software_RX 10
#define Software_TX 11
#define serial_ausgabe false
#define enable_receiveDatapack_output false
#include <SoftwareSerial.h>
SoftwareSerial mySerial(Software_RX, Software_TX); //RX, TX
