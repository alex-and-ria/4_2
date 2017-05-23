#include "my_bp.h"
my_bp mbpz(7/*en*/,8/*dirp*/,
      9/*stpp*/,10/*dlyv*/,
     10/*cntrlp*/,3/*Ocd*/);
my_bp mbpy(3/*en*/,4/*dirp*/,
      5/*stpp*/,1/*dlyv*/,
     6/*cntrlp*/,2/*Ocd*/);
my_bp mbpx(22/*en*/,23/*dirp*/,
      24/*stpp*/,1/*dlyv*/,
     25/*cntrlp*/,1/*Ocd*/);
     
void setup() {
  mbpz.initb();
  mbpy.initb();
  mbpx.initb();
  //mbp.mv(100,1);
  mbpz.mv(mbpz.cntm-50,0);
  delay(100);
  mbpy.mv(mbpy.cntm-50,0);
  delay(100);
  mbpx.mv(mbpx.cntm-50,0);
  delay(100);
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
