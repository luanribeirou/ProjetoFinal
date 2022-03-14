#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

IPAddress server_addr(85, 10, 205, 173);
char user[] = "luanribeiro";
char pas[] = "12345678";
const char* ssid = "Claudio_Rede_Conesul"; //VARIÁVEL QUE ARMAZENA O NOME DA REDE SEM FIO EM QUE VAI CONECTAR
const char* password = "99681141"; //VARIÁVEL QUE ARMAZENA A SENHA DA REDE SEM FIO EM QUE VAI CONECTAR
char insert[] = "INSERT INTO `historico` (`TEMPI`, `TEMPE`, `UMII`, `UMIE`, `MEDIAI`, `MEDIAE`, `COLETA`) VALUES ('%f', '%f', '%f', '%f', '%f', '%f', CURRENT_TIMESTAMP)";
char query[128];
IPAddress ip(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 254); //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
IPAddress subnet(255, 255, 255, 0); //MASCARA DE REDE*/
WiFiClient client;
MySQL_Connection conn((Client *)&client);
float ti = 0, te = 0, ui = 0, ue = 0, mi, me;
int cont;
String str;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE SEM FIO
  WiFi.config(ip, gateway, subnet); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI SETAR O IP FIXO NO NODEMCU
  while (WiFi.status() != WL_CONNECTED) { //ENQUANTO STATUS FOR DIFERENTE DE CONECTADO
    delay(500); //INTERVALO DE 500 MILISEGUNDOS
  }
  while (!conn.connect(server_addr, 3306, user, pas)) {
    conn.close();
    delay(1000);
  }
  cont = 1;
  mi = 0;
  me = 0;
}

void loop() {
  if (Serial.available()) {
    str = Serial.readString();
    ti = str.toFloat();
    Serial.println(ti);
    delay(1000);
    str = Serial.readString();
    te = str.toFloat();
    Serial.println(te);
    delay(1000);
    str = Serial.readString();
    ui = str.toFloat();
    Serial.println(ui);
    delay(1000);
    str = Serial.readString();
    ue = str.toFloat();
    Serial.println(ue);
    delay(1000);
    mi = (mi + ti) / cont;
    me = (me + te) / cont;
    enviaBD();
    cont++;
  }
}

void enviaBD() {
  sprintf(query, insert, ti, te, ui, ue, mi, me);
  MySQL_Cursor *cur = new MySQL_Cursor(&conn);
  cur->execute(query);
  Serial.write("Chego");
  delay(900000);
  return;
}
