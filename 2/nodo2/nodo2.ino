#define MY_RADIO_RF24
#define MY_RF24_IRQ_PIN (2)
#define MY_RF24_CHANNEL 30
#define MY_NODE_ID 57
// #define MY_DEBUG
 
#include <MySensors.h>
#include <DHT.h>
#include <Vcc.h>

constexpr int DHT_DATA_PIN = 3;
constexpr int SENSOR_TEMP_CELSIUS_OFFSET = 0;
constexpr uint64_t DHT_UPDATE_INTERVAL = 10000;

// Enviar una lectura de la temperatura despues de 10 intentos aunque no haya
// actualizacion
constexpr uint8_t FORCE_UPDATE_N_READS = 10;

constexpr uint8_t humId = 1;
constexpr uint8_t tempId = 2;
constexpr uint8_t batteryId = 3;

DHT dht;

MyMessage humMsg(humId, V_HUM);
MyMessage tempMsg(tempId, V_TEMP);
MyMessage batteryMsg(batteryId, V_VOLTAGE);

void presentation() {
  sendSketchInfo("TemperatureAndHumidity", "1.1");
  present(humId, S_HUM);
  present(tempId, S_TEMP);
  present(batteryId, S_MULTIMETER);
}

void setup() {
  dht.setup(DHT_DATA_PIN);
  // Esperar al periodo minimo de muestreo para evitar un error de timeout en
  // la primera lectura
  sleep(dht.getMinimumSamplingPeriod());
}

constexpr float VccExpected = 3.0;
Vcc vcc(2.860 / 2.92);

void manageBattery(void) {
  float v = vcc.Read_Volts();
  send(batteryMsg.set(v, 1));

  #ifdef MY_DEBUG
  Serial.print("Vcc: ");
  Serial.print(v);
  Serial.println(" V");
  Serial.print("Pcnt: ");
  Serial.print(round(vcc.Read_Perc(VccExpected)));
  Serial.println(" %");
  #endif
}

void manageTemperature(void) {
  static uint8_t nNoUpdatesTemp = 0;
  static float lastTemp;

  float temperature = dht.getTemperature();
  
  if (isnan(temperature)) {
    #ifdef MY_DEBUG
    Serial.println("Failed reading temperature from DHT!");
    #endif
  } else if (temperature != lastTemp || nNoUpdatesTemp == FORCE_UPDATE_N_READS) {
    lastTemp = temperature;

    temperature += SENSOR_TEMP_CELSIUS_OFFSET;
    if (!getControllerConfig().isMetric) {
      temperature = dht.toFahrenheit(temperature);
    }
    
    nNoUpdatesTemp = 0;
    send(tempMsg.set(temperature, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature);
    #endif
  } else {
    nNoUpdatesTemp++;
  }
}

void manageHumidity(void) {
  static uint8_t nNoUpdatesHum = 0;
  static float lastHum;

  float humidity = dht.getHumidity();
  
  if (isnan(humidity)) {
    #ifdef MY_DEBUG
    Serial.println("Failed reading humidity from DHT");
    #endif
  } else if (humidity != lastHum || nNoUpdatesHum == FORCE_UPDATE_N_READS) {
    lastHum = humidity;

    nNoUpdatesHum = 0;
    send(humMsg.set(humidity, 1));

    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity);
    #endif
  } else {
    nNoUpdatesHum++;
  }
}

void loop() {
  // Forzar la lectura del sensor para que tambien funcione despues de sleep()
  dht.resetTimer();

  manageBattery();
  manageTemperature();
  manageHumidity();
  
  sleep(DHT_UPDATE_INTERVAL); 
}
