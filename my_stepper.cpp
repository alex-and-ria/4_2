#include "my_stepper.h"
#include <Arduino.h>
mStepper::mStepper(unsigned char in1,unsigned char in2,unsigned char in3,unsigned char in4){
  _in[0]=in1; _in[1]=in2; _in[2]=in3; _in[3]=in4;
  for(unsigned char i=0;i<4;i++)
    pinMode(_in[i], OUTPUT);

  for(unsigned char i=0;i<4;i++)
    digitalWrite(_in[i], 0);
  _ncv=0;
}

void mStepper::g2right(unsigned char nummv){
  unsigned char j,i,k;
  /*for(k=0;k<4;k++)
        digitalWrite(_in[k],0);
         delay(dly);*/
  
  for (j=0; j<nummv; j++){
    for(i=0;i<steps;i++){
      for(k=0;k<4;k++){
        digitalWrite(_in[k],arr[i*n+k]);
      }
      delay(dly);
    }
    //_ncv++; _ncv%=4;
  }
}

void mStepper::g2left(unsigned char nummv){
  unsigned char j,k;
  int i=0;
  for (j=0; j<nummv; j++){
    for(i=steps-1;i>=0;i--){
      for(k=0;k<4;k++){
        digitalWrite(_in[k],arr[i*n+k]);
        //_ncv++; _ncv%=4;
      }
      delay(dly);
    }
  }
}

