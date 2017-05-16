#define dir 8
#define stp 9
#define dly 10

void setup() {
  pinMode(dir,OUTPUT);
  pinMode(stp,OUTPUT);
  //digitalWrite(dir,LOW);
}

void loop() {
  digitalWrite(dir,HIGH);
  delay(dly);
  for(unsigned int i=0;i<20;i++){
    digitalWrite(stp, HIGH);
    delay(dly);
    digitalWrite(stp, LOW);
    delay(dly);
  }
  digitalWrite(dir,LOW);
  delay(dly);
  for(unsigned int i=0;i<20;i++){
    digitalWrite(stp, HIGH);
    delay(dly);
    digitalWrite(stp, LOW);
    delay(dly);
  }
}
