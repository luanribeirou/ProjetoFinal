
int porta_rele1 = 23;


void setup() {
  pinMode(porta_rele1, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
 digitalWrite(porta_rele1, LOW);
}
