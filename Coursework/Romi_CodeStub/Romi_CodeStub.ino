/*
       @@@@@@@@@@@&*           %@@@@@%       @@@@@@@@@    @@@@@@@@@  @@@@@@@@
       @@@@@@@@@@@@@@@     #@@@@@@@@@@@@    @@@@@@@@@@   @@@@@@@@@* @@@@@@@@@
       @@@@@@   @@@@@@   /@@@@@%  .@@@@@@    @@@/@@@@@ @@@@@@@@@@    @@@@@@
      &@@@@@##&@@@@@@   @@@@@@(   @@@@@@@   @@@,.@@@@@@@@,.@@@@@    @@@@@@
      @@@@@@@@@@@@@    &@@@@@@    @@@@@@   @@@@  @@@@@@@  @@@@@    (@@@@@
     @@@@@@  @@@@@@*   @@@@@@    @@@@@@   .@@@   @@@@@#  @@@@@@    @@@@@&
   @@@@@@@@   @@@@@@%  .@@@@@@@@@@@@@    @@@@@%  @@@@  @@@@@@@@  @@@@@@@@
  %@@@@@@@&   @@@@@@     #@@@@@@@@      @@@@@@   @@@   @@@@@@@/ @@@@@@@@%

  Provided by Paul O'Dowd Oct 2020
*/


// The following files should all appear in
// tabs above.  They are incomplete and match
// up to exercie labsheets provided.
#include "lineSensor.h"
#include "encoders.h"
// #include "kinematics.h"
// #include "motor.h"
// #include "pid.h"
#include "movement.h"

#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN  9
#define R_DIR_PIN 15
#define R_ENCODER 7
#define L_ENCODER 8

#define CLOCKWISE 0
#define ANTICLOCKWISE 1
// Setup, only runs once when the power
// is turned on.  However, if your Romi
// gets reset, it will run again.

#define LINE_LEFT_PIN A4
#define LINE_CENTRE_PIN A3
#define LINE_RIGHT_PIN A2

lineSensor_c line_left(LINE_LEFT_PIN);
lineSensor_c line_centre(LINE_CENTRE_PIN);
lineSensor_c line_right(LINE_RIGHT_PIN);
movement_c movement(A4);

int l_cal; // left sensor
int c_cal; // centre sensor
int r_cal; // right sensor

int l_value; // left sensor
int c_value; // centre sensor
int r_value; // right sensor

int l_status;
int c_status;
int r_status;
int count = 0;
int rotation_counter = 0;
int found_line = 2;
int direction = ANTICLOCKWISE;
double l_power = 0;
double r_power = 0;

struct Powers {
    float r_power;
    float l_power;
};

struct Turning { 
  int direction;
  int count;
};

struct Encodings {
  int r_enc_count;
  int l_enc_count;
  int enc_direction;
};
int current_r_enc;
int current_l_enc;
int r_enc_count;
int l_enc_count;

int full_rotation_counter = 0;
String cur_state = "initial";

void setup() {
  // Start up the serial port.
  Serial.begin(9600);

  // Delay to connect properly.
  // delay(3000);

  pinMode(6, OUTPUT);
  l_cal = line_left.calibrate();
  c_cal = line_centre.calibrate();
  r_cal = line_right.calibrate();
  // You can open either Serial monitor or plotter.
  digitalWrite(LED_BUILTIN, HIGH);
  current_r_enc = digitalRead(R_ENCODER);

  current_l_enc = digitalRead(L_ENCODER);
  setupEncoder0();
  setupEncoder1();
//   delay(1000);
//   analogWrite(6, 0);

  Serial.println("***RESET***");
} // end of setup()

