#include <Wire.h>
#include <LiquidCrystal_I2C.h>

constexpr int HIGH_OUT_DPIN = 12;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  lcd.init();
  lcd.cursor();
  lcd.backlight();

  Serial.begin(9600);

  // Hay que activar un pin de salida en alto para conectarlo al pin de reset
  // y asi evitar el reinicio automatico al abrir el puerto serie desde Linux.
  // Esto ocurre porque Linux envia una senal de control DTR/RTS al abrir el
  // puerto aunque se ajuste para que no envie senales de ese tipo.
  pinMode(HIGH_OUT_DPIN, OUTPUT);
  digitalWrite(HIGH_OUT_DPIN, HIGH);
}

void loop() {
  if (Serial.available()) {
    lcd.write(Serial.read());
  }
}
