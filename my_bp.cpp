#include "my_bp.h"
#include <Arduino.h>
my_bp::my_bp(unsigned char en,unsigned char dirp,
      unsigned char stpp,unsigned char dlyv,
      unsigned char cntrlp,unsigned char Ocd){
  enp=en;dir=dirp;stp=stpp;dly=dlyv;cntrl=cntrlp;O=Ocd;
  pinMode(dir,OUTPUT);pinMode(stp,OUTPUT);
  pinMode(cntrl,INPUT);pinMode(enp,INPUT);
}

void my_bp::initb(){
  if(O==Oz){
    pinMode(enp,OUTPUT); digitalWrite(enp,LOW);//enable move;
    if(digitalRead(cntrl)!=LOW){//to the cntrl;
      digitalWrite(dir,LOW);//for Oz;
    }
    bool cfl=1,cfl1=0; unsigned long cmll=millis();
    while(digitalRead(cntrl)!=LOW){//move;
      //for(unsigned int i=0;i<num;i++){
      /*if(digitalRead(cntrl)==LOW){//fin;
        pinMode(enp,INPUT);//off move;
        incnt=0; break;
      }*/
      if(cfl==1){
        digitalWrite(stp, HIGH); cfl=0;
        cmll=millis();
      }
      if(millis()-cmll>=dly){//delay(dly);
        cfl1=1; cmll=millis();
      }
      if(cfl1==1){
        digitalWrite(stp, LOW); cfl1=0;
        cmll=millis();
      }
      if(millis()-cmll>=dly){//delay(dly);
        cfl=1; cmll=millis();
      }
    }
    incnt=0; pinMode(enp,INPUT);//off move;
  }
}

void my_bp::mv(unsigned int num,bool to){
  if(O==Oz){
    pinMode(enp,OUTPUT); digitalWrite(enp,LOW);//enable move;
    if(to==1&&incnt>0){//to the cntrl;
      digitalWrite(dir,LOW);//for Oz;
    }
    else if(to==0&&incnt<cntzm){//from cntrl;
      digitalWrite(dir,HIGH);//for Oz;
    }
    unsigned int i=0;bool cfl=1,cfl1=0; unsigned long cmll=millis();
    while(i<num){//move;
    //for(unsigned int i=0;i<num;i++){
      if(digitalRead(cntrl)==LOW&&to==1){//fin;
        pinMode(enp,INPUT);//off move;
        i=num; incnt=0; break;
      }
      if(to==0&&incnt>=cntzm-50){
        pinMode(enp,INPUT);//off move;
        i=num; break;
      }
      if(cfl==1){
        digitalWrite(stp, HIGH); cfl=0;
        cmll=millis();
      }
      if(millis()-cmll>=dly){//delay(dly);
        cfl1=1; cmll=millis();
      }
      if(cfl1==1){
        digitalWrite(stp, LOW); cfl1=0; i++;
        if(to==1) incnt--;
        else incnt++;
        cmll=millis();
      }
      if(millis()-cmll>=dly){//delay(dly);
        cfl=1; cmll=millis();
      }
    }
    pinMode(enp,INPUT);//off move;
  }
  //else
}

