#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LowPower.h"

constexpr int LDR_APIN = 0;

constexpr int RX_DPIN = 2;
constexpr int SWITCH_DPIN = 3;
constexpr int HIGH_OUT_DPIN = 12;

constexpr int MAX_ADC_READING = 1023;
constexpr int ADC_REF_VOLTAGE = 5;
constexpr int REF_RESISTANCE = 1000;
constexpr long LUX_CALC_SCALAR = 12518931;
constexpr float LUX_CALC_EXPONENT = -1.405;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

volatile float resistorV;
volatile float ldrLux;

void read_and_send_light(void) {
  int ldrRawData = analogRead(LDR_APIN);
  resistorV = (float) ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  float ldrV = ADC_REF_VOLTAGE - resistorV;
  float ldrR = ldrV / resistorV * REF_RESISTANCE;
  ldrLux = LUX_CALC_SCALAR * pow(ldrR, LUX_CALC_EXPONENT);

  Serial.write((uint8_t*) &resistorV, 4);
  Serial.write((uint8_t*) &ldrLux, 4);
}

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  // Hay que activar un pin de salida en alto para conectarlo al pin de reset
  // y asi evitar el reinicio automatico al abrir el puerto serie desde Linux.
  // Esto ocurre porque Linux envia una senal de control DTR/RTS al abrir el
  // puerto aunque se ajuste para que no envie senales de ese tipo.
  pinMode(HIGH_OUT_DPIN, OUTPUT);
  digitalWrite(HIGH_OUT_DPIN, HIGH);

  pinMode(SWITCH_DPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH_DPIN), read_and_send_light, FALLING);

  pinMode(RX_DPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RX_DPIN), read_and_send_light, FALLING);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting...");
}

void loop() {
  // Hay que activar los conversores ADC para que pueda realizar la lectura
  // apropiada en la rutina de interrupcion
  LowPower.powerDown(SLEEP_FOREVER, ADC_ON, BOD_OFF);

  // El display debe manejarse aqui porque la interfaz I2C funciona por
  // interrupciones, por lo que en una ISR se va a bloquear
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(String(resistorV, 2) + " V");
  lcd.setCursor(0, 1);
  lcd.print(String(ldrLux, 2) + " lux");
}
