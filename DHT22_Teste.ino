#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22
#define DHTPIN 12

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

void setup() {
  Serial.begin(9600);                             // monitor serial 9600 bps
  dht.begin();                                    // inicializa a função
  Serial.println("Usando o Sensor DHT");
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);           // imprime os detalhes do Sensor de Temperatura
  Serial.println("------------------------------------");
  Serial.println("Temperatura");
  Serial.print  ("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print  ("Valor max:    ");
  Serial.print(sensor.max_value);
  Serial.println(" *C");
  Serial.print  ("Valor min:    ");
  Serial.print(sensor.min_value);
  Serial.println(" *C");
  Serial.print  ("Resolucao:   ");
  Serial.print(sensor.resolution);
  Serial.println(" *C");
  Serial.println("------------------------------------");
  dht.humidity().getSensor(&sensor);            // imprime os detalhes do Sensor de Umidade
  Serial.println("------------------------------------");
  Serial.println("Umidade");
  Serial.print  ("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print  ("Valor max:    ");
  Serial.print(sensor.max_value);
  Serial.println("%");
  Serial.print  ("Valor min:    ");
  Serial.print(sensor.min_value);
  Serial.println("%");
  Serial.print  ("Resolucao:   ");
  Serial.print(sensor.resolution);
  Serial.println("%");
  Serial.println("------------------------------------");
  delayMS = sensor.min_delay / 1000;            // define o atraso entre as leituras

}

void loop() {
  delay(delayMS);                               // atraso entre as medições
  sensors_event_t event;                        // inicializa o evento da Temperatura
  dht.temperature().getEvent(&event);           // faz a leitura da Temperatura
  if (isnan(event.temperature))                 // se algum erro na leitura
  {
    Serial.println("Erro na leitura da Temperatura!");
  }
  else                                          // senão
  {
    Serial.print("Temperatura: ");              // imprime a Temperatura
    Serial.print(event.temperature);
    Serial.println(" *C");
  }
  dht.humidity().getEvent(&event);              // faz a leitura de umidade
  if (isnan(event.relative_humidity))           // se algum erro na leitura
  {
    Serial.println("Erro na leitura da Umidade!");
  }
  else                                          // senão
  {
    Serial.print("Umidade: ");                  // imprime a Umidade
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
}
