#ifndef MODES_H
#define MODES_H

#include <Arduino.h>
#include <inttypes.h>
#include <FastLED.h>


// rainbow colors
namespace mode0 {

  
  uint16_t hue;
  
  // cycle through the rainbow
  void cycle3(uint8_t& v0, uint8_t& v1, uint8_t& v2, uint8_t& curHi) {
    // shift values
    if (curHi == 0) {
      v0--; v1++;
    } else if (curHi == 1) {
      v1--; v2++;
    } else if (curHi == 2) {
      v2--; v0++;
    }
  
    // allign curhi
    if (v0 <= 0 && curHi == 0)
      curHi = 1;
    else if (v1 <= 0 && curHi == 1)
      curHi = 2;
    else if (v2 <= 0 && curHi == 2)
      curHi = 0;
  }
  
  // run at start of mode
  void init() {
    hue = 0;
  
  #ifdef DEBUG_MODE
    Serial.println("switched to rainbow mode");
  #endif
  
  }
  
  // run each frame of mode
  void periodic() {
    // initialize the rainbow
    CRGB clr(255, 0, 0);
    uint8_t curHi = 0;
  
    // shift the rainbow
    for (int i = hue % 768; i > 0; i--)
      cycle3(clr.r, clr.g, clr.b, curHi);
  
    // set the colors
    for (int i = 0; i < NUM_LEDS; i++) {
      l_leds[i] = clr;
      r_leds[i] = clr;
      cycle3(clr.r, clr.g, clr.b, curHi);
    }
  
    hue++;
  }
}

// red mode (red alliance)
namespace mode1 {
  
  bool goingRight;
  uint8_t curLED;
  
  
  void init() {
    goingRight = true;
    curLED = 0;
  
  #ifdef DEBUG_MODE
    Serial.println("switched to red cylon");
  #endif
  
  
  }
  
  void periodic() {
  
    size_t i = curLED;
  
    if (goingRight) {
  
      if (curLED < 20)
        curLED += 20;
      
      for (; i > 0 && curLED - i < 20; i--) {
        l_leds[i].setRGB(255, 0, 0);
        r_leds[i].setRGB(255, 0, 0);
      }
  
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
  
      // move (and possibly switch directions)
      if (++curLED == NUM_LEDS - 1)
        goingRight = false;
  
    } else {
      if (curLED > NUM_LEDS - 20)
        curLED -= 20;
  
      for (; i < NUM_LEDS - 1 && i - curLED < 20; i++) {
        l_leds[i].setRGB(255, 0, 0);
        r_leds[i].setRGB(255, 0, 0);
      }
  
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
  
      // move (and possibly switch directions)
      if (--curLED == 1)
        goingRight = true;
    }
  
    delay(5);
  }

}

// blue mode (blue alliance)
namespace mode2 {
  bool goingRight;
  uint8_t curLED;
  
  
  void init() {
    goingRight = true;
    curLED = 0;
  
  
  #ifdef DEBUG_MODE
    Serial.println("switched to blue cylon");
  #endif
  
  
  }
  
  void periodic() {
  
    uint16_t i = curLED;
  
    if (goingRight) {
  
      if (curLED < 20)
        curLED += 20;
  
      for (; i > 0 && curLED - i < 20; i--) {
        l_leds[i].setRGB(0, 0, 255);
        r_leds[i].setRGB(0, 0, 255);
      }
  
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
  
      // move (and possibly switch directions)
      if (++curLED == NUM_LEDS - 1)
        goingRight = false;
  
    } else {
      if (curLED > NUM_LEDS - 20)
        curLED -= 20;
  
      for (; i < NUM_LEDS - 1 && i - curLED < 20; i++) {
        l_leds[i].setRGB(0, 0, 255);
        r_leds[i].setRGB(0, 0, 255);
      }
  
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
  
      // move (and possibly switch directions)
      if (--curLED == 1)
        goingRight = true;
    }
  
    delay(5);
  }

}


// based off of fire2012 demo
// https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino
namespace mode3 {
  
