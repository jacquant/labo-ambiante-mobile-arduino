#include <SPI.h>
#include <WiFi101.h>


char ssid[] = "nomDuReseau";
char pass[] = "motDePasse";
int status = WL_IDLE_STATUS;
WiFiClient client; //initialize the wifi client library

char server[] = "www.aduino.cc";   //mettre l'addrese du serveur ici

// partie non wifi

int buttonPin = 1;
int ledPin = 13;
int soundPinDigi = 3;
int soundAlimPin = 5;

int valDigi = 0;
int buttonState = 0;

int buttonPushed =0;
int onAir =0;
int lock =0;


void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin,INPUT);
  pinMode(soundPinDigi,INPUT);
  pinMode(soundAlimPin,OUTPUT);

}

void loop() {
  buttonState = digitalRead(buttonPin);
  if(buttonState == LOW){
    buttonPushed = 1; 
  }
  else{
    buttonPushed = 0;
  }
  if(buttonPushed ==1 & onAir == 1 & lock ==0){  //case turn off air
    TurnOffAir();
  }
  if(buttonPushed ==1 & onAir == 0 & lock==0){  // case of turn on air
    TurnOnAir();
  }
  else{
    delay(200);
  }
  lock = 0;
}

//record sound in digital for a duration defined inside the variable duration
void record(){ 
  digitalWrite(soundAlimPin,HIGH);
  int duration = 50000;
  while(duration > 0){
    valDigi = digitalRead(soundPinDigi);
    Serial.println(valDigi);
    duration = duration -1;
  }
  digitalWrite(soundAlimPin,LOW);
  return;
}

void TurnOnAir(){
  Serial.println("turned on air");
  lock = 1;
  onAir =1;                     // on air activated => need to send http post
  digitalWrite(ledPin,HIGH);    //led on because on air
  record();
  return;
}

void TurnOffAir(){
  Serial.println("turned off air");
  lock = 1;
  onAir =0;
  digitalWrite(ledPin,LOW);
  return;
}

void wifiConnect(){
  while(status != WL_CONNECTED){
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid,pass);
    delay(1000);
  }
  Serial.println("connection established");
}

void printWifiStatus(){
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP address : ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI) :");
  Serial.print(rssi);
  Serial.print("  Dbm");
}


void httpRequest(){
  String request;
  if(onAir == 1){
    request = "POST /... je passe en ligne HTTP/1.1";
    }
  else{
    request = "POST/ ... je ne suis plus en ligne HTTP/1.1";
  }

  if(client.connect(server,80)){
    Serial.println("connected to server");
    Serial.println(request);
    //token part etc
    Serial.println("Authorization : Token *****");
    Serial.println("Host : *****");
    Serial.println("Connection : close");
    Serial.println();
  }
  else{
    Serial.println("connection to server failed");
  }
  
  
}
