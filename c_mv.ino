#define p0 0//pin 3,4 -- for USB;
#define p1 1//pin 5 -- not programmed;
#define p2 2
#define dly0 10//dly 10;
#define dly1 20//dly 15;
#define dly2 30//dly 25;

unsigned long cur_mil0;
unsigned long cur_mil1;
unsigned long cur_mil2;
unsigned char fl0=0;
unsigned char fl1=0;
unsigned char fl2=0;

void setup() {
  pinMode(p0,OUTPUT);
  pinMode(p1,OUTPUT);
  pinMode(p2,OUTPUT);
  digitalWrite(p0,fl0);
  digitalWrite(p1,fl1);
  digitalWrite(p2,fl2);
  cur_mil0=cur_mil1=cur_mil2=millis();
}

void loop() {  
  if(millis()-cur_mil0>=dly0){
    fl0^=1;  digitalWrite(p0,fl0); 
    cur_mil0=millis();
  }
  if(millis()-cur_mil1>=dly1){
    fl1^=1;  digitalWrite(p1,fl1); 
    cur_mil1=millis();
  }
  if(millis()-cur_mil2>=dly2){
    fl2^=1;  digitalWrite(p2,fl2); 
    cur_mil2=millis();
  }
}
