#include <Wire.h>
#define SLAVE_ADDRESS 0x04

//pin 2-7: Button
//2=A, 3=D ...
//pin 8-13: LED
//8=A, 9=D ...


//time to reset after first pressed (ms)
const int TIMEOUT = 5*1000;

//record each house is pressed or not
int counter[6]={0, 0, 0, 0, 0, 0};

//digit's position:
//signal[0] = First Pressed
//signal[1] = Second Pressed
//signal[2] = Second Pressed
//...
//signal[5] = Fifth Pressed
//===========================
//digit's value
//signal[n] = [1-6]
//1=A, 2=D, 3=H, 4=J, 5=L, 6=M
//===========================
//eg.
//000001 = [A]
//000645 = [L,J,M], L pressed first
char signal[7];

//number of house pressed
int pressed = 0;

//buzzerState = 0 => Disable
//buzzerState = 1 => Enable
int buzzerState = 1;

//record the first pressed time
unsigned long timer = 0;

int index = 0;


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
  
  pinMode(8, LOW);
  pinMode(9, LOW);
  pinMode(10, LOW);
  pinMode(11, LOW);
  pinMode(12, LOW);
  pinMode(13, LOW);

  // = End
  signal[6] = 65;
  
  //start i2c connection
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}// configuration anf the initialzationof the io devices

void loop()
{
  while(buzzerState==1)
  {
    for(int index=2; index<8; index++)  //2-7
    {
      if(counter[index-2]==1) {
        continue;
      }
      else {
        //read from button
        int buttonvoltage = digitalRead(index);
        if (buttonvoltage == LOW)
        { //First pressed for this house
          if(pressed == 0)
          { //First pressed for all house
            timer = millis();
          }
          signal[pressed] = (index-1)+48;
          counter[index-2]=1;
          pinMode(index+6, HIGH);
          pressed++;
        }
      }
    }
    if(timer>0)
    {
      if(millis() - timer > TIMEOUT)
      {
        reset();
      }
    }
  }
}

void reset()
{
  timer = 0;
  pressed = 0;
  for(int i=0; i<6; i++) {
    signal[i] = 0;
    counter[i] = 0;
    pinMode(i+8, LOW);
  }
}

void receiveData(int byteCount) {
  while(Wire.available()) {
    int reader = Wire.read();
    
    if(reader==0) //reset()
    {
      reset();
    }
    if(reader==1) //enable()
    {
      buzzerState = 1;
    }
    if(reader==2) //disable()
    {
      buzzerState = 0;
    }

    Serial.print("data received: ");
    Serial.println(buzzerState);
    //Serial.print("data size: ");
    //Serial.println(byteCount);
  }
}

void sendData() {
  Wire.write(signal[index]);
  index++;
  if(index > 6) {
    index = 0;
  }
}
