#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>
#include <Joystick.h>
#define BOUNCE_INTERVAL 5

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  18, 0,                  // Button Count, Hat Switch Count
  false, false, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

#define BUTTON_NUMBER 18
// Instantiate a Bounce object
Bounce debouncer1 = Bounce(); 
Bounce debouncer2 = Bounce(); 
Bounce debouncer3 = Bounce(); 
Bounce debouncer4 = Bounce(); 
Bounce debouncer5 = Bounce(); 
Bounce debouncer6 = Bounce(); 
Bounce debouncer7 = Bounce(); 
Bounce debouncer8 = Bounce(); 
Bounce debouncer9 = Bounce(); 
Bounce debouncer10 = Bounce(); 
Bounce debouncer11 = Bounce(); 
Bounce debouncer12 = Bounce(); 
Bounce debouncer13 = Bounce(); 
Bounce debouncer14 = Bounce(); 
Bounce debouncer15 = Bounce(); 
Bounce debouncer16 = Bounce(); 
Bounce debouncer17 = Bounce(); 
Bounce debouncer18 = Bounce(); 
Bounce buttonBounce[] = {debouncer1, debouncer2, debouncer3, debouncer4, debouncer5, debouncer6, debouncer7, debouncer8, debouncer9, debouncer10, debouncer11, debouncer12, debouncer13, debouncer14, debouncer15, debouncer16, debouncer17, debouncer18};
int buttonPin[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,A5,A4,A3,A2};
bool buttonState[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
bool previousState[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

void setup() {

 for (int i = 0; i < BUTTON_NUMBER; i++){
   pinMode(buttonPin[i],INPUT_PULLUP);
   buttonBounce[i].attach(buttonPin[i]);
   buttonBounce[i].interval(BOUNCE_INTERVAL);
  }
  Joystick.begin();

}

void loop() {

  for (int i = 0; i < BUTTON_NUMBER; i++){
   buttonBounce[i].update();
   buttonState[i] = (buttonBounce[i].read() == LOW);
   
   if (buttonState[i] && !previousState[i]) { 
    Joystick.setButton(i, buttonState[i]);
   } else if (!buttonState[i] && previousState[i]) {
    Joystick.setButton(i, buttonState[i]);
   }
   previousState[i] = buttonState[i];
  }
  
}


