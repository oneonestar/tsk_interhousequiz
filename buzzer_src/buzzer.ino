#include <Wire.h>
#define SLAVE_ADDRESS 0x04

const int one =1;
int counter[6]={0, 0, 0, 0, 0, 0};
int signal = 0;//it determines which button has been pressed

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
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  //start i2c connection
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}// configuration anf the initialzationof the io devices

void loop()
{
  int temp;//use for storing a temp integer for the other usage
  signal = 0;//it determines which button has been pressed
  
  for(int index=2; index<8; index++)
  {
    //read from button
    int buttonvoltage = digitalRead(index);
  
    if (buttonvoltage == HIGH)
    {
      //digitalWrite(13, LOW);
    }
    else
    {
      //digitalWrite(13, HIGH);
      temp = one << (index-2);  //rotate the bits
      signal += temp;  //set respective bit to 1
      
     // if(index==2)
      counter[index-2]=1000;// counter fot that partiucular button
    }
  }
  Serial.println(signal);
  
  //if anyone presses the button, the signal should not be zero, so the led light will turn on when it happens
  /*if(signal != 0)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW);
  }
  */
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
}

void receiveData(int byteCount) {
  
}

void sendData() {
  
  Wire.write(signal);
}
