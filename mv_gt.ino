#include "my_stepper.h"
/*#define in1 8
#define in2 9
#define in3 10
#define in4 11

int n = 4;
int k = 5;
int steps =4;
int dly = 25;*/

/*bool arr[]={0,1,1,0, 
            0,1,0,1, 
            1,0,0,1, 
            1,0,1,0};*/
/*bool arr[]={1,0,0,0, 
            0,1,0,0, 
            0,0,1,0, 
            0,0,0,1};*/
mStepper msr(8,9,10,11);

void setup() {
  
}

void loop() {
  msr.g2right(15);
  msr.g2left(15);//right
  //g2left();
}

/*void g2right(){
  uint8_t j,i;
  for (j=0; j<k; j++)
  for(i=0;i<steps;i++){
    digitalWrite(in1,arr[i*n+0]);
    digitalWrite(in2,arr[i*n+1]);
    digitalWrite(in3,arr[i*n+2]);
    digitalWrite(in4,arr[i*n+3]);
    delay(dly);
  }
}

void g2left(){
  int8_t j,i;
  for (j=0; j<k; j++)
  for(i=steps-1;i>=0;i--){
    digitalWrite(in1,arr[i*n+0]);
    digitalWrite(in2,arr[i*n+1]);
    digitalWrite(in3,arr[i*n+2]);
    digitalWrite(in4,arr[i*n+3]);
    delay(dly);
  }
}*/
