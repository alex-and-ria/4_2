#include "my_bp.h"
my_bp mbp(7/*en*/,8/*dirp*/,
      9/*stpp*/,10/*dlyv*/,
     10/*cntrlp*/,3/*Ocd*/);
void setup() {
  mbp.initb();

}

void loop() {
  mbp.mv(100,0);

}
