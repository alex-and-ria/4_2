#ifndef MY_ROT_H
#define MY_ROT_H

class my_rot{
  public:
    my_rot(unsigned char _in1,unsigned char _in2,
      unsigned char _in3, unsigned char _in4,unsigned int _dly);
    unsigned char in1,in2,in3,in4,curpos;
    unsigned int n = 4, steps = 8,dly;
    /*unsigned int mass[4*8]={
            1,0,0,0, 
            0,1,0,0, 
            0,0,1,0, 
            0,0,0,1, 
            1,0,0,0, 
            0,1,0,0, 
            0,0,1,0, 
            0,0,0,1};*/
    unsigned int mass[4*8]={
            1,0,0,0, 
            1,1,0,0, 
            0,1,0,0, 
            0,1,1,0, 
            0,0,1,0, 
            0,0,1,1, 
            0,0,0,1, 
            1,0,0,1};
    void mv(unsigned int num,bool to);
};

#endif
