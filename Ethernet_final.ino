#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>
#define DHTTYPE DHT22
#define DHTPININ 7
#define DHTPINEX 6

SoftwareSerial espSerial(0, 1);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //ATRIBUIÇÃO DE ENDEREÇO MAC AO ETHERNET SHIELD W5100
byte ip[] = { 192, 168, 1, 154 }; //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110  **** ISSO VARIA, NO MEU CASO É: 192.168.0.175
byte gateway[] = {192, 168, 1, 254}; //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
byte subnet[] = {255, 255, 255, 0}; //MASCARA DE REDE (ALTERE PARA A SUA MÁSCARA DE REDE)
EthernetServer server(80); //PORTA EM QUE A CONEXÃO SERÁ FEITA
int rele = 5;
DHT_Unified dht_in(DHTPININ, DHTTYPE);
DHT_Unified dht_ex(DHTPINEX, DHTTYPE);

void setup() {
  espSerial.begin(115200);
  Ethernet.begin(mac, ip, gateway, subnet); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE
  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA 80
  dht_in.begin();
  dht_ex.begin();
  sensor_t sensor;
  pinMode(rele, OUTPUT);
}

void loop() {
  String readString = String(30); //VARIÁVEL PARA BUSCAR DADOS NO ENDEREÇO (URL)
  float ti, te, ui, ue;
  int status = 0;
  //CRIA UMA CONEXÃO COM O CLIENTE
  sensors_event_t event_in;
  sensors_event_t event_ex;
  dht_in.temperature().getEvent(&event_in);
  dht_ex.temperature().getEvent(&event_ex);
  dht_in.temperature().getEvent(&event_in);
  ti = (float) event_in.temperature;
  dht_in.humidity().getEvent(&event_in);
  ui = (float) event_in.relative_humidity;
  dht_ex.temperature().getEvent(&event_ex);
  te = (float) event_ex.temperature;
  dht_ex.humidity().getEvent(&event_ex);
  ue = (float) event_ex.relative_humidity;
  EthernetClient client = server.available();
  if (client) { // SE EXISTE CLIENTE, FAZ
    while (client.connected()) {//ENQUANTO EXISTIR CLIENTE CONECTADO, FAZ
      if (client.available()) { //SE O CLIENTE ESTÁ HABILITADO, FAZ
        char c = client.read(); //LÊ CARACTERE A CARACTERE DA REQUISIÇÃO HTTP
        if (readString.length() < 100) //SE O ARRAY FOR MENOR QUE 100, FAZ
        {
          readString += c; // "readstring" VAI RECEBER OS CARACTERES LIDO
        }
        if (c == '\n') { //SE ENCONTRAR "\n" É O FINAL DO CABEÇALHO DA REQUISIÇÃO HTTP
          if (readString.indexOf("?") < 0) { //SE ENCONTRAR O CARACTER "?", FAZ
          }
          else //SENÃO,FAZ
            if (readString.indexOf("ArParam=1") > 0) //SE ENCONTRAR O PARÂMETRO "ledParam=1", FAZ
            {
              digitalWrite(rele, HIGH);
              //LIGA O LED
              status = 1; //VARIÁVEL RECEBE VALOR 1(SIGNIFICA QUE O LED ESTÁ LIGADO)
            } else { //SENÃO, FAZ
              digitalWrite(rele, LOW);
              //DESLIGA O LED
              status = 0; //VARIÁVEL RECEBE VALOR 0(SIGNIFICA QUE O LED ESTÁ DESLIGADO)
            }
          client.println("HTTP/1.1 200 OK"); //ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
          client.println("Content-Type: text/html"); //ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
          client.println("");
          client.println("<!DOCTYPE HTML>"); //INFORMA AO NAVEGADOR A ESPECIFICAÇÃO DO HTML
          client.println("<html>"); //ABRE A TAG "html"
          client.println("<head>"); //ABRE A TAG "head"
          client.println("<meta http-equiv=""refresh"" content=""900000"">");
          client.println("<title>Monitor de temperatura - Controlador de Energia</title>"); //ESCREVE O TEXTO NA PÁGINA
          client.println("</head>"); //FECHA A TAG "head
          client.println("<body style=background-color:#ADD8E6>"); //DEFINE A COR DE FUNDO DA PÁGINA
          client.println("<center><font color='blue'><h1>Monitor de Temperatura e Umidade</font></center></h1>"); //ESCREVE "MASTERWALKER SHOP" EM COR AZUL NA PÁGINa
          client.println("<br></br>");
          client.print("<div text-align:center><center><font size='5'>Temperatura atual na parte interna da maquina:");
          client.print(ti);
          client.print(" C</center></div><br></br>");
          client.print("<div text-align:center><center><font size='5'>Umidade atual na parte interna da maquina:");
          client.print(ui);
          client.print(" %</center></div><br></br>");
          client.print("<div text-align:center><center><font size='5'>Temperatura atual na parte externa da maquina:");
          client.print(te);
          client.print("  C</center></div><br></br>");
          client.print("<div text-align:center><center><font size='5'>Umidade atual na parte externa da maquina:");
          client.print(ue);
          client.print("  %</center></div><br></br>");
          if (digitalRead(rele) == HIGH) {
            client.println("<center><form method=get name=DESLIGAR><input type=hidden name=ArParam value=0 /><input type=submit value=Desligar Ar></form></center>");
          }
          else {
            client.println("<center><form method=get name=LIGAR><input type=hidden name=ArParam value=1 /><input type=submit value=Ligar Ar></form></center>");
          }
          client.println("<center><font size='5'>Status atual do Ar Condicionado: </center>"); //ESCREVE "Status atual do LED:" NA PÁGINA
          if (status == 1) { //SE VARIÁVEL FOR IGUAL A 1, FAZ
            client.println("<center><font color='green' size='5'>Ar Condicionado Ligado</center>"); //ESCREVE "LIGADO" EM COR VERDE NA PÁGINA
          } else { //SENÃO, FAZ
            client.println("<center><font color='red' size='5'>Ar condicionado Desligado</center>"); //ESCREVE "DESLIGADO" EM COR VERMELHA NA PÁGINA
          }
          if (te > 30.0) {
            digitalWrite(rele, LOW);
            client.println("<br></br><center><font color='red' size='8'>Ar condicionado desligado automaticamente devido a alta temperatura na sala - sera automaticamente religado em 15 minutos</center>");
            delay(900000);
            digitalWrite(rele, HIGH);
          }
          client.println("</body>"); //FECHA A TAG "body"
          client.println("</html>"); //FECHA A TAG "html"
          readString = ""; //A VARIÁVEL É REINICIALIZADA
          client.stop();
        }
      }
    }
  }
  espSerial.println(ti);
  delay(1000);
  espSerial.println(te);
  delay(1000);
  espSerial.println(ui);
  delay(1000);
  espSerial.println(ue);
  delay(1000);
}