void loop() {

  l_value = analogRead( LINE_LEFT_PIN );
  c_value = analogRead( LINE_CENTRE_PIN );
  r_value = analogRead( LINE_RIGHT_PIN );
  l_status = line_left.onLine(l_value - l_cal, 150);
  // Serial.print("l_status: ");
  // Serial.print(l_status);
  // Serial.print( "\n" );  
  c_status = line_centre.onLine(c_value - c_cal, 150);
  // Serial.print("c_status: ");
  // Serial.print(c_status);
  // Serial.print( "\n" );  
  r_status = line_right.onLine(r_value - r_cal, 150);
  // Serial.print("r_status: ");
  // Serial.print(r_status);
  // Serial.print( "\n" );

  // Serial.print("line found?: ");
  // Serial.print(found_line);
  // Serial.print("\n");
  struct Encodings encodings = read_encoders(r_enc_count, l_enc_count, current_r_enc, current_l_enc);
  r_enc_count = encodings.r_enc_count;
  l_enc_count = encodings.l_enc_count;
  // Serial.print("line found: ");
  // Serial.print(found_line);
  // Serial.print("\n");
  Serial.print("right count: ");
  Serial.print(count_e1);
  Serial.print("\n");
  Serial.print("left count: ");
  Serial.print(count_e0);
  Serial.print("\n");

  if ((count_e1 >= -2000) && (count_e0 >= -2000)){
    movement.move(20,20);
  }
  else{
    movement.move(0,0);
  }

  if (found_line == 0){
    if ((l_status == 1) || (c_status == 1) || (r_status == 1)){
      movement.move(20,-20);
      delay(50);
      found_line = 1;
    }
    else {
      movement.move(20,20);
    }
  }
  else if (found_line == 1){
      cur_state = state_finder(l_status, c_status, r_status);
      // Serial.print(cur_state);
      // Serial.print("\n");
      if (cur_state == "on line"){
        movement.move(20,20);
        count = 0;
        full_rotation_counter = 0;
        direction = ANTICLOCKWISE;
      }
      else if (cur_state == "turn onto path"){
        count = 0;
        full_rotation_counter = 0;
        direction = ANTICLOCKWISE;
        struct Powers powers = weighted_sensing(l_value, c_value, r_value);
        
        // Serial.print("r_power: ");
        // Serial.print(powers.r_power);
        // Serial.print( "\n" );  
        // Serial.print("l_power: ");
        // Serial.print(powers.l_power);
        // Serial.print( "\n" );

        movement.move(powers.r_power, powers.l_power);
      }
      else if (cur_state == "find path"){
        if (full_rotation_counter >= 71){
          movement.move(20,20);
        }
        else {
          struct Turning new_vals = pathfinder(direction, count);
          direction = new_vals.direction;
          count = new_vals.count;
          full_rotation_counter += 1;
        }
        // if (count == 18){}
      }
  }
  delay(50);
} // end of loop()


String state_finder(int l_status, int c_status, int r_status){
  String state = "";
  if((l_status == 1) && (c_status == 1) && (r_status == 1)){
    state = "on line";
  }
  else if((l_status == 1) || (r_status == 1)){
    state = "turn onto path";
  }
  else if((l_status == 0) && (r_status == 0)){
    state = "find path";
  }
  return state;
}


struct Powers weighted_sensing(int left, int centre, int right){
    float readings[3] = {left, centre, right};
    float proportional;
    float I_total = left+centre+right;
    int max_power = 200;
    double M = ((readings[0]/I_total) - readings[2]/I_total);
    struct Powers new_powers;
    new_powers.l_power = (M*max_power*-1);
    new_powers.r_power = (M*max_power*+1);
    return new_powers;
  }


struct Turning pathfinder(int direction, int count){
  // When turning to and from the left.
  // if (count <= 17){
    if(count == -17){
      direction = CLOCKWISE;
    }
  // }
  // if (count >= 18){
    if (count == 17){
      direction = ANTICLOCKWISE;
    }
  if (direction == ANTICLOCKWISE){
    movement.move(20,-20);
    count -=1;
  }
  if (direction == CLOCKWISE){
    movement.move(-20,20);
    count += 1;
  }
  delay(50);
  struct Turning new_vals;
  new_vals.direction = direction;
  new_vals.count = count;
  return new_vals;
}

struct Encodings read_encoders(int r_enc_count, int l_enc_count, int prevr_enc, int prevl_enc){
  int r_enc = digitalRead(R_ENCODER);
  int l_enc = digitalRead(L_ENCODER);
  struct Encodings new_encodings;
  if (r_enc != prevr_enc){
    new_encodings.r_enc_count = r_enc_count + 1;
    new_encodings.enc_direction = ANTICLOCKWISE;
  }
  if (l_enc != prevl_enc){
    new_encodings.l_enc_count = l_enc_count + 1;
    new_encodings.enc_direction = CLOCKWISE;

  }
  current_r_enc = r_enc;
  current_l_enc = l_enc;
  return new_encodings;
  }

