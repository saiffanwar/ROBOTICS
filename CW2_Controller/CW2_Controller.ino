#define E1_A_PIN  7
#define E1_B_PIN  23
#define E0_A_PIN  26

#include "motor.h"
#include "pid.h"
#include "lineSensor.h"
#include "kinematics.h"
#include "proxSensor.h"
#include <EEPROM.h>


#define l_PWM 10
#define r_PWM  9
#define l_DIR 16
#define r_DIR 15

#define SCALING 2

#define left_pin A3
#define center_pin A2
#define right_pin A4

float target_dist = 100; // mm

 // (P, Int, Deriv)
PID_motor left_controller(80,0,10);
PID_motor right_controller(80,0,10);

PID_heading heading_controller(0,0,0);

motor_c left_motor(l_PWM, l_DIR);
motor_c right_motor(r_PWM, r_DIR);

kinematics_c kinematics;

prox_sensor sensor;

float left_speed;
float right_speed;

float max_speed = 0.15;
float max_power = 30;

float p_x = 50; // proximity sensor location relative to robot center
float p_y = 75; //

float heading = 0;

float left_demand = 0;
float right_demand = 0;

float left_power;
float right_power;
float output;
float dist;
float sample_period = 1000; // how many milliseconds between memory writing

float tick_dist = 140;

int iter_count = 0;
int addr = 0;

// time stamps
unsigned long left_time_stamp;
unsigned long right_time_stamp;
unsigned long time_stamp;

// Volatile Global variables used by Encoder ISR.
volatile long count_e1; // used by encoder to count the rotation
volatile bool oldE1_A;  // used by encoder to remember prior state of A
volatile bool oldE1_B;  // used by encoder to remember prior state of B

volatile long count_e0; // used by encoder to count the rotation
volatile bool oldE0_A;  // used by encoder to remember prior state of A
volatile bool oldE0_B;  // used by encoder to remember prior state of B


void setup() {

  // setup motor encoders
  setupEncoder1();
  setupEncoder0();

  pinMode(LED_BUILTIN, OUTPUT);

  // Start up the serial port.
  Serial.begin(9600);

  Wire.begin();      // Initialise the I2C protocol
  sensor.init();     // initialise the sensor library class.
  sensor.configureDefault();  // the following configure the sensor
  sensor.setScaling(SCALING); // which is achieved through the I2C
  sensor.setTimeout(500);     // protocol. Therefore, it is necessary
                                // to call Wire.begin() first.
  // Delay to connect properly.
  delay(2000);

  // Print a debug, so we can see a reset on monitor.
  Serial.println("***RESET***");

  
  left_power = 0;
  right_power = 0;

  time_stamp = millis();
 
} 

void loop() {
  
  // measure obstacle distance
  dist = sensor.mean_reading(10);

  // calculate heading
  heading = heading_controller.update(target_dist, dist);

  // apply heading to motor demands
  track_heading(heading);


  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  // update motor power
  left_power = left_controller.update(left_demand, left_speed);
  right_power = right_controller.update(right_demand, right_speed);

//  Serial.print(left_power);
//  Serial.print(',');
//  Serial.print(kinematics.y);
//  Serial.print(',');
//  Serial.print(kinematics.x);

  // write kinematics to memory
  if (millis() - time_stamp > sample_period) {

    if (5*(iter_count) <= (EEPROM.length()-4)) {
      Serial.println();

      // linear transform to proximity sensor location
      float x_offset = cos(kinematics.theta)*p_x - sin(kinematics.theta)*p_y;
      float y_offset = sin(kinematics.theta)*p_x + cos(kinematics.theta)*p_y;

      // cast to short int
      short int x_loc = (int)(kinematics.x + x_offset);
      short int y_loc = (int)(kinematics.y + y_offset);
  
      // convert short int to two unsigned char
      unsigned char x_byte_1 = (x_loc & 255);          // 
      unsigned char x_byte_2 = ((x_loc >> 8) & 255);   // Ou: ((num >> 8) & 0xFF);
  
      unsigned char y_byte_1 = (y_loc & 255);          // Ou: (num & 0xFF)
      unsigned char y_byte_2 = ((y_loc >> 8) & 255);   // Ou: ((num >> 8) & 0xFF);
  
      // store x_loc
      EEPROM.write(5*(iter_count), x_byte_1);
      EEPROM.write(5*(iter_count)+1, x_byte_2);
  
      // store y_loc
      EEPROM.write(5*(iter_count)+2, y_byte_1);
      EEPROM.write(5*(iter_count)+3, y_byte_2);
      
      // store sensor reading
      EEPROM.write(5*(iter_count)+4, int(dist));
      // addr = addr + 2;

      iter_count=iter_count+1;

    time_stamp = millis();
    }
    else if (5*(iter_count) >= EEPROM.length()) {
        Serial.println("EEPROM Memory Full");
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
      }
  }
 
  // apply updated power values
  left_motor.set_power((int)(left_power));
  right_motor.set_power((int)(right_power));

  // update kinemtaics
  kinematics.update(count_e0, count_e1);
  // Serial.print(count_e0);
  // Serial.print(',');
  // Serial.print(count_e1);
  // Serial.println();

  Serial.println();
  delay( 5 );
} 


