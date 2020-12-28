#define COUNT_TIMINGS 10
class RPM_Detector {
  private:
  byte magnets = 1;
  unsigned short rpm_timing[COUNT_TIMINGS];
  byte rpm_index = 0;
  unsigned long timeLastRPM = 0;
  double timing_mean;
  
  public:
  volatile byte rpm = 0;
  
  void addRPM() {
    rpm_timing[rpm_index] = millis() - timeLastRPM;
    if(rpm_index < COUNT_TIMINGS-1) {
      rpm_index++;
    } else {
      rpm_index = 0;
    }
    timeLastRPM = millis();
    calculateRPM();
  }

  void calculateRPM() {
    unsigned short timing_sum = 0;
    byte count = 0;
    for(int i = 0; i < COUNT_TIMINGS; i++) {
      if(rpm_timing[i] != 0) {
        timing_sum += rpm_timing[i];
        count++;
      }
    }
    if(timing_sum != 0 && count > 2){
      timing_mean = timing_sum/count;
      unsigned short tmp = 1000/timing_mean*60/magnets;
      if(tmp > 255) tmp = 255;
      rpm = byte(tmp);
    } else {
      rpm = 0;
    }
  }

  void checkForError() {
    if(millis() - timeLastRPM > 10000) {
      for(int n = 0; n < COUNT_TIMINGS; n++) {
        rpm_timing[n] = 0;
      }
        rpm = 0;
        timeLastRPM = millis();
    }
  }
};
