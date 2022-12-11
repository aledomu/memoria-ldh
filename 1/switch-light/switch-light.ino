constexpr int LED_PIN = 2;
constexpr int HIGH_OUT_DPIN = 12;

void setup() {
  pinMode(LED_PIN, OUTPUT);
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
    switch (Serial.read()) {
      case 'h':
        digitalWrite(LED_PIN, HIGH);
        break;
      case 'l':
        digitalWrite(LED_PIN, LOW);
        break;
      default:
        break;
    }
  }
}
