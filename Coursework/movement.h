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
        if(abs(M*100)<1){
          new_powers.l_power = 100;
          new_powers.r_power = 100;
          return new_powers;
        }
        new_powers.l_power = (M*100*-1);
        new_powers.r_power = (M*100*+1);
        return new_powers;
    }

    int findPath(int counter){
        move(15,-15);
        delay(50);
        counter += 1;
        return counter;
    }


        // You can define other functions for
        // yourself. 
        // ...

};

#endif
