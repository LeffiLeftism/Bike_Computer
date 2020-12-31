#include "serial_message.h"

#define receiveData
#define wait 30

message paul;
message tom;

void setup () {
#ifdef _USB_
	Serial.begin(_BAUDRATE_);
#endif
	mySerial.begin(_BAUDRATE_);
	byte joe[] = {0,1,2,3,4,5};
	paul.setData(joe, sizeof(joe));
}

void loop () {
#ifdef receiveData
	if(!tom.receiveMessage()) {
		Serial.println("TIMEOUT/RECEIVE-ERROR");
	} else {
		tom.printMessage();
	}
	delay(wait);
#else
	paul.sendMessage();
	delay(wait);
#endif
}