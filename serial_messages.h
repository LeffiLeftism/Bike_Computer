// This file includes structur and function to send and receive serial message as via a bluetooth modul

#define _RX_PIN_ 10               //RX_pin for serial communication
#define _TX_PIN_ 11               //TX_pin for serial communication
#define _ORIGIN_DEVICE_ADDR_ 0x01 //Origin address for communication
#define _DEST_DEVICE_ADDR_ 0x02   //Destination address for communication
#define _ARDUINO_                 //define _ARDUINO_ or _ATTINY_ for right communication-library
#define _USB_                     //if defined, serial USB-communication is activated
#define _BAUDRATE_ 38400          //Bautrate for serial communication

#define _ADDR_LENGTH_ 1           //bytes for one address
#define _DATA_LENGTH_ 6           //bytes for data
#define _CHECKSUM_LENGTH_ 2       //bytes for checksum
#define _MESSAGE_LENGTH_ 2*_ADDR_LENGTH_+_DATA_LENGTH_+_CHECKSUM_LENGTH_ //total message length


#ifdef _ARDUINO_
#include <SoftwareSerial.h>           //library for arduino boards to enable software serial communication
SoftwareSerial mySerial(_RX_PIN_, _TX_PIN_);  //bind serial communication to RX and TX pins
#else if _ATTINY_
#include <SoftSerial.h>             //library for digispark boards to enable software serial communication
#include <TinyPinChange.h>            //library needed to change interrupts on digispark board
SoftSerial mySerial (_RX_PIN_, _TX_PIN_);   //bind serial communication to RX and TX pins
#endif



typedef struct message {
  private:
    byte originAddr = _ORIGIN_DEVICE_ADDR_;
    byte destAddr = _DEST_DEVICE_ADDR_;
    bool createdMessageArray = 0;               //if checkSums and messageArray are created, its true. So they don't need to be created again for this message.
    unsigned long mID = 0;              //Message ID, to connect request and awnser. This is set when requesting a message.
    byte data[_DATA_LENGTH_];
	byte messageArray [_MESSAGE_LENGTH_];

  public:


  private:
    void createMessageArray () {
      this->messageArray[0] = this->originAddr;
      this->messageArray[1] = this->destAddr;
      for (int n = 0; n < _DATA_LENGTH_; n++) {
        this->messageArray[n + 2] = this->data[n];
      }
      createCheckSum();   //writes sum1 and sum2 in last two messageArray positions
      this->createdMessageArray = true;
    }

    void createCheckSum () {
      byte sum1 = 0;
      byte sum2 = 0;
      for (int n = 0; n < _MESSAGE_LENGTH_ - _CHECKSUM_LENGTH_; n++) {
        sum1 += this->messageArray[n];
        sum2 += sum1;
      }
      sum1 %= 255;
      sum2 %= 255;
	  
      this->messageArray[_MESSAGE_LENGTH_ - 2] = sum1;
      this->messageArray[_MESSAGE_LENGTH_ - 1] = sum2;
      /*
        data-Array structure:
        + originAddr
        + destAddr
        + data
        + sum1 (checkSum1)
        + sum2 (checkSum2)
      */
    }

	public:
#ifdef _USB_
    void printMessage() {
      Serial.println("Message-Print");
      Serial.print("originAddr: ");
      Serial.println(originAddr);
      Serial.print("destAddr: ");
      Serial.println(destAddr);
      Serial.print("Created MA: ");
      Serial.println(createdMessageArray);
      for(int n = 0; n < _DATA_LENGTH_; n++) {
        Serial.print("Data [");
        Serial.print(n);
        Serial.print("]: ");
        Serial.println(data[n]);
      }
      for(int n = 0; n < _MESSAGE_LENGTH_; n++) {
        Serial.print("Message [");
        Serial.print(n);
        Serial.print("]: ");
        Serial.println(messageArray[n]);
      }
    }
#endif

    void setData (byte *data, int len) {
      auto lengthDiff = _DATA_LENGTH_ - len;
      if(lengthDiff == 0) {
        for(int n = 0; n < len; n++) {
          this->data[n] = *data;
          data++;
        }
      } else if(lengthDiff > 0) {
        for(int n = 0; n < len; n++) {
          this->data[n] = *data;
          data++;
        }
        for(int n = len; n < _DATA_LENGTH_; n++) {
          this->data[n] = 0;
        }
      } else if(lengthDiff < 0) {
        
      }
      this->createdMessageArray = false;
    }
    
    void sendMessage () {
      if(!this->createdMessageArray) {createMessageArray();}
      printMessage();
    }

    void receiveMessage () {

    }
};




message paul;

void setup () {
#ifdef _USB_
  Serial.begin(_BAUDRATE_);
#endif
  mySerial.begin(_BAUDRATE_);
  byte joe[] = {0,1,2,3,4,5};
  paul.setData(joe, sizeof(joe));
}


void loop () {
  paul.sendMessage();
  delay(1000);
}