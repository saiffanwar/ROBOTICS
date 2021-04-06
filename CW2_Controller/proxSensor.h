#ifndef _PROXSENSOR_H
#define _PROXSENSOR_H

#include <VL6180X.h>

class prox_sensor : public VL6180X
{
  public:

    // return mean of n sensor readings
    float mean_reading(int n_readings){

      float ave_value;

      for(int i=0;i<n_readings;i++) {

        ave_value = ave_value*((float)(i) / (float)(i+1)) + ((float)(readRangeSingleMillimeters()) / (float)(i+1));
        
      }

     return ave_value;

      
      
    }
  
};


#endif