// map heading into individual wheel speed demands
void track_heading(float heading) {
 
  // steer 
  left_demand = max_speed*heading + 0.4*max_speed;
  right_demand = -max_speed*heading + 0.4*max_speed;
}


// This ISR handles just Encoder 1 (right motor)
// executes whenever encoder state changes
ISR( INT6_vect ) {

  boolean newE1_B = digitalRead( E1_B_PIN );
  boolean newE1_A = digitalRead( E1_A_PIN );

  newE1_A ^= newE1_B;

  // forward states; 2, 4, 13, 11
  // bacward states; 1, 8, 14, 7

  // State :  (bit3)  (bit2)  (bit1)  (bit0)
  // State :  New A,  New B,  Old A,  Old B.
  byte state = 0;
  state = state | ( newE1_A  << 3 );
  state = state | ( newE1_B  << 2 );
  state = state | ( oldE1_A  << 1 );
  state = state | ( oldE1_B  << 0 );
  
  float delta_t = micros() - right_time_stamp;
  
  if( state == 4 || state == 2 || state == 13 || state ==  11) {
      count_e1 = count_e1 + 1;  // forward
      right_speed = tick_dist / (float)(delta_t);
  } else if( state == 1 || state == 8 || state == 14 || state == 7 ) {
      count_e1 = count_e1 - 1; // backward
      right_speed = -tick_dist / (float)(delta_t);
  }

  right_time_stamp = micros();
      
  // Save current state as old state for next call.
  oldE1_A = newE1_A;
  oldE1_B = newE1_B;
  
}


// ISR for encoder0 (left motor)
ISR( PCINT0_vect ) {

  boolean newE0_B = PINE & (1<<PINE2);
  //boolean newE0_B = PINE & B00000100;  // Does same as above.

  // Standard read fro the other pin.
  boolean newE0_A = digitalRead( E0_A_PIN ); // 26 the same as A8

  newE0_A ^= newE0_B;

  byte state = 0;                   
  state = state | ( newE0_A  << 3 );
  state = state | ( newE0_B  << 2 );
  state = state | ( oldE0_A  << 1 );
  state = state | ( oldE0_B  << 0 );

  float delta_t = micros() - left_time_stamp;
  
  if( state == 4 || state == 2 || state == 11 || state == 13 ) {
      count_e0 = count_e0 + 1;  // forward
      left_speed = tick_dist / (float)(delta_t);
           
  } else if( state == 7 || state == 14 || state == 8 || state == 1 ) {
      count_e0 = count_e0 - 1; // backward
      left_speed = -tick_dist / (float)(delta_t);
   
  } 
  left_time_stamp = micros();

  // Save current state as old state for next call.
  oldE0_A = newE0_A;
  oldE0_B = newE0_B; 

}

void setupEncoder1() {

  // Initialise our count value to 0.
  count_e1 = 0;

  // Initialise the prior A & B signals
  // to zero, we don't know what they were.
  oldE1_A = 0;
  oldE1_B = 0;

  // Setup pins for encoder 1
  pinMode( E1_A_PIN, INPUT );
  pinMode( E1_B_PIN, INPUT );

  EIMSK = EIMSK & ~(1<<INT6);

  EICRB |= ( 1 << ISC60 );  // using header file names, push 1 to bit ISC60
  //EICRB |= B00010000; // does same as above

  // Page 90, 11.1.4 External Interrupt Flag Register – EIFR
  // Setting a 1 in bit 6 (INTF6) clears the interrupt flag.
  EIFR |= ( 1 << INTF6 );
  //EIFR |= B01000000;  // same as above

  EIMSK |= ( 1 << INT6 );
  //EIMSK |= B01000000; // Same as above

}

void setupEncoder0() {

    // Initialise our count value to 0.
    count_e0 = 0;

    // Initialise the prior A & B signals
    // to zero, we don't know what they were.
    oldE0_A = 0;
    oldE0_B = 0;

    DDRE = DDRE & ~(1<<DDE6);

    PORTE = PORTE | (1<< PORTE2 );
    //PORTE = PORTE | 0B00000100;

    // Encoder0 uses conventional pin 26
    pinMode( E0_A_PIN, INPUT );
    digitalWrite( E0_A_PIN, HIGH ); // Enc

     PCICR = PCICR & ~( 1 << PCIE0 );
    // PCICR &= B11111110;  // Same as above

    // 11.1.7 Pin Change Mask Register 0 – PCMSK0
    PCMSK0 |= (1 << PCINT4);

    // Page 91, 11.1.6 Pin Change Interrupt Flag Register – PCIFR
    PCIFR |= (1 << PCIF0);  // Clear its interrupt flag by writing a 1.

    // Enable
    PCICR |= (1 << PCIE0);
}
