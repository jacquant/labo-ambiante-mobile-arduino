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
