int p1=16;
int p2=14;
int p3=12;
int p4=15;
unsigned int pstp=0;
#define dir 10
#define stp 11
#define led 13

void setup() {
  // put your setup code here, to run once:
 //Serial.begin(9600);
 pinMode(p1, OUTPUT);
 pinMode(p2, OUTPUT);
 pinMode(p3, OUTPUT);
 pinMode(p4, OUTPUT);
 pinMode(dir, INPUT);
 pinMode(stp, INPUT);
 pinMode(led, OUTPUT);
}

int n = 4;
int steps = 8;
int k = 50;
//int dly = 250;

// int mass[]={1,0,0,0, 1,1,0,0, 0,1,0,0, 0,1,1,0, 0,0,1,0, 0,0,1,1, 0,0,0,1, 1,0,0,1};
int mass[]={1,0,0,0, 
            1,1,0,0, 
            0,1,0,0, 
            0,1,1,0, 
            0,0,1,0, 
            0,0,1,1, 
            0,0,0,1, 
            1,0,0,1};
/*int mass[]={1,0,0,0, 
            0,1,0,0, 
            0,0,1,0, 
            0,0,0,1, 
            1,0,0,0, 
            0,1,0,0, 
            0,0,1,0, 
            0,0,0,1};*/

void loop() 
{
  unsigned int edge=digitalRead(stp);
  /*Serial.print("edge=");Serial.println(edge);
  Serial.print("pstp=");Serial.println(pstp);*/
  if(edge!=pstp){
    pstp=edge;
    if(edge==HIGH){//rising
      digitalWrite(led,HIGH);
    }
    else{
      digitalWrite(led,LOW);
    }
  }
  int i, j;
  for (j=0; j<k; j++) {
  i=0;while(i<steps)
  {
    digitalWrite(p1,mass[i*n+0]);
    digitalWrite(p2,mass[i*n+1]);
    digitalWrite(p3,mass[i*n+2]);
    digitalWrite(p4,mass[i*n+3]);
    //delay(dly);
    i++;
  }
  }
  for (j=0; j<k; j++) {
  i=7;while(i>=0)
  {
    digitalWrite(p1,mass[i*n+0]);
    digitalWrite(p2,mass[i*n+1]);
    digitalWrite(p3,mass[i*n+2]);
    digitalWrite(p4,mass[i*n+3]);
    //delay(dly);
    i--;
  }
  }
}
