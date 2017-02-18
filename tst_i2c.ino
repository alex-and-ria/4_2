#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#define scl 19
#define sda 38
#define sa 0x21
#define dl 5
unsigned long ulPeriod, dutyCycle;
unsigned long mcs0=0;
unsigned int state = HIGH;

void setup() {
    // 40 MHz system clock
  SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
  ulPeriod = 4;
  dutyCycle = 2;
  // Configure PB6 as T0CCP0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  GPIOPinConfigure(GPIO_PB6_T0CCP0);
  GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
  // Configure timer
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
  TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod);
  TimerMatchSet(TIMER0_BASE, TIMER_A, dutyCycle); // PWM
  TimerEnable(TIMER0_BASE, TIMER_A);
  Serial.begin(9600);
  pinMode(scl,OUTPUT);
  pinMode(sda,OUTPUT);
  digitalWrite(scl,HIGH);
  digitalWrite(sda,HIGH);
  pinMode(GREEN_LED, OUTPUT);
  mcs0=micros();
}

void isd(){
  mcs0=micros();
  while((micros()-mcs0)<dl);
}

void sst(){
  digitalWrite(sda,HIGH); isd();
  digitalWrite(scl,HIGH); isd();
  digitalWrite(sda,LOW); isd();
  digitalWrite(scl,LOW); isd();
}

void ssp(){
  digitalWrite(sda,LOW); isd();
  digitalWrite(scl,HIGH); isd();
  digitalWrite(sda,HIGH); isd();
}

void nack(){
  digitalWrite(sda,HIGH); isd();
  digitalWrite(scl,HIGH); isd();
  digitalWrite(scl,LOW); isd();
  digitalWrite(sda,LOW); isd();
}

bool sendb(byte data){
  bool sdd=false;
  for(byte i=0;i<8;i++){
    if((data<<i)&0x80)
      digitalWrite(sda,HIGH);
    else
      digitalWrite(sda,LOW);
    isd();
    digitalWrite(scl,HIGH); isd();
    digitalWrite(scl,LOW); isd();
  }
  pinMode(sda,INPUT_PULLUP); isd();
  digitalWrite(scl,HIGH); isd();
  if(digitalRead(sda)==LOW) sdd=true;
  else sdd=false;
  digitalWrite(scl,LOW); isd();
  pinMode(sda,OUTPUT);
}

byte getb(){
  byte data=0;
  pinMode(sda,INPUT_PULLUP); isd();
  for(byte i=8;i>0;i--){
    digitalWrite(scl,HIGH); isd();
    data<<=1;
    if(digitalRead(sda)==HIGH) data++;
    digitalWrite(scl,LOW); isd();
  }
  pinMode(sda,OUTPUT);
  return data;
}

bool getval(byte regadr, byte& val){
  sst();
  if(sendb(2*sa)){
    if(sendb(regadr)){
      ssp();
      sst();
      if(sendb(2*sa+1)){
        val=getb();
        nack(); ssp();
        return true;
      }
      else{
        ssp(); return false;
      }
    }
    else{
      ssp(); return false;
    }
  }
  else{
      ssp(); return false;
    }
}

void loop() {
  byte val=0;
  if(getval(0x1,val)) {
    Serial.print(val,HEX);
    if(val==0x80){
       digitalWrite(GREEN_LED, state);
       state=!state;
       for(unsigned int i=0;i<65535;i++) isd();
    }
  }
  //else Serial.println("qqq");
  //delay(1000);    
}
