#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* ssid = "Claudio_Rede_Conesul";
const char* password = "99681141";
IPAddress ip(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(80);
String str;
int rele = 23;
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  delay(100);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
  pinMode(rele, OUTPUT);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<br></br><br></br><br></br>");
            client.println("<div text-align:center><h1><center>Monitor de Temperatura e Umidade</center></h1></div><br></br>");
            if (Serial.available()) {
              client.print("<div text-align:center><center><font size='5'>O que ta chegando: ");
              str = (String) Serial.read();
              client.print(str);
              client.print("</center></div><br></br>");
            }
            if (digitalRead(rele) == LOW) {
              client.print("<center><a href=\"/D\"><input type=""button"" text-align:center value=""Desligar Ar Condicionado""></a></center><br>");
            }
            else {
              client.print("<center><a href=\"/L\"><input type=""button"" text-align:center value=""Ligar Ar Condicionado""></a></center><br>");
            }
            client.println("</html>");
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }
      }
      if (currentLine.endsWith("GET /L")) {
        digitalWrite(rele, LOW);
      }
      if (currentLine.endsWith("GET /D")) {
        digitalWrite(rele, HIGH);
      }
    }
  }
}
