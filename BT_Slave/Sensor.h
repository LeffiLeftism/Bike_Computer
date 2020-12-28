
typedef struct sensor {
  byte addr;
  byte data;
  byte error = 0;
  byte requested = 0;
  byte fletch1;
  byte fletch2;
};


void printOut(sensor s);
void sendSensorData(byte type, sensor s);
bool calcFletchSum(sensor& s, bool compare);
void receiveSensorData(bool& shareData, bool& request, bool& rerun);
void sendSensorData(byte type, sensor s);
void requestSensor();
void NewSensorData(bool mode, sensor& s);

void printOut(sensor s) {
#if enable_receiveDatapack_output
  calcFletchSum(s, 0);
  Serial.println("------------------");
  Serial.print("Sensor Addr: ");
  Serial.println(s.addr);
  Serial.print("Sensor Data: ");
  Serial.println(s.data);
  Serial.print("Error occur: ");
  Serial.println(s.error);
  Serial.print("Requested  : ");
  Serial.println(s.requested);
  Serial.print("FletchSum 1: ");
  Serial.println(s.fletch1);
  Serial.print("FletchSum 2: ");
  Serial.println(s.fletch2);
  Serial.println("------------------");
#endif
}

void sendSensorData(byte type, sensor s) {
  //noInterrupts();
  //Serial.println("Send");
  mySerial.write(type);
  calcFletchSum(s, 0);
  mySerial.write(s.addr);
  mySerial.write(s.data);
  mySerial.write(s.error);
  mySerial.write(s.requested);
  mySerial.write(s.fletch1);
  mySerial.write(s.fletch2);
  //interrupts();
}

void receiveSensorData(bool& shareData, bool& request, bool& rerun) {
  byte data[6];
  byte num = 0;
  static byte attemps = 0;
#if serial_ausgabe
  static unsigned int fail = 0;
  static unsigned long trys = 0;
  static unsigned long succ = 0;
  static unsigned long time1 = millis(), time2;
#endif
  sensor nextS;
  if (mySerial.available() > 0) {
#if serial_ausgabe
    trys++;
#endif
    mySerial.readBytes(&num, 1);
    mySerial.readBytes(data, 6);
  } else {
#if serial_ausgabe
    Serial.println("W8ing");
#endif
    attemps++;
    if (attemps > 10) {
      rerun = true;
      attemps = 0;
#if serial_ausgabe
      Serial.println("Rerun");
#endif
    }
  }

  switch (num) {
    case 1:
      nextS.addr = data[0];
      nextS.data = data[1];
      nextS.error = data[2];
      nextS.requested = data[3];
      nextS.fletch1 = data[4];
      nextS.fletch2 = data[5];
      if (!calcFletchSum(nextS, 1)) {
        rerun = true;
        attemps = 0;
#if serial_ausgabe
        fail++;
        Serial.println("FAIL");
#endif
      } else {
        request = false;
        rerun = false;
        attemps = 0;
#if serial_ausgabe
        succ++;
        Serial.println("Success");
        printOut(nextS);
        if (succ % 1000 == 0) Serial.println(succ);
#endif
      }
      break;
    case 251:
      nextS.addr = data[0];
      nextS.data = data[1];
      nextS.error = data[2];
      nextS.requested = data[3];
      nextS.fletch1 = data[4];
      nextS.fletch2 = data[5];
      shareData = calcFletchSum(nextS, 1);
      break;
    default:
      break;
  }

#if serial_ausgabe
  if (trys % 100 == 0 && !request) {
    double perc = double(fail) / double(trys) * 100;
    double deltaT = double(time2) / double(1000);
    time2 = millis() - time1;
    time1 = millis();
    Serial.print("Tries|Fails: ");
    Serial.print(trys);
    Serial.print(" | ");
    Serial.print(fail);
    Serial.print("   ");
    Serial.print("%-Failed|deltaT(ms): ");
    Serial.print(perc, 3);
    Serial.print(" | ");
    Serial.println(deltaT, 3);
  }
#endif
}

bool calcFletchSum(sensor& s, bool compare) {
  //Using Fletcher's Checksum calculation. More information -> https://de.wikipedia.org/wiki/Fletcher%E2%80%99s_Checksum
  short sum1 = 0;
  short sum2 = 0;
  sum1 = (sum1 + s.addr) % 255;
  sum2 = (sum2 + sum1) % 255;
  sum1 = (sum1 + s.data) % 255;
  sum2 = (sum2 + sum1) % 255;
  sum1 = (sum1 + s.error) % 255;
  sum2 = (sum2 + sum1) % 255;
  sum1 = (sum1 + s.requested) % 255;
  sum2 = (sum2 + sum1) % 255;
  if (compare) {
    if (sum1 == s.fletch1 && sum2 == s.fletch2) {
      return true;
    } else {
      return false;
    }
  } else {
    s.fletch1 = sum1;
    s.fletch2 = sum2;
    return;
  }
}

void requestSensor() {
  sensor s;
  s.addr = 0;
  s.data = 5;
  s.error = 5;
  s.requested = 1;
  sendSensorData(251, s);
}

void NewSensorData(bool mode, sensor& s) {
  static bool shareData = false;
  static bool request = false;
  static bool rerun = false;
  if (mode) {
    if (!request || rerun) {
#if serial_ausgabe
      Serial.println("Start");
#endif
      requestSensor();
      request = true;
      rerun = false;
    } else {
      receiveSensorData(shareData, request, rerun);
    }
  } else {
    if (!shareData) {
#if serial_ausgabe
      Serial.println("Receiv");
#endif
      receiveSensorData(shareData, request, rerun);
    } else {
#if serial_ausgabe
      Serial.println("Send");
#endif
      sendSensorData(1, s);
      shareData = false;
    }
  }
  delay(5);
}
