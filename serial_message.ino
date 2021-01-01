#include "serial_message.h"

#define receiveData
#define wait 30			//Set wait for transmitter to 10 and for receiver to 30 for up to 19 success messages per second

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
#ifdef _USB_
		Serial.println("TIMEOUT/RECEIVE-ERROR");
#endif
	} else {
		tom.printMessage();
	}
	delay(wait);
#else
	paul.sendMessage();
	delay(wait);
#endif
}