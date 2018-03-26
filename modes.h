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

  // relevant variables
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
    const uint8_t cooling = 55;
    
    // SPARKING: What chance (out of 255) is there that a new spark will be lit?
    // Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    const uint8_t sparking = 120;

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < NUM_LEDS; i++)
      heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / NUM_LEDS) + 2));
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = NUM_LEDS - 1; k >= 2; k--)
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if(random8() < sparking) {
      int y = random8(7);
      heat[y] = qadd8(heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      
      CRGB color = HeatColor(heat[j]);
      
      int pixelnumber;
      
      if (reverse)
        pixelnumber = (NUM_LEDS-1) - j;
      else
        pixelnumber = j;
      
      leds[pixelnumber] = color;
    }
  }
  
  void init(){ }

  void periodic(){
    fire(l_leds, l_heat);
    fire(r_leds, r_heat);
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

