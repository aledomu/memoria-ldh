#define MY_RADIO_RF24
#define MY_RF24_IRQ_PIN (2)
#define MY_RF24_CHANNEL 30
#define MY_NODE_ID 56
#define MY_DEBUG

#define MY_SIGNING_SOFT
#define MY_SIGNING_SOFT_RANDOMSEED_PIN 7
#define MY_SIGNING_REQUEST_SIGNATURES
#define MY_DEBUG_VERBOSE_SIGNING

//#define MY_RF24_ENABLE_ENCRYPTION

#include <MySensors.h>
#include <Bounce2.h>

constexpr uint8_t switch1Id = 1;
constexpr uint8_t switch2Id = 2;
constexpr uint8_t relayId = 3;

constexpr uint8_t switch1Pin = 3;
constexpr uint8_t switch2Pin = 4;
constexpr uint8_t relayPin = 5;

// Dos debouncers para impedir rebote en los switches/botones
Bounce switch1(switch1Pin, 10);
Bounce switch2(switch2Pin, 10);

MyMessage switch1Msg(switch1Id, V_STATUS);
MyMessage switch2Msg(switch2Id, V_STATUS);
MyMessage relayMsg(relayId, V_STATUS);

void presentation() {
  present(switch1Id, S_BINARY);
  present(switch2Id, S_BINARY);
  present(relayId, S_BINARY);
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void loop() {
  if (switch1.update()) {
    send(switch1Msg.set(switch1.read()));
    changeRelayState();
  }

  if (switch2.update()) {
    send(switch2Msg.set(switch2.read()));
    changeRelayState();
  }
}

void changeRelayState(void) {
  int relayState = digitalRead(relayPin);
  // El rele se enciende con la senal en baja y se apaga en alta
  send(relayMsg.set(!relayState));
  digitalWrite(relayPin, !relayState);
}

void receive(const MyMessage &message){
  if (message.getSensor() == relayId && message.type == V_STATUS) {
    digitalWrite(relayPin, message.getBool());

    #ifdef MY_DEBUG
    Serial.print("Cambios en el sensor ID ");
    Serial.print(message.getSensor());
    Serial.print(", nuevo estado: ");
    Serial.println(message.getBool());
    #endif    
  }
}
