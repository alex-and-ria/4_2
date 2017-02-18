#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#define sioc 19
#define siod 38
#define vsync 2
#define href 3
#define pclk 4
#define d7 5
#define d6 25
#define d5 6
#define d4 26
#define d3 7
#define d2 27
#define d1 8
#define d0 28
unsigned long ulPeriod, dutyCycle;
volatile bool isvsh=false;
volatile bool ishrr=false;
volatile bool ishrf=false;
volatile unsigned int lcnt=0,rcnt=0,lccnt=0;
unsigned long mcs0=0;

void vsr(){
  if(digitalRead(vsync)==HIGH){
    Serial.println(rcnt);
  }
  else{
    rcnt=0;
  }
  //isvsh=true;Serial.println(digitalRead(vsync));
}

void vsf(){isvsh=false; /*Serial.println(rcnt);*/ rcnt=0;}

void hrr(){
  if(digitalRead(href)==HIGH){
    rcnt++; lcnt=0;
  }
  else{
    lccnt=lcnt;
    Serial.println(lccnt);
  }
  //ishrr=true; ishrf=false; rcnt++; lcnt=0;
}

void hrf(){ishrr=false; ishrf=true;}

void pcr(){
  lcnt++;
  //if(lcnt==1285) digitalWrite(GREEN_LED,HIGH);
}

void isd(){
  mcs0=micros();
  while((micros()-mcs0)<10);
}

void setup() {
  Serial.begin(230400);
  pinMode(sioc,OUTPUT);
  pinMode(siod,OUTPUT);
  digitalWrite(sioc,HIGH);
  digitalWrite(siod,HIGH);
  pinMode(vsync,INPUT);
  pinMode(href,INPUT);
  pinMode(pclk,INPUT);
  pinMode(d7,INPUT);
  pinMode(d6,INPUT);
  pinMode(d5,INPUT);
  pinMode(d4,INPUT);
  pinMode(d3,INPUT);
  pinMode(d2,INPUT);
  pinMode(d1,INPUT);
  pinMode(d0,INPUT);
  pinMode(GREEN_LED, OUTPUT);
  // 40 MHz system clock
  SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
  ulPeriod = 128;
  dutyCycle =64;
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
  attachInterrupt(vsync,vsr,CHANGE);
  //attachInterrupt(vsync,vsf,FALLING);
  attachInterrupt(href,hrr,CHANGE);
  //attachInterrupt(href,hrf,FALLING);
  attachInterrupt(pclk,pcr,RISING);
}

void loop() {
  /*Serial.print("rcnt="); Serial.println(rcnt);
  Serial.print("lcnt=");  Serial.println(lcnt);
  Serial.print("isvsh=");  Serial.println(isvsh);
  Serial.print("isvsh=");  Serial.println(isvsh);
  Serial.print("ishrr=");  Serial.println(ishrr);
  Serial.print("ishrf=");  Serial.println(ishrf); Serial.println();*/
  //Serial.println(lcnt);
  //Serial.println(isvsh);
  //Serial.println(isvsh);
  //Serial.println(ishrr);
  //Serial.println(ishrf);
}
