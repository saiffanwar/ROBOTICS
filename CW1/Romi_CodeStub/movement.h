#ifndef _MOVEMENT_H
#define _MOVEMENT_H


#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN  9
#define R_DIR_PIN 15

class movement_c {
  
  public:

    int pin;

    // Constructor, accepts a pin number as
    // argument and sets this as input.
    movement_c( int which_pin ) {
       // Record which pin we used.
    //    pin = which_pin;
       // Set this pin to input.
    //    pinMode( pin, INPUT );
    };

    void move(double r_power, double l_power) {
        setRightMotorDirection(r_power);
        setLeftMotorDirection(l_power);
        if ((int abs(l_power)) <= 15 && (int abs(l_power)) != 0) {
            l_power = 15;
        }
        if ((int abs(r_power)) <= 15 && (int abs(l_power)) != 0){
            r_power = 15;
        }
        analogWrite(L_PWM_PIN, int abs(l_power));
        analogWrite(R_PWM_PIN, int abs(r_power));


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
    

};
    

#endif
