#ifndef _MOTOR_H
#define _MOTOR_H

// A class to neatly contain commands for the 
// motors, to take care of +/- values, a min/max
// power value, & pin setup.

class motor_c {
  public:

    // define motor pins  
    int DIR;
    int PWM;
    byte power;

    // This is the constructor.
    motor_c( int PWM_pin, int DIR_pin ) {

      DIR = DIR_pin;
      PWM = PWM_pin;
      power = 0;
         
    };

    // motor power
    set_power(int input_power) {

      if (input_power < 0){
        digitalWrite(DIR, HIGH);
        power = -input_power;     
      }
      else {
        digitalWrite(DIR, LOW);
        power = input_power;
      }

      analogWrite(PWM, power);
     
    }
       
};



#endif
