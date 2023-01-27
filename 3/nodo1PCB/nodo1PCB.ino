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

constexpr uint8_t relayId = 1;
constexpr uint8_t relayPin = 5;

MyMessage relayMsg(relayId, V_STATUS);

void presentation() {
  present(relayId, S_BINARY);
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
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
