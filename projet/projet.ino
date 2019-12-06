
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoHttpClient.h>

#include <NMEAGPS.h>


NMEAGPS gps;
gps_fix fix;



float lattitude;
float longitude;


//SoftwareSerial gpsSerial(rxGps,txGps);
String stringGps = "";


char ssid[] = "ricardo wifi";
char pass[] = "Teafourtwo";

int status = WL_IDLE_STATUS;
WiFiClient client; //initialize the wifi client library

char server[] = "51.91.99.16";   //mettre l'addrese du serveur ici  possiblement rajouter :8181
int port = 8181;

HttpClient Http_client = HttpClient(client,server,port);

// partie non wifi

int buttonPin = 2;
int ledPin = 12;
int soundPinDigi = 3;
int soundPinAna = A0;
int soundAlimPin = 5;

int valDigi = 0;
int valAna = 0;
int buttonState = 0;

int buttonPushed =0;
int onAir =0;
int lock =0;

int soundLevel = 0;


void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin,INPUT);
  pinMode(soundPinDigi,INPUT);
  pinMode(soundPinAna, INPUT);
  pinMode(soundAlimPin,OUTPUT);

  wifiConnect();



  Serial.begin(9600);
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
  soundLevel =0; 
  digitalWrite(soundAlimPin,HIGH);
  int duration = 10000;
  while(duration > 0){
    valAna = analogRead(soundPinAna);
    Serial.println(valAna);
    duration = duration -1;
    soundLevel = soundLevel + valAna;
  }
  soundLevel = soundLevel/10000;
  Serial.println("Le niveau moyen de son est :");
  Serial.println(soundLevel);
  digitalWrite(soundAlimPin,LOW);
  return;
}



void TurnOnAir(){
  Serial.println("turned on air");
  lock = 1;
  onAir =1;                     // on air activated => need to send http post
  digitalWrite(ledPin,HIGH);    //led on because on air
  record();

  httpRequest_post();

  //getGpsLocation();

    return;
  }
  

  


void TurnOffAir(){
  Serial.println("turned off air");
  lock = 1;
  onAir =0;
  digitalWrite(ledPin,LOW);
  httpRequest_delete();
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


void httpRequest_post(){
  String postData = "{\"id\": \"Boitier_1\", \"title\": \"Bip boup je suis un arduino\", \"organizers\": \"Bip boup entertainment\", \"start_time\": \"none\",\"end_time\" : \"none\", \"description\": \"boop boop bap bip\", \"category\": \"electro\", \"zip_code\": \"5000\", \"city\": \"Namur\", \"street\": \"Grand Gagnage\", \"street_number\" : \"69\", \"phone\": \"118218\", \"mail\" : \"arduinono@bipboup.com\", \"website\" : \"bipboup.com\", \"lat\": 0.0, \"lon\" : 0.0, \"source\" : \"*****\", \"sound_level\" : 62}";
  String contentType = "application/json";

  Serial.println("connecting to server ...");
  
  Http_client.post("/events",contentType,postData);

  
  int statusCode = Http_client.responseStatusCode();
  String response = Http_client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  
 
}

void httpRequest_delete(){
  String postData = "";
  String contentType = "";

  Serial.println("connecting to server ...");
  
  Http_client.del("/events/Boitier_1",contentType,postData);

  
  int statusCode = Http_client.responseStatusCode();
  String response = Http_client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  
 
}


void getGpsLocation(){
  Serial.println("getting location ...");
  if(gps.available()){
    fix = gps.read();
    Serial.println(fix.latitude());
  }
  else{
    delay(1000);
    getGpsLocation();
  }
  
}

  
