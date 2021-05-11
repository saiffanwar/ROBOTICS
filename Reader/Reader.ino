#include <EEPROM.h>

int printed = 0;
void setup(){
  Serial.begin(9600);
  
}
void loop() {
    Serial.print("################");
    Serial.println();

    // if (printed = 0){
        Serial.print("[");
        for (int i=0; i<EEPROM.length()/2; i++)
        {
        Serial.print("[");
        Serial.print(EEPROM.read(2*i));
        Serial.print(", ");
        Serial.print(EEPROM.read(2*i+1));
        Serial.print("]");
        if (i<(EEPROM.length()/2)-1){
            Serial.print(", ");
        }
        }
        Serial.print("]");

        Serial.println();
        delay(1000);
        printed = 1;
    // }
}