
#include "2004LCD.h"
LiquidCrystal_I2C lcd(0x27, 20, 4);
char lines[4][21];

void updateLine(int line, char output[]) {
  sprintf(lines[line], "%-20s", output);
}

void updateLCD() {
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0,i);
    lcd.print(lines[i]);
  }
}

void setupLCD() {
  lcd.init();
  lcd.backlight();
}
