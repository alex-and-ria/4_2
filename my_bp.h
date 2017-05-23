#ifndef MY_BP_H
#define MY_BP_H

#define Ox 1
#define Oy 2
#define Oz 3

class my_bp{
  public:
    my_bp(unsigned char en,unsigned char dirp,
      unsigned char stpp,unsigned char dlyv,
      unsigned char cntrlp,unsigned char Ocd);
    unsigned char enp,dir,stp,dly,cntrl;
    unsigned char O;//Ox -- 1; Oy -- 2; Oz -- 3;
    unsigned int incnt,cntzm=1500;
    void initb();
    void mv(unsigned int num,bool to);
};

#endif
