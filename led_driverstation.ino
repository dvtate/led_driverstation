#include <Arduino.h>
#include <inttypes.h>
#include <FastLED.h>

// pushbutton to swich modes
#define BUTTON_PIN 2

// pot for brightness setting
#define POT_DIMMER_PIN A5

// 2M of a 144LED/M led strip
#define L_LED_DATA_PIN 5
#define L_LED_CLOCK_PIN 4
#define R_LED_DATA_PIN 7
#define R_LED_CLOCK_PIN 6

#define NUM_LEDS 144 // per side of the setup
CRGB l_leds[NUM_LEDS];
CRGB r_leds[NUM_LEDS];

// uncomment to enable debug mode (useful info sent over serial)
//#define DEBUG_MODE


// light and color modes and switching mechanisms
#include "modes.h"

// isr to check if mode change button was pressed
// ret - true if mode changed, else false
bool chkin(){
  // don't want repeated triggering
  static bool changeable = true;

  #ifdef DEBUG_MODE
    //Serial.print("button:");
    //Serial.println(digitalRead(BUTTON_PIN));
  #endif
  
  if (changeable && digitalRead(BUTTON_PIN)) {
    // if triggered, change modes and disable trigger
    changeable = false;
    mode::next();
    return true;
  
  } else if (!changeable && !digitalRead(BUTTON_PIN)) {
    // if trigger disabled and they arent pressing btn, re-enable
    changeable = true;
    
  }
  
  return false;  
  
}



// run once at startup
void setup(){
  
  #ifdef DEBUG_MODE
    Serial.begin(9600);
  #endif
  

  
  // pushbutton to switch modes
  pinMode(BUTTON_PIN, INPUT);

  pinMode(POT_DIMMER_PIN, INPUT);

  // link the colors to the LEDs
  FastLED.addLeds<APA102, L_LED_DATA_PIN, L_LED_CLOCK_PIN, BGR>(l_leds, NUM_LEDS);
  FastLED.addLeds<APA102, R_LED_DATA_PIN, R_LED_CLOCK_PIN, BGR>(r_leds, NUM_LEDS);

}

// looped repeatedly forever
void loop(){
  
  // check input
  chkin();

  // run light pattern
  mode::run();

    
  // in case lights are too bright we might wanna add a brightness pot
  // here I wanted minimum brightness to be 50, might need some adjustments later
  FastLED.setBrightness(analogRead(POT_DIMMER_PIN) / 4);

  #ifdef DEBUG_MODE // if flickery
    //Serial.println(analogRead(POT_DIMMER_PIN) / 4);
  #endif
  
  // apply the colors
  FastLED.show();

  FastLED.delay(1000 / 200); // 200 fps (prolly less)

  //while(Serial.available()) { Serial.read(); }
}
