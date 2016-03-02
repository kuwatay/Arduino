#include "HT1621.h"
#include "SLCD_HT1621.h"

SLCD_HT1621 lcd(TS206, 2,3,4,5);

void setup() {
  lcd.begin();
  
  for (long i = - 1000 ; i < 10000 ; i++) {
    lcd.printNum(i);
    lcd.printSNum(abs(i) % 100);
    delay(10);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
