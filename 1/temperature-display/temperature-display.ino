#include <Wire.h>
#include <LiquidCrystal_I2C.h>

constexpr int TEMP_PIN = 0;

constexpr int MAX_ADC_READING = 1023;
constexpr int ADC_REF_VOLTAGE = 5;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  int rawData = analogRead(TEMP_PIN);
  float v = (float) rawData * ADC_REF_VOLTAGE / MAX_ADC_READING;
  float temp = v * 100 - 50;
  
  lcd.clear();
  lcd.print(String(temp, 2) + " C degrees");
  delay(500);
}
