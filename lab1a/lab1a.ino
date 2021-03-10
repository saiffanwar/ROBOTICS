int total_flashes = 0;
int counter = 0;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //Start a serial connection
  Serial.begin(9600);

  // Wait for stable connection, report reset.
  delay(1000);
  Serial.println("***RESET***");
}

// the loop function runs over and over again forever
void loop() {
  flash();
  Serial.println("Hello, from the Romi");
  if ( Serial.available() ) {

      char inChar = Serial.read(); //This reads one byte

      if (inChar =='s') {
          Serial.println("Got s");
      }

  }
}

void flash() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(300);                       // wait for a second
  total_flashes = total_flashes + 1;
  Serial.println(total_flashes);

//  if (total_flashes%5 == 0) {
//    counter = 0;
//    while(counter <=255){
//    play_tone(counter);
//    delay(10);
//    counter = counter + 1;
//    }
//  }
}

void play_tone(int volume) {
  analogWrite(6, volume);
}
