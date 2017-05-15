#ifndef MYSTEPPER_H
#define MYSTEPPER_H
class mStepper{
  public:
    unsigned char steps=4;
    unsigned char n = 4;
    bool arr[4*4]={1,0,0,0, 
              0,1,0,0, 
              0,0,1,0,
              0,0,0,1
              };
    unsigned int dly = 25;// ranges: [0:255];
    mStepper(unsigned char in1,unsigned char in2,unsigned char in3,unsigned char in4);
    void g2right(unsigned char nummv);
    void g2left(unsigned char nummv);
  //private:
    unsigned char _in[4];
    unsigned char _ncv;
};

#endif

