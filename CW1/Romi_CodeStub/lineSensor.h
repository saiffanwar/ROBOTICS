#ifndef _LINESENSOR_H
#define _LINESENSOR_H


#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN  9
#define R_DIR_PIN 15

class lineSensor_c {
  
  public:

    int pin;

    // Constructor, accepts a pin number as
    // argument and sets this as input.
    lineSensor_c( int which_pin ) {
       // Record which pin we used.
       pin = which_pin;
       // Set this pin to input.
       pinMode( pin, INPUT );
    };


    // Write your calibration routine here
    // to remove bias offset
    float calibrate() {
      float value;
      // To help get you started, we use 
      // the pin set/recorded by the 
      // constructor to do an analogRead.
      int no_of_samples = 0;
      float mean = 0;
      while (no_of_samples < 1000) {
        no_of_samples +=1;
        value = analogRead( pin );
        mean = ((mean*(no_of_samples-1)) + value)/no_of_samples;
      }
      return value;
    }


    // Write a routine here to check if your
    // sensor is on a line (true or false).
    int onLine(int bias_reading, float threshold ) {
      if (bias_reading > threshold) {
         return 1;
      }
      else {
        return 0;
      }   
    }

    float l_power = 0;
    float r_power = 0;

    
    void move(double r_power, double l_power) {
      setRightMotorDirection(r_power);
      setLeftMotorDirection(l_power);
      // int i = 0;
      // while (i < time) {
      if ((int abs(l_power)) <= 15 && (int abs(l_power)) != 0) {
        l_power = 15;
      }
      if ((int abs(r_power)) <= 15 && (int abs(l_power)) != 0){
        r_power = 15;
      }
      analogWrite(L_PWM_PIN, int abs(l_power));
      analogWrite(R_PWM_PIN, int abs(r_power));
      // delay(1);
      // i = i + 1;

      // }

    }

    void setLeftMotorDirection(float power) {
      if (power <= 0) {
        digitalWrite(L_DIR_PIN, HIGH);
      }
      else if (power > 0) {
        digitalWrite(L_DIR_PIN, LOW);
      }
    }


    void setRightMotorDirection(float power) {
      if (power <= 0) {
        digitalWrite(R_DIR_PIN, HIGH);
      }
      else if (power > 0) {
        digitalWrite(R_DIR_PIN, LOW);
      }
    }

    struct Powers {
        float r_power;
        float l_power;
    };

    struct Powers weighted_sensing(int left, int centre, int right, int i){
        float readings[3] = {left, centre, right};
        float proportional;
        float I_total = left+centre+right;
        double M = ((readings[0]/I_total) - readings[2]/I_total);
        
        struct Powers new_powers;
        new_powers.l_power = (M*100*-1);
        new_powers.r_power = (M*100*+1);
        return new_powers;
      }


    // You can define other functions for
    // yourself. 
    // ...

};

#endif
