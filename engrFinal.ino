#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
//Initializing all global variables used in the game
const byte interuptRightButton = 4;
const byte interuptLeftButton = 5;
const byte interuptSwitch = 7;
bool rightButtonDown;
bool leftButtonDown;
bool switchTriggered;
int level = 0;
int levelStore = 0;
int timer = 1;
int timerStore = 0;
int switchCounter = 0;
int light = 0;
unsigned long green = 0x16de27;
AsyncDelay delay_3s;
AsyncDelay delay_5s;
AsyncDelay delay_1s;
AsyncDelay switchTime;
bool delay3reset = false;
bool delay1reset = false;
bool delay5reset = false;
bool switchTimer = false;
bool timerStart = false;
bool timestopper = false;
bool buttons = false;
bool quick = false;
bool moistb = false;
bool lightb = false;
bool tempb = false;
int timerSound;
float X, Y, Z, totalAccel;
float tempF;

void setup() {
  // setting up all timers, interrupts, and the accelerator
  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(interuptRightButton), RBP, FALLING);
  attachInterrupt(digitalPinToInterrupt(interuptLeftButton), LBP, FALLING);
  attachInterrupt(digitalPinToInterrupt(interuptSwitch), SwitchFlick, CHANGE);
  delay_3s.start(3000, AsyncDelay::MILLIS);
  delay_1s.start(1000, AsyncDelay::MILLIS);
  delay_5s.start(5000, AsyncDelay::MILLIS);
  switchTime.start(1000, AsyncDelay::MILLIS);
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
  Serial.begin(9600);

}

