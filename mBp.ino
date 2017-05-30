#include "my_bp.h"
#include "my_rot.h"

#define pumpp 30

my_bp mbpz(7/*en*/,8/*dirp*/,
      9/*stpp*/,10/*dlyv*/,
     10/*cntrlp*/,3/*Ocd*/);
my_bp mbpy(3/*en*/,4/*dirp*/,
      5/*stpp*/,1/*dlyv*/,
     6/*cntrlp*/,2/*Ocd*/);
my_bp mbpx(22/*en*/,23/*dirp*/,
      24/*stpp*/,1/*dlyv*/,
     25/*cntrlp*/,1/*Ocd*/);

my_rot mrot(38,39,40,40,50);

void setup() {
  pinMode(pumpp,OUTPUT);
  mbpz.initb();
  mbpy.initb();
  mbpx.initb();
  //mbp.mv(100,1);
  //mbpz.mv(mbpz.cntm-50,0);
  mbpz.mv(50,0);
  delay(100);
  //mbpy.mv(mbpy.cntm-50,0);
  mbpy.mv(50,0);
  delay(100);
  //mbpx.mv(mbpx.cntm-50,0);
  mbpx.mv(50,0);
  delay(100);
  digitalWrite(pumpp,HIGH);
  
  /*delay(1000);
  mbpz.mv(200,0);*/
  delay(100);
  digitalWrite(pumpp,LOW);
  delay(1000);
  mrot.mv(10,1);
}

void loop() {
  //mbp.incntz=1400;
/*  mbpz.mv(mbpz.cntm-50,0);
  delay(100);
  mbpy.mv(mbpy.cntm-50,0);
  delay(100);
  mbpx.mv(mbpx.cntm-50,0);
  delay(100);*/
  
}
