#include "my_rot.h"
#include <Arduino.h>

my_rot::my_rot(unsigned char _in1,unsigned char _in2,
      unsigned char _in3, unsigned char _in4,unsigned int _dly):
      in1(_in1), in2(_in2), in3(_in3), in4(_in4), dly(_dly){
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  curpos=255;
}

void my_rot::mv(unsigned int num,bool to){
  for (unsigned int j=0; j<num; j++){
    if(to==0){//counterclockwise;
      curpos++; curpos= (curpos==steps)? 0:curpos;
      //for(unsigned char i=0;i<steps;i++){
        digitalWrite(in1,mass[/*i*/curpos*n+0]);
        digitalWrite(in2,mass[/*i*/curpos*n+1]);
        digitalWrite(in3,mass[/*i*/curpos*n+2]);
        digitalWrite(in4,mass[/*i*/curpos*n+3]);
        delay(dly);
      //}
    }
    else{//clockwise;
      curpos--; curpos= ((255-curpos)<2)? steps-1:curpos;
      //for(unsigned char i=steps-1;i<255;i--){
        digitalWrite(in1,mass[/*i*/curpos*n+0]);
        digitalWrite(in2,mass[/*i*/curpos*n+1]);
        digitalWrite(in3,mass[/*i*/curpos*n+2]);
        digitalWrite(in4,mass[/*i*/curpos*n+3]);
        delay(dly);
      //}
    }
  }
}

