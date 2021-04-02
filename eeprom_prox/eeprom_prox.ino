
/** 
There are two modes to this file which can be defined in the setup.
for mode "write":
This file stores the values of the proximity sensor into the EEPROM Memory of the Arduino. 
It can store upto 1kB and once it is full it will flash the builtin LED on the ROMI and print that the memory is full. 

for mode "read"
This file will display all the values stored in the EEPROM along with the location in memory.
**/

#include <EEPROM.h>
#include <Wire.h>
#include <VL6180X.h>
VL6180X sensor;

int addr = 0;
byte val;
String mode;
int printed;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    sensor.init();
    sensor.configureDefault();
    sensor.setTimeout(500);
    printed = 0;
    mode = "read";
}

void loop() {

    // Serial.print(mode);
    // Serial.println(printed);
  if (mode == "write"){
    if (addr <= (EEPROM.length()-1)) {
      int val = sensor.readRangeSingleMillimeters();
      EEPROM.write(addr, val);
      Serial.print(addr);
      Serial.print("\t");
      Serial.print(val, DEC);
      Serial.println();

      addr = addr + 1;
    }

    else if (addr == EEPROM.length()) {
      Serial.println("EEPROM Memory Full");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }
  else if ((mode == "read")){// && (printed == 0)){

      for (int i = 0; i < EEPROM.length(); i++) {
        val = EEPROM.read(i);
        Serial.print(i);
        Serial.print(": ");
        Serial.print(val, DEC);
        Serial.print("\n");
        delay(100);
      }
      // printed = 1;
  }

  delay(100);
}
