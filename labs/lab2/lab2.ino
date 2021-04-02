#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN  9
#define R_DIR_PIN 15

// minimum power to go forwards right = 11
// minimum power to go forwards left = 11
// minimum power to go backwards right = 10
// minimum power to go backwards left = 10

//
void setup() {

  // Set our motor driver pins as outputs.
  pinMode( L_PWM_PIN, OUTPUT );
  pinMode( L_DIR_PIN, OUTPUT );
  pinMode( R_PWM_PIN, OUTPUT );
  pinMode( R_DIR_PIN, OUTPUT );

  // Set initial direction for l and r
  digitalWrite( L_DIR_PIN, LOW  );
  digitalWrite( R_DIR_PIN, HIGH );

  // Start up the Serial for debugging.
  Serial.begin(9600);
  delay(1000);
  // Print reset so we can catch any reset error.
  Serial.println(" ***Reset*** ");

}

float l_power = 0;
float r_power = 0;
String acceleration = "speeding up";

void loop() {

  move(20, -20);
  delay(1800);
  move(-20,20);
  delay(3600);
  move(20, -20);
  delay(1800);
}

struct Powers {
    float r_power;
    float l_power;
};

void cycle_speed() {
  if (acceleration == "speeding up") {
    struct Powers new_powers = speed_up(l_power, r_power);
    r_power = new_powers.r_power;
    l_power = new_powers.l_power;
    if ((r_power == 255) && (l_power == 255)){
      acceleration = "slowing down";
    }
  }
  else if (acceleration == "slowing down") {
    struct Powers new_powers = slow_down(l_power, r_power);
    r_power = new_powers.r_power;
    l_power = new_powers.l_power;
    if ((r_power == 0) && (l_power == 0)) {
      acceleration = "speeding up";
    }
  }
}

struct Powers slow_down(int l_power_old, int r_power_old) {
  
      float l_power_new = l_power_old - 5;
      float r_power_new = r_power_old - 5;
      analogWrite( L_PWM_PIN, l_power_new );
      analogWrite( R_PWM_PIN, r_power_new );
      delay(250);
      
      struct Powers new_powers;
      new_powers.l_power = l_power_new;
      new_powers.r_power = r_power_new;
      return new_powers;
}

struct Powers speed_up(float l_power_old, float r_power_old) {

      float l_power_new = l_power_old + 5;
      float r_power_new = r_power_old + 5;
      analogWrite( L_PWM_PIN, l_power_new );
      analogWrite( R_PWM_PIN, r_power_new );
      delay(250);
      
      struct Powers new_powers;
      new_powers.l_power = l_power_new;
      new_powers.r_power = r_power_new;
      return new_powers;
}

void move(double r_power, double l_power) {
  setRightMotorDirection(r_power);
  setLeftMotorDirection(l_power);
  // int i = 0;
  // while (i < time) {
  if ((int abs(l_power)) <= 11 && (int abs(l_power)) != 0) {
    l_power = 11;
  }
  if ((int abs(r_power)) <= 11 && (int abs(l_power)) != 0){
    r_power = 11;
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
