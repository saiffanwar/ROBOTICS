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
#include "movement.h"

#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN  9
#define R_DIR_PIN 15


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

int l_cal, c_cal, r_cal;

int l_value, c_value, r_value; // right sensor
int l_status, c_status, r_status; // right sensor

int count = 0;
int rotation_counter = 0;
int turning_counter = 0;
int found_line = 0;
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
int turning_status = 0;
int original_l = 0;
int original_r = 0;


bool reached_destination = false;
int full_rotation_counter = 0;
int break_num = 0;
int theta = 0;
String cur_state = "initial";

void setup() {
  // Start up the serial port.
  Serial.begin(9600);

  // Delay to connect properly.
  delay(3000);

  pinMode(6, OUTPUT);
  l_cal = line_left.calibrate();
  c_cal = line_centre.calibrate();
  r_cal = line_right.calibrate();

  digitalWrite(LED_BUILTIN, HIGH);
  analogWrite(6,25);
  delay(1000);
  analogWrite(6, 0);

  // Serial.println("***RESET***");
} // end of setup()

void loop() {

  l_value = analogRead( LINE_LEFT_PIN );
  c_value = analogRead( LINE_CENTRE_PIN );
  r_value = analogRead( LINE_RIGHT_PIN );

  l_status = line_left.onLine(l_value - l_cal, 100);
  r_status = line_right.onLine(r_value - r_cal, 100);
  c_status = line_centre.onLine(c_value - c_cal, 150);

  if (found_line == 0){
    if ((l_status == 1) || (c_status == 1) || (r_status == 1)){
      movement.move(-40,40);
      delay(150);
      found_line = 1;
    }
    else {
      movement.move(40,35);
    }
  }
  else if (found_line == 1){
      cur_state = state_finder(l_status, c_status, r_status);

      if (cur_state == "on line"){
        movement.move(40,35);
        count = 0;
        full_rotation_counter = 0;
        direction = ANTICLOCKWISE;
        turning_status = 0;
        turning_counter = 0;
      }
      else if (cur_state == "turn onto path"){
        count = 0;
        full_rotation_counter = 0;
        direction = ANTICLOCKWISE;
        struct Powers powers = weighted_sensing(l_value, c_value, r_value);
        movement.move(powers.r_power, powers.l_power);
        turning_status = 0;
        turning_counter = 0;
      }
      else if (cur_state == "find path"){

        if (full_rotation_counter == 0){
          movement.move(20,20);
          delay(50);
        }
        if (full_rotation_counter >= 32){
          if (break_num == 0){
            movement.move(40,35);
            delay(500); 
            break_num += 1;
          }
          else{
            movement.move(0,0);
            found_line = 2;
          }
        }
        else {
          struct Turning new_vals = pathfinder(direction, count);
          direction = new_vals.direction;
          count = new_vals.count;
          full_rotation_counter += 1;
        }
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
    int max_power = 100;
    double M = ((readings[0]/I_total) - readings[2]/I_total);
    struct Powers new_powers;
    new_powers.l_power = (M*max_power*-1);
    new_powers.r_power = (M*max_power*+1);
    return new_powers;
  }

struct Turning pathfinder(int direction, int count){
  if(count == -8){
    direction = CLOCKWISE;
  }
  if (count == 8){
    direction = ANTICLOCKWISE;
  }
  if (direction == ANTICLOCKWISE){
    movement.move(-60,60);
    count -=1;
  }
  if (direction == CLOCKWISE){
    movement.move(60,-60);
    count += 1;
  }
  delay(20);
  struct Turning new_vals;
  new_vals.direction = direction;
  new_vals.count = count;
  return new_vals;

}
