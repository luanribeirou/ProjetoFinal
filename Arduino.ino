#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22
#define DHTPININ 8
#define DHTPINEX 9

DHT_Unified dht_in(DHTPININ, DHTTYPE);
DHT_Unified dht_ex(DHTPINEX, DHTTYPE);
uint32_t delayMS;
SoftwareSerial espSerial(5, 6);
String str_ti, str_te, str_hi, str_he;

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  dht_in.begin();
  dht_ex.begin();
  sensor_t sensor;
  delayMS = sensor.min_delay * 1500000;

}

void loop() {
  sensors_event_t event_in;
  sensors_event_t event_ex;
  dht_in.temperature().getEvent(&event_in);
  dht_ex.temperature().getEvent(&event_ex);
  /*if (isnan(event_in.temperature) && isnan(event_in.temperature))
    {
    Serial.println("Erro na leitura da Temperatura!");
    }
    else
    {*/
  Serial.print("Temperatura Interna: ");
  Serial.print(event_in.temperature);
  Serial.println(" *C");
  str_ti = String(event_in.temperature) + String("ºC");
  espSerial.println(str_ti);
  delay(1000);
  Serial.print("Temperatura Externa: ");
  Serial.print(event_ex.temperature);
  Serial.println(" *C");
  str_te = String(event_ex.temperature) + String("ºC");
  espSerial.println(str_te);
  delay(1000);
  //}
  dht_in.humidity().getEvent(&event_in);
  dht_ex.humidity().getEvent(&event_ex);
  /*if (isnan(event_in.relative_humidity) && isnan(event_in.relative_humidity))
    {
    Serial.println("Erro na leitura da Umidade!");
    }
    else
    {*/
  Serial.print("Umidade Interna: ");
  Serial.print(event_in.relative_humidity);
  Serial.println("%");
  str_hi = String(event_in.relative_humidity) + String("%");
  espSerial.println(str_hi);
  delay(1000);
  Serial.print("Umidade Externa: ");
  Serial.print(event_ex.relative_humidity);
  Serial.println("%");
  str_he = String(event_in.relative_humidity) + String("%");
  espSerial.println(str_he);
  delay(1000);
  //}
  delay(delayMS);
}
