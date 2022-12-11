#include <Servo.h>

constexpr int SERVO_PIN = 9;
constexpr int HIGH_OUT_DPIN = 12;

Servo motor;

void setup() {
  motor.attach(SERVO_PIN);
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
    motor.write(Serial.read());
  }
}
