#include "my_rot.h"
#include <Arduino.h>

my_rot::my_rot(unsigned char _in1,unsigned char _in2,
      unsigned char _in3, unsigned char _in4,unsigned int _dly):
      in1(_in1), in2(_in2), in3(_in3), in4(_in4), dly(_dly){
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
}

void my_rot::mv(unsigned int num,bool to){
  for (unsigned int j=0; j<num; j++){
    for(unsigned int i=0;i<steps;i++){
      digitalWrite(in1,mass[i*n+0]);
      digitalWrite(in2,mass[i*n+1]);
      digitalWrite(in3,mass[i*n+2]);
      digitalWrite(in4,mass[i*n+3]);
      delay(dly);
    }
  }
}

