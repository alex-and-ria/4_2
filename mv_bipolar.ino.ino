#define in1 8
#define in2 9
#define in3 10
#define in4 11

int n = 4;
int k = 5;
int steps =4;
int dly = 1000;

bool arr[]={0,1,1,0, 
            0,1,0,1, 
            1,0,0,1, 
            1,0,1,0};

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
int i, j;
  for (j=0; j<k; j++) {
  i=0;while(i<steps){
      digitalWrite(in1,arr[i*n+0]);
      digitalWrite(in2,arr[i*n+1]);
      digitalWrite(in3,arr[i*n+2]);
      digitalWrite(in4,arr[i*n+3]);
      delay(dly);
      i++;
    }
  }
  for (j=0; j<k; j++) {
  i=steps-1;while(i>=0){
      digitalWrite(in1,arr[i*n+0]);
      digitalWrite(in2,arr[i*n+1]);
      digitalWrite(in3,arr[i*n+2]);
      digitalWrite(in4,arr[i*n+3]);
      delay(dly);
      i--;
    }
  }
}
