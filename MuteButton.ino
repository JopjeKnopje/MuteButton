


#include <Keyboard.h>

#define PIN_BTN 7
#define PIN_INTERRUPT 2 // if this pin is pulled low the main loop will stop

#define DEBUG false // print debug messages
#define longPress 500
#define shortPress 5

// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 30;    // the debounce time; increase if the output flickers

bool interrupted = false;

bool done = false;
int startTimer;
int endTimer;

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);
  // set the interrupt pin as input
  pinMode(PIN_INTERRUPT, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), interrupt, LOW);

  if(DEBUG) Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  if (!interrupted) {
    debounce();
  }
}

void keyStroke(char c) {
  Keyboard.press(c);
  delay(100);
  Keyboard.releaseAll();
}

// when the interuppt happens
void interrupt() {
  interrupted = true;
}


void debounce() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(PIN_BTN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
    
      // set timer to currenttime
      done = false;
      startTimer = millis();

      while(digitalRead(PIN_BTN)) {

        endTimer = (millis() - startTimer);

        if (endTimer > longPress) {
          if(DEBUG) Serial.println("long");
          keyStroke(KEY_F14);
          done = true;
          // exit loop when long 
          break;
        }       
      }
      if (endTimer > shortPress && endTimer < longPress && !done) {
        if (DEBUG) Serial.println("short");
          keyStroke(KEY_F13);
          endTimer = 0;
      }
    }
  }
  lastButtonState = reading;
}
