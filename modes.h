#ifndef MODES_H
#define MODES_H


// rainbow colors
namespace mode0 {
  
  // cycle through the rainbow
  void cycle3(uint8_t& v0, uint8_t& v1, uint8_t& v2, uint8_t& curHi){
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
  
  struct {
    unsigned int offset : 24;
    uint8_t curHi;
  } rainbow;

  // run at start of mode
  void init(){
    rainbow.offset = 0;
  }

  // run each frame of mode
  void periodic(){
    // initialize the rainbow
    uint8_t r = 255, g = 0, b = 0;
    rainbow.curHi = 0;
  
    // shift the rainbow
    for (int i = rainbow.offset % 768; i > 0; i--)
      cycle3(r, g, b, rainbow.curHi);
  
    // set the colors
    for (int i = 0; i < NUM_LEDS; i++) {
      l_leds[i] = CRGB(r, g, b);
      r_leds[i] = CRGB(r, g, b);
      cycle3(r, g, b, rainbow.curHi);
    }
    
    rainbow.offset++; 
  }
}

// red mode (red alliance)
namespace mode1 {
  bool goingRight;
  uint16_t curLED;

  
  void init(){
    goingRight = true;
    curLED = 0;
  }
  
  void periodic(){
    
    size_t i = curLED;
    
    if (goingRight) {
      
      if (curLED < 20) 
        curLED += 20;
        
      for (; i > 0 && curLED - i < 20; i--) {
        l_leds[i] = CRGB(255, 0, 0);
        r_leds[i] = CRGB(255, 0, 0);
      }
      
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
      
      // move (and possibly switch directions)
      if (++curLED == NUM_LEDS)
        goingRight = false;
        
    } else {
      if (curLED > NUM_LEDS - 20)
        curLED -= 20;
        
      for (; i < NUM_LEDS - 1 && i - curLED < 20; i++) {
        l_leds[i] = CRGB(255, 0, 0);
        r_leds[i] = CRGB(255, 0, 0);
      }
      
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
      
      // move (and possibly switch directions)
      if (--curLED == 0)
        goingRight = true;
    }

    delay(5);
  }

}

// blue mode (blue alliance)
namespace mode2 {
  bool goingRight;
  uint16_t curLED;

  
  void init(){
    goingRight = true;
    curLED = 0;
  }
  
  void periodic(){
    
    size_t i = curLED;
    
    if (goingRight) {
      
      if (curLED < 20) 
        curLED += 20;
        
      for (; i > 0 && curLED - i < 20; i--) {
        l_leds[i] = CRGB(0, 0, 255);
        r_leds[i] = CRGB(0, 0, 255);
      }
      
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
      
      // move (and possibly switch directions)
      if (++curLED == NUM_LEDS)
        goingRight = false;
        
    } else {
      if (curLED > NUM_LEDS - 20)
        curLED -= 20;
        
      for (; i < NUM_LEDS - 1 && i - curLED < 20; i++) {
        l_leds[i] = CRGB(0, 0, 255);
        r_leds[i] = CRGB(0, 0, 255);
      }
      
      l_leds[i] = CRGB::Black;
      r_leds[i] = CRGB::Black;
      
      // move (and possibly switch directions)
      if (--curLED == 0)
        goingRight = true;
    }

    delay(5);
  }

}


namespace mode3 {

  
  void init(){
    
  }
  void periodic(){
    
  }
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// 0-3
#define NUM_MODES 4

// mode related stuff
namespace mode {

  // current mode
  uint8_t modeNum = 0;

  // switch modes
  void next(){
    
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
    }
  }

  // run current mode
  void run(){
    switch (modeNum) {
      case 0: mode0::periodic(); break;
      case 1: mode1::periodic(); break;
      case 2: mode2::periodic(); break;
      case 3: mode3::periodic(); break;
    }  
  }

}


#endif
