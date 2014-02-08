#include <Wire.h>
#define SLAVE_ADDRESS 0x04

const int one =1;
int counter[6]={0, 0, 0, 0, 0, 0};
int signal = 0;//it determines which button has been pressed
int buzzerState = 0;
unsigned long timer[6];

void setup()
{
  //start serial connection
  Serial.begin(9600);
  
  //setup the input pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  
  //setup the output pins
  pinMode(8, OUTPUT);
  pinMode(8, LOW);
  pinMode(9, OUTPUT);
  pinMode(9, LOW);
  pinMode(10, OUTPUT);
  pinMode(10, LOW);
  pinMode(11, OUTPUT);
  pinMode(11, LOW);
  pinMode(12, OUTPUT);
  pinMode(12, LOW);
  pinMode(13, OUTPUT);
  pinMode(13, LOW);
  
  //start i2c connection
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}// configuration anf the initialzationof the io devices

void loop()
{
  while(buzzerState==1)
  {
    //int temp;//use for storing a temp integer for the other usage
    //signal = 0;//it determines which button has been pressed
    
    for(int index=2; index<8; index++)
    {
      if(counter[index-2]==1) {
        continue;
      }
      else {
        //read from button
        int buttonvoltage = digitalRead(index);
        timer[index-2] = millis();
        if (buttonvoltage == LOW)
        {
          //digitalWrite(13, HIGH);
          signal += (one << (index-2));  //rotate the bits
          //signal += temp;  //set respective bit to 1
          
         // if(index==2)
          counter[index-2]=1;// counter fot that partiucular button
          pinMode(index+6, HIGH);
          //Serial.println(counter[index-2]);
        }
      }
    }
    //Serial.println(signal);
/*    
    //try to turn on the led for few seconds after pressing the button
    for(int index = 0; index<6; index++)
    {
      if(counter[index] !=0)
      {
        digitalWrite(index+8, LOW);
        counter[index] --;
      }
      else
      {
        digitalWrite(index+8, HIGH);
      }
    }
*/
  }
}

void reset()
{
  signal = 0;
  /*
  for(int i=0; i<6; i++) {
    Serial.println(timer[i]);
  }
  */
  for(int i=0; i<6; i++) {
    counter[i] = 0;
    timer[i] = 0;
    pinMode(i+8, LOW);
  }
}

void receiveData(int byteCount) {
  while(Wire.available()) {
    int reader = Wire.read();
    
    if(reader==0) {
      buzzerState = reader;
      reset();
    }
    else {
      reset();
      buzzerState = reader;
    }
  
    Serial.print("data received: ");
    Serial.println(buzzerState);
    //Serial.print("data size: ");
    //Serial.println(byteCount);
  }
}

void sendData() {
  
  Wire.write(signal);
}
