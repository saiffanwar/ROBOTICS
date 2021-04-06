#ifndef _PID_h
#define _PID_h

// Base class for PID controllers 
class PID_c {
  public:
  
    // Constructor, not order of P I & D arguments when calling.
    PID_c(float P, float I, float D)
    {
      //Store the gains
      setGains(P, I, D);
      //Set last_millis
      reset();
    };
    
    // This function updates the values of the gains
    void setGains(float P, float I, float D) {
      Kp = P;
      Kd = D;
      Ki = I;
    };
    
    // Useful to remove any intergral wind-up
    void reset() {
      last_error      = 0;
      integral_error  = 0;
      Kp_output       = 0;
      Ki_output       = 0;
      Kd_output       = 0;
      last_millis     = millis();
    };                                
    
    void printComponents()
    {
      //Serial.print(Kp_output*100);
      //Serial.print(", ");
      //Serial.print(Kd_output);
      //Serial.print(", ");
      //Serial.print(Ki_output);
      //Serial.print(", ");
      Serial.print(output_signal);
    }; 
   
    float Kp_output = 0;
    float Ki_output = 0;
    float Kd_output = 0;

    /* Private functions and variables are defined here. These functions / variables cannot be accessed from outside the class.
       For example, if we try to set the value of Kp in the file "Romi.h", we will get an error (Try it out!)
    */

    //Control gains
    float Kp; //Proportional
    float Ki; //Integral
    float Kd; //Derivative

    //Values to store
    float output_signal = 0;
    float last_error;
    float error = 0; 
    float error_delta = 0;
    float integral_error = 0; //For storing the integral of the error
    unsigned long last_millis = 0;

};


class PID_motor : public PID_c
{
  // inherit parent constructor.
  using PID_c::PID_c;
  
  public:

    float update(float demand, float measurement) {
    
      long time_now = millis();
      long diff_time = time_now - last_millis;
      last_millis = time_now;
      
      float time_delta = (float)diff_time; // change in time
    
      // Calculate error between demand and measurement.
      float error = demand - measurement;
    
      //This represents the error derivative
      float error_delta = error - last_error;
      last_error = error;
    
      // Integral term.
      integral_error += error*time_delta;
    
      //Calculate P,I,D Term contributions.
      Kp_output = Kp*error;
      
      Kd_output = Kd*error_delta; 
      Ki_output = Ki*integral_error; 
    
      //Add the three components to get the total output
      output_signal = output_signal + Kp_output + Kd_output + Ki_output;
    
      return output_signal;
      
    }
};

class PID_heading : public PID_c
{
  // inherit parent constructor.
  using PID_c::PID_c;
  
  public:

    float update(float demand, float measurement) {
    
      long time_now = millis();
      long diff_time = time_now - last_millis;
      last_millis = time_now;
      
      float time_delta = (float)diff_time; // change in time
    
      // Calculate error between demand and measurement.
      float error = demand - measurement;
    
      //This represents the error derivative
      float error_delta = error - last_error;
      last_error = error;
    
      // Integral term.
      integral_error += error*time_delta;
    
      //Calculate P,I,D Term contributions.
      Kp_output = Kp*error;
      
      Kd_output = Kd*error_delta; 
      Ki_output = Ki*integral_error; 
    
      //Add the three components to get the total output
      //output_signal = output_signal + Kp_output + Kd_output + Ki_output;

      output_signal = 1 - (measurement / demand);

      if (output_signal > 1) {
        return 1;
      }
      else if (output_signal < -1) {
        return -1;
      }
      else {
    
        return output_signal;
      }
      
    }
};


#endif
