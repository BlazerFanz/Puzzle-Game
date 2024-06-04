#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

const byte interuptRightButton = 4;
const byte interuptLeftButton = 5;
bool rightButtonDown;
bool leftButtonDown;
int level = 0;
int levelStore = 0;
int timer;
int timerStore = 0;
unsigned long green = 0x16de27;
AsyncDelay delay_3s;
AsyncDelay delay_5s;
AsyncDelay delay_1s;
bool delay3reset = false;
bool delay1reset = false;
bool delay5reset = false;




void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(interuptRightButton), RBP, FALLING);
  attachInterrupt(digitalPinToInterrupt(interuptLeftButton), LBP, FALLING);
  delay_3s.start(3000, AsyncDelay::MILLIS);
  delay_1s.start(1000, AsyncDelay::MILLIS);
  delay_5s.start(5000, AsyncDelay::MILLIS);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //int speaker = analogRead(A0);
  int timerSound = map(timer, 1, 60, 700, 200);
  switch(level){
    case 0:
      if(delay3reset == false){
        delay_3s.restart();
        delay3reset = true;
      }
      ClockwiseGreen();
      if(delay_3s.isExpired()){
        CircuitPlayground.clearPixels();
        levelStore += 1;
        level = levelStore;
        Serial.println("Delay 3 done");
      }
    case 1:
      if(delay1reset == false){
        delay_1s.restart();
        delay1reset = true;
      }
      if(delay5reset == false){
        delay_5s.restart();
        delay5reset = true;
      }
      timer = 60;
      if(delay_1s.isExpired() && timerStore >= 11){
        timer -= 1;
        CircuitPlayground.playTone(timerSound, 50);
        Serial.println("Delay 1 done");
        delay_1s.repeat();
      }
      else if(delay_1s.isExpired()){
        timer -= 1;
        Serial.println("Delay 1 done");
        delay_1s.repeat();
      }
      if(delay_5s.isExpired() && timerStore < 11){
        timerStore += 1;
        Serial.println("Delay 5 done");
        CircuitPlayground.playTone(timerSound, 50);
        delay_5s.repeat();
      }
      if(CircuitPlayground.leftButton() == true){
        CircuitPlayground.setPixelColor(0, green);
        CircuitPlayground.setPixelColor(1, green);
        CircuitPlayground.setPixelColor(2, green);
        CircuitPlayground.setPixelColor(3, green);
        CircuitPlayground.setPixelColor(4, green);
      }
      else{
        CircuitPlayground.setPixelColor(0, 0,0,0);
        CircuitPlayground.setPixelColor(1, 0,0,0);
        CircuitPlayground.setPixelColor(2, 0,0,0);
        CircuitPlayground.setPixelColor(3, 0,0,0);
        CircuitPlayground.setPixelColor(4, 0,0,0); 
      }
      if(CircuitPlayground.rightButton() == true && CircuitPlayground.leftButton() == true){
        CircuitPlayground.setPixelColor(5, green);
        CircuitPlayground.setPixelColor(6, green);
        CircuitPlayground.setPixelColor(7, green);
        CircuitPlayground.setPixelColor(8, green);
        CircuitPlayground.setPixelColor(9, green); 
        delay(250);
        delay3reset = false;
        level = 0;
      }
    case 2:
      break;
  }
}

void RBP(){
  rightButtonDown = !rightButtonDown;
}

void LBP(){
  leftButtonDown = !leftButtonDown;
}

void ClockwiseGreen(){
  CircuitPlayground.setPixelColor(9, green);
  delay(25);
  CircuitPlayground.setPixelColor(8, green);
  delay(25);
  CircuitPlayground.setPixelColor(7, green);
  delay(25);
  CircuitPlayground.setPixelColor(6, green);
  delay(25);
  CircuitPlayground.setPixelColor(5, green);
  delay(25);
  CircuitPlayground.setPixelColor(4, green);
  delay(25);
  CircuitPlayground.setPixelColor(3, green);
  delay(25);
  CircuitPlayground.setPixelColor(2, green);
  delay(25);
  CircuitPlayground.setPixelColor(1, green);
  delay(25);
  CircuitPlayground.setPixelColor(0, green);
  delay(25);

  CircuitPlayground.setPixelColor(9, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(8, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(7, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(6, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(5, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(4, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(3, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(2, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(1, 0x000000);
  delay(25);
  CircuitPlayground.setPixelColor(0, 0x000000);
  delay(25);  
}