void loop() {
  // map to make the frequency higher as time goes on
  timerSound = map(timer, 60, 1, 200, 700);
  //switch statement used for different levels
  switch(level){
    //level 0 is the transition stage where variables are set and timers are restartes
    case 0:
      if(delay3reset == false){
        delay_3s.restart();
        delay3reset = true;
      }
      ClockwiseGreen();
      if(delay_3s.isExpired()){
        CircuitPlayground.clearPixels();
        delay5reset = false;
        delay1reset = false;
        timerStart = false;
        timestopper = false;
        switchTriggered = false;
        timerStore = 0;
        switchCounter = 0;
        levelStore += 1;
        level = levelStore;
        Serial.println("Delay 3 done");
        break;
      }
    //level 1
    case 1:
      Timer();
      //Checks to see if left then right button are down and turns lights on if they are
      if(CircuitPlayground.leftButton() == true){
        CircuitPlayground.setPixelColor(0, green);
        CircuitPlayground.setPixelColor(1, green);
        CircuitPlayground.setPixelColor(2, green);
        CircuitPlayground.setPixelColor(3, green);
        CircuitPlayground.setPixelColor(4, green);
        if(CircuitPlayground.rightButton() == true){
          CircuitPlayground.setPixelColor(5, green);
          CircuitPlayground.setPixelColor(6, green);
          CircuitPlayground.setPixelColor(7, green);
          CircuitPlayground.setPixelColor(8, green);
          CircuitPlayground.setPixelColor(9, green); 
          delay(250);
          //resets timer for transition stage and send you back to it
          delay3reset = false;
          level = 0;
        }
      }
      //turns off lights if button is no longer being pressed
      else{
        CircuitPlayground.setPixelColor(0, 0,0,0);
        CircuitPlayground.setPixelColor(1, 0,0,0);
        CircuitPlayground.setPixelColor(2, 0,0,0);
        CircuitPlayground.setPixelColor(3, 0,0,0);
        CircuitPlayground.setPixelColor(4, 0,0,0); 
      }
      break;
    //Level 2
    case 2:
      //timer is stared
      Timer();
      //detects if switch had been flicked
      if(switchTriggered == true){
        //acceleromoter is started
        Accel();
        Serial.print(switchCounter);
        CircuitPlayground.setPixelColor(0, green);
        CircuitPlayground.setPixelColor(9, green);
        //starts 5 second timer for switch
        if(timestopper == false){
          switchTime.restart();
          timestopper = true;
        }
        if(switchTime.isExpired()){
          switchCounter += 1;
          switchTime.repeat();
        }
        if(switchCounter < 5){
          //if the board is moving it will light up
          if(totalAccel > 40){
            CircuitPlayground.setPixelColor(1, green);
            CircuitPlayground.setPixelColor(2, green);
            CircuitPlayground.setPixelColor(8, green);
            CircuitPlayground.setPixelColor(7, green);
            quick = true;
          }
          //if both button are pressed it will light up
          else if(CircuitPlayground.leftButton() == true && CircuitPlayground.rightButton() == true){
            CircuitPlayground.setPixelColor(3, green);
            CircuitPlayground.setPixelColor(4, green);
            CircuitPlayground.setPixelColor(5, green);
            CircuitPlayground.setPixelColor(6, green);
            buttons = true;
          }
        }
        //if it reaches 5 seconds afer the switch is flicked then the lights turn off and booleans are set to false
        else if(switchCounter == 5){
          switchTriggered = false;
          quick = false;
          buttons = false;
          for(int i = 0; i < 10; i++){
            CircuitPlayground.setPixelColor(i, 0, 0, 0);
          }
        }
         //if all conditions are bet you go to the next level
        if(buttons == true && quick == true && switchTriggered == true){
          level = 0;
          delay3reset = false;
        }
      }
      break;
    //Level 3
    case 3:
      //starts timer and starts detecting temp and light
      Timer();
      tempF = CircuitPlayground.temperatureF();
      light = CircuitPlayground.lightSensor();

      //checks to see if the right pin is being pressed to detect moisture
      if(CircuitPlayground.readCap(1) >= 980){
        moistb = true;
        CircuitPlayground.setPixelColor(0, green);
        CircuitPlayground.setPixelColor(9, green);
      }
      else if(CircuitPlayground.readCap(1) < 980){
        moistb = false;
        CircuitPlayground.setPixelColor(0, 0,0,0);
        CircuitPlayground.setPixelColor(9, 0,0,0);
      }
      //Checks to see if the temp is hot enough
      if(tempF >= 88){
        tempb = true;
        CircuitPlayground.setPixelColor(1, green);
        CircuitPlayground.setPixelColor(2, green);
        CircuitPlayground.setPixelColor(8, green);
        CircuitPlayground.setPixelColor(7, green);
      }
      else if(tempF < 88){
        tempb = false;
        CircuitPlayground.setPixelColor(1, 0,0,0);
        CircuitPlayground.setPixelColor(2, 0,0,0);
        CircuitPlayground.setPixelColor(8, 0,0,0);
        CircuitPlayground.setPixelColor(7, 0,0,0);
      }
      //checks to see if there is enough light
      if(light >= 950){
        lightb = true;
        CircuitPlayground.setPixelColor(3, green);
        CircuitPlayground.setPixelColor(4, green);
        CircuitPlayground.setPixelColor(5, green);
        CircuitPlayground.setPixelColor(6, green);
      }
      else if(light < 950){
        lightb = false;
        CircuitPlayground.setPixelColor(3, 0,0,0);
        CircuitPlayground.setPixelColor(4, 0,0,0);
        CircuitPlayground.setPixelColor(5, 0,0,0);
        CircuitPlayground.setPixelColor(6, 0,0,0);
      }
      //if all conditions are met then you wil the level
      if(lightb == true && tempb == true && moistb == true){
        level = 0;
        delay3reset = false;
      }
      break;
    //Win stage
    case 4:
      RandomLights();
      break;
    //Lose stage, sets variables back and sends you back to the level you were on after holding either button
    case 9:
      if(CircuitPlayground.leftButton() == true || CircuitPlayground.rightButton() == true){
        delay5reset = false;
        delay1reset = false;
        timerStart = false;
        timestopper = false;
        switchTriggered = false;
        timerStore = 0;
        switchCounter = 0;
        level = levelStore;
      }
      Red();
      break;
  }
}
//right button interrupt
void RBP(){
  rightButtonDown = !rightButtonDown;
}
//left button interrupt
void LBP(){
  leftButtonDown = !leftButtonDown;
}
//switch interrupt
void SwitchFlick(){
  switchTriggered = true;
  switchCounter = 0;
  timestopper = false;
}
//acceletomoter code to detect motion
void Accel(){
  X = 0;
  Y = 0;
  Z = 0;
  for (int i=0; i<10; i++) {
    X += CircuitPlayground.motionX();
    Y += CircuitPlayground.motionY();
    Z += CircuitPlayground.motionZ();
    delay(1);
  }
  X /= 10;
  Y /= 10;
  Z /= 10;

  totalAccel = sqrt(X*X + Y*Y + Z*Z);
  Serial.println(totalAccel);
}
//light function to make green lights go off
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
//light function to make red lights go off
void Red(){
  CircuitPlayground.setPixelColor(9, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(8, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(7, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(6, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(5, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(4, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(3, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(2, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(1, 255,0,0);
  delay(25);
  CircuitPlayground.setPixelColor(0, 255,0,0);
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
//timer to be reset every level and count down from 60
void Timer(){
  if(timer == 0){
    level = 9;
  }
  if(delay1reset == false){
    delay_1s.restart();
    delay1reset = true;
  }
  if(delay5reset == false){
    delay_5s.restart();
    delay5reset = true;
  }
  if(timerStart == false){
    timer = 60;
    timerStart = true;
  }
  if(delay_1s.isExpired() && timerStore >= 11){
    timer -= 1;
    CircuitPlayground.playTone(timerSound, 50);
    Serial.println(timer);
    delay_1s.repeat();
  }
  else if(delay_1s.isExpired()){
    timer -= 1;
    Serial.println(timer);
    delay_1s.repeat();
  }
  if(delay_5s.isExpired() && timerStore < 11){
    timerStore += 1;
    Serial.println("Delay 5 done");
    CircuitPlayground.playTone(timerSound, 50);
    delay_5s.repeat();
  }
}
//ranibow lights for when the game is won
void RandomLights(){
  CircuitPlayground.setPixelColor(random(0,9), random(0,255), random(0,255), random(0,255));
  delay(random(5,50));
}

