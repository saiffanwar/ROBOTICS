#ifndef _KINEMATICS_H
#define _KINEMATICS_H


class kinematics_c {
  public:

    float theta; //
    float x; // describe global position
    float y; //

    // these values unique for each romi
    float l = 142; // distance between wheels (encoder counts)
    //float l = 134; // corrected wheel base 

    float const_l = 1; // 1.011467; // wheel diameter correction values
    float const_r = 1; // 0.988533; // 
    float E_s = 1; // / 1.2; // scaling error
    
    int last_left; // previous encoder counts
    int last_right;
    unsigned long time_stamp;

    float constant = 0.152716309549; // convert encoder count to mm (mm / encoder unit)
    //float constant = 0.18325957;
    
    // Function Prototypes
    kinematics_c();   // constructor 
    void update(int left, int right);    // update kinematics
    
    
}; // End of class definition.

kinematics_c::kinematics_c() {
  theta = 0;
  x = 0;
  y = 0;
  last_left = 0;
  last_right = 0;  
}

// Routine to execute the update to
// kinematics 
void kinematics_c::update(int left, int right) {
  
//  float d_left = (float)(left - last_left);
//  float d_right = (float)(right - last_right);
//
//  theta = theta + constant*(d_left-d_right) / 142.0;

  float delta_t = (float)(millis() - time_stamp); // delta t in milliseconds

  // encoder counts to wheel velocities (counts/milli sec)
//  float V_l = (float)(left - last_left) / delta_t;
//  float V_r = (float)(right - last_right) / delta_t;
  
  // corrected
  float V_l = const_l*(float)(left - last_left) / delta_t;
  float V_r = const_r*(float)(right - last_right) / delta_t;


  last_left = left;
  last_right = right;

  float R;

  // angular velocity about ICC
  if (V_r == V_l) { 
    R = 999999;  // handle division by zero exception
  }
  else{
    R = ( l*(V_l + V_r) )/( 2*(V_r - V_l) ); // mm
  }
  
  float omega = ((V_r - V_l)*constant) / l; // rads / milli sec

//  Serial.print(R);
//  Serial.print(',');
//  Serial.print(omega);
//  Serial.println();

  // center of rotation
  float ICC_x = x - R*sin(theta);
  float ICC_y = y + R*cos(theta);
  
  // update pose
  x = ICC_x + E_s*(x - ICC_x)*cos(omega*delta_t) - (y - ICC_y)*sin(omega*delta_t);
  y = ICC_y + E_s*(x - ICC_x)*sin(omega*delta_t) + (y - ICC_y)*cos(omega*delta_t);
  theta = theta + omega*delta_t;

}


#endif
