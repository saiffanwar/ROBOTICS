#include <EEPROM.h>

int printed = 0;
void setup(){
  Serial.begin(9600);
  
  
}
void loop() {

//     // wipe
//    for (int i=0; i<EEPROM.length(); i++) {
//      EEPROM.write(i, 0);
//    }

  
    Serial.print("################");
    Serial.println();

    Serial.print("[");
    
    for (int i=0; i<EEPROM.length()/5; i++)
    {
      Serial.print("[");
      Serial.print(EEPROM.read(5*i));
      Serial.print(", ");
      Serial.print(EEPROM.read(5*i+1));
      Serial.print(", ");
      Serial.print(EEPROM.read(5*i+2));
      Serial.print(", ");
      Serial.print(EEPROM.read(5*i+3));
      Serial.print(", ");
      Serial.print(EEPROM.read(5*i+4));

      Serial.print("]");
      
      if (i<(EEPROM.length()/5)-1){
          Serial.print(", ");
      }
    }
    Serial.print("]");

    Serial.println();

    printed = printed + 1;

//    if(printed > 10){
//       // wipe
//      for (int i=0; i<EEPROM.length(); i++) {
//        EEPROM.write(i, 0);
//  }
      
    
    delay(1000);
// }
}
