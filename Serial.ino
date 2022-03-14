#include <SoftwareSerial.h>

SoftwareSerial espSerial(5, 6);
String str;

void setup() {
  // put your setup code here, to run once:
  espSerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  str = (String) "Comunicacao";
  espSerial.println(str);
  delay(5000);
}
