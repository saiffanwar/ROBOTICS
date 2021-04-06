#ifndef _LINESENSOR_H
#define _LINESENSOR_H

class lineSensor_c {
  
  public:

    int pin;
    int bias;
    int threshold;

    // Constructor, accepts a pin number and light threshold as
    // argument and sets this as input.
    lineSensor_c( int which_pin, int thresh ) {

       // Record which pin we used.
       pin = which_pin;
       threshold = thresh;

       // Set this pin to input.
       pinMode( pin, INPUT );
    };
    
    // get sensor reading
    int reading() {
      return analogRead(pin);
    }

    // get calibrated sensor reading
    int calibrated_reading() {

      return analogRead(pin) - bias;
    }

    bool on_line() {

      if (calibrated_reading() > threshold){
        return true;
      }
      else {
        return false;
      }
    }

    // Write your calibration routine here
    void calibrate() {
      
      float ave_value = 0;

      for(int i = 0; i < 100; i++) {

        ave_value = ave_value*((float)(i) / (float)(i+1)) + ((float)(analogRead( pin )) / (float)(i+1));
        
      }

      bias = ave_value;
      
    }
};

#endif