  // temperature readings
  uint8_t l_heat[NUM_LEDS];
  uint8_t r_heat[NUM_LEDS];
  
  void fire(CRGB* leds, uint8_t* heat, const bool reverse = false) {
  
    // COOLING: How much does the air cool as it rises?
    // Less cooling = taller flames.  More cooling = shorter flames.
    // Default 50, suggested range 20-100
    const uint8_t cooling = 100;
  
    // SPARKING: What chance (out of 255) is there that a new spark will be lit?
    // Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    const uint8_t sparking = 120;
  
    // Cool down every cell a little
    for (uint16_t i = 0; i < NUM_LEDS; i++)
      heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / NUM_LEDS) + 2));
  
    // Heat from each cell drifts 'up' and diffuses a little
    for (uint16_t k = NUM_LEDS - 1; k >= 2; k--)
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  
    // Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < sparking) {
      unsigned char y = random8(7);
      heat[y] = qadd8(heat[y], random8(160, 255));
    }
  
    // Map from heat cells to LED colors
    for (int j = 0; j < NUM_LEDS; j++) {
  
      CRGB color = HeatColor(heat[j]);
  
      uint16_t pixelNum = reverse ? (NUM_LEDS - 1) - j : j;
  
      leds[pixelNum] = color;
    }
  }
  
  void init() {
  #ifdef DEBUG_MODE
    Serial.println("switched fire mode");
  #endif
  
  }
  
  
  void periodic() {
    fire(l_leds, l_heat);
    fire(r_leds, r_heat);
  }
}


// super lame basic switch simulator
namespace mode4 {
  
  bool redCtl = false;
  bool lit = true;
  uint8_t frame = 0;
  uint8_t slidein = 0;
  
  void init() {
    #ifdef DEBUG_MODE
      Serial.println("switched to switch mode");
    #endif

    for (uint8_t i = 0; i < NUM_LEDS; i++)
      l_leds[i] = r_leds[i] = CRGB::White;
    
  
    redCtl = false;
    lit = true;
    frame = 0;
    slidein = 0;
  }
  
  void periodic() {

    // startup
    if (slidein < NUM_LEDS / 2) {
      l_leds[slidein] = CRGB::Red;
      l_leds[NUM_LEDS - 1 - slidein] = CRGB::Red;
      r_leds[slidein] = CRGB::Blue;
      r_leds[NUM_LEDS - 1 - slidein] = CRGB::Blue;
      
      slidein++;
    } else {
      // every 10th frame we flicker
      if (++frame % 10 == 0)
        lit = !lit;

      // after 10 flickers we reset
      if (frame > 100) {
        redCtl = !redCtl;
        frame = 0;
      }
    
      CRGB clr;

      // set color
      if (redCtl)
        clr = CRGB(lit ? 255 : 100, 0, 0);
      else
        clr = CRGB(0, 0, lit ? 255 : 100);

      // apply color
      if (redCtl)
        for (CRGB& c : l_leds)
          c = clr;
      else
        for (CRGB& c : r_leds)
          c = clr;

    }
  
  }
}



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



// 0-3
#define NUM_MODES 5

// mode related stuff
namespace mode {
  
  // current mode
  volatile uint8_t modeNum = 0;
  
  // switch modes
  void next() {
  
  #ifdef DEBUG_MODE
    Serial.println("switched modes");
  #endif
  
  
    // reset LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
    }
  
    // change mode number
    if (++modeNum == NUM_MODES)
      modeNum = 0;
  
    // initialize mode
    switch (modeNum) {
      case 0: mode0::init(); break;
      case 1: mode1::init(); break;
      case 2: mode2::init(); break;
      case 3: mode3::init(); break;
      case 4: mode4::init(); break;
    }
  }
  
  // run current mode
  void run() {
    switch (modeNum) {
      case 0: mode0::periodic(); break;
      case 1: mode1::periodic(); break;
      case 2: mode2::periodic(); break;
      case 3: mode3::periodic(); break;
      case 4: mode4::periodic(); break;
    }
  }

}


#endif

