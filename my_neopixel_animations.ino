#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 300 // Number of LEDs in strip

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// Hardware SPI: must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

#include "Larsen1.h"

class BrightToDim
{
  int fadeAmount;     // amount to dim per fade interval
  int fadeIntervalMs;    // milliseconds between fade steps
  unsigned int ledIndex;  // where our led is

  // These maintain the current state
  unsigned long previousMillis;   // will store last time LED was updated
  uint8_t cRed = 0;
  uint8_t cGreen = 0;
  uint8_t cBlue = 0xFF;

  void decrement(uint8_t& val, uint8_t amount) {
    if( val > amount) {
      val -= amount;
    } else {
      val = 0;
    }
  }

  public:
    BrightToDim() {
      static unsigned int led_index = 0;
      
      ledIndex = led_index++;

      // 10% chance of on at initialize
      if( random(100) < 10 ) {
        cRed = random(255);
        cGreen = random(255);
        cBlue = random(255);
      } else {
        cRed = 0;
        cGreen = 0;
        cBlue = 0;
      }

      fadeAmount = 1;
      fadeIntervalMs = random(1,8);
    }
    
    BrightToDim(uint8_t c_red, uint8_t c_green, uint8_t c_blue, unsigned int led_index, int fade_amount, int fade_interval_ms) {
      cRed = c_red;
      cGreen = c_green;
      cBlue = c_blue;
      fadeAmount = fade_amount;
      fadeIntervalMs = fade_interval_ms;
      ledIndex = led_index;
      previousMillis = 0;
    }

    // returns false if the dot is black
    bool Update(unsigned long curMillis, Adafruit_NeoPixel& strip) {
      if( curMillis > previousMillis && curMillis - previousMillis > fadeIntervalMs ) {
        // time to update
        decrement(cRed, fadeAmount);
        decrement(cGreen, fadeAmount);
        decrement(cBlue, fadeAmount);
        previousMillis = curMillis;
        strip.setPixelColor(ledIndex, cRed, cGreen, cBlue);
        strip.show();
      }
      return cRed > 0 || cGreen > 0 || cBlue > 0;
    }

    void Reset( uint8_t c_red, uint8_t c_green, uint8_t c_blue, uint16_t idx, unsigned long curMillis, unsigned long delayMs ) {
      cRed = c_red;
      cGreen = c_green;
      cBlue = c_blue;
      ledIndex = idx;
      previousMillis = curMillis + delayMs;
    }
    
};



void setup() {

//  Serial.begin(9600);

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  randomSeed(analogRead(0));
}

void doMulticolorStars(unsigned long duration) {
  const int NUMSTARS = NUMPIXELS / 10;
  BrightToDim stars[NUMSTARS];
  unsigned long now = millis();
  unsigned long end = now + duration;

  for(int i = 0; i < NUMSTARS; ++i) {
    stars[i].Reset(random(256),random(256),random(256), random(NUMPIXELS), now, random(0, 6000));
  }
  
  while( now < end ) {
    now = millis();
    for(int i = 0; i < NUMSTARS; ++i) {
      bool res = stars[i].Update(now, strip);
      if( !res ) {
        stars[i].Reset(random(256),random(256),random(256), random(NUMPIXELS), now, random(0, 6000));
      }
    }
  }
}

void doRedGreenStars(unsigned long duration) {
  const int NUMSTARS = NUMPIXELS / 10;
  BrightToDim stars[NUMSTARS];
  unsigned long now = millis();
  unsigned long end = now + duration;

  for(int i = 0; i < NUMSTARS; ++i) {
    uint8_t cflag = random(1, 3); // 1 or 2 
    stars[i].Reset(255 * (cflag & 0x1),
                   255 * ((cflag & 0x2) >> 1),
                   0, random(NUMPIXELS), now, random(0, 6000));
  }
  
  while( now < end ) {
    now = millis();
    for(int i = 0; i < NUMSTARS; ++i) {
      bool res = stars[i].Update(now, strip);
      if( !res ) {
    uint8_t cflag = random(1, 3); // 1 or 2 
    stars[i].Reset(255 * (cflag & 0x1),
                   255 * ((cflag & 0x2) >> 1),
                   0, random(NUMPIXELS), now, random(0, 6000));
      }
    }
  }
}

void doBlueWhiteStars(unsigned long duration) {
  const int NUMSTARS = NUMPIXELS / 10;
  BrightToDim stars[NUMSTARS];
  unsigned long now = millis();
  unsigned long end = now + duration;

  for(int i = 0; i < NUMSTARS; ++i) {
    uint8_t cflag = random(2); // 0 or 1 
    stars[i].Reset(255 * cflag,
                   255 * cflag,
                   255, random(NUMPIXELS), now, random(0, 6000));
  }
  
  while( now < end ) {
    now = millis();
    for(int i = 0; i < NUMSTARS; ++i) {
      bool res = stars[i].Update(now, strip);
      if( !res ) {
    uint8_t cflag = random(2); // 0 or 1 
    stars[i].Reset(255 * cflag,
                   255 * cflag,
                   255, random(NUMPIXELS), now, random(0, 6000));
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint8_t check_collision(uint8_t d1, uint8_t d2, int16_t b1, int16_t b2, int16_t a1, int16_t a2) {
  return false;
  if( d1 != d2 ) {

    // ignore stuff right near the ends
    if( b1 < 2 || b2 < 2 || b1 > NUMPIXELS - 2 || b2 > NUMPIXELS - 2 ) {
//      return 0;
    }
    
    // if the befores are different and now they are the same, that's a collision
    // if they 'pass through' each other in one step that's a collision too
    if( (b1 != b2 && a1 == a2) ||
        ( b1 > b2 && a1 < a2 ) ||
        ( b1 < b2 && a1 > a2 ) ) {
      return 1;
    }
  }
  return 0;
}

void addToComposite(const uint8_t* const in, uint16_t hue, Adafruit_NeoPixel& strip ) {
  uint16_t redsum;
  uint16_t grnsum;
  uint16_t blusum;
  for( int i = 0; i < NUMPIXELS; ++i ) {
    // get rgb value for incoming hue/val data
    uint32_t newColor = Adafruit_NeoPixel::ColorHSV(hue, 0xFF, in[i]);
    // get existing values to add to
    uint32_t baseColor = strip.getPixelColor(i);
    redsum = ((newColor >> 16) & 0xFF) + ((baseColor >> 16) & 0xFF);
    grnsum = ((newColor >> 8) & 0xFF) + ((baseColor >> 8) & 0xFF);
    blusum = (newColor & 0xFF) + (baseColor & 0xFF);
    if( redsum > 255) redsum = 255;
    if( grnsum > 255) grnsum = 255;
    if( blusum > 255) blusum = 255;
    strip.setPixelColor(i, uint8_t(redsum), uint8_t(grnsum), uint8_t(blusum));
  }
  
}

uint16_t getHueDelta(const int32_t a, const int32_t b) {
  int32_t d = a - b;
  if( d < 0 ) {
    d *= -1; 
  }
  if( d > 32767 ) {
    int32_t d1, d2;
    if( a > 32767 ) {
      d1 = 65536 - a;
    } else {
      d1 = a;
    }
    if( b > 32767 ) {
      d2 = 65536 - b;
    } else {
      d2 = b;
    }
    d = d1 + d2;
  }
  return d;
}

void doBounceLarsen(unsigned long duration) {
  unsigned long now = millis();
  unsigned long end = now + duration;
  const uint16_t hue1 = random(65536);
  uint16_t hue2 = random(65536);
  int32_t delta = getHueDelta(hue1, hue2);
  if( delta < 0 ) delta *= -1;
  const int16_t space = 10000;
  while( delta < space ) {
    hue2 = random(65536);
    delta = getHueDelta(hue1, hue2);
  }
  uint16_t hue3 = random(65536);
  delta = getHueDelta(hue2, hue3);
  int32_t delta2 = getHueDelta(hue1, hue3);
  while( delta < space  || delta2 < space) {
    hue3 = random(65536);
    delta = getHueDelta(hue2, hue3);
    delta2 = getHueDelta(hue1, hue3);
  }

  // min, max, len, speed, hue, mode
  Larsen1 l1(0, NUMPIXELS, 5, random(1, 15), hue1, random(4) & 14);
  Larsen1 l2(0, NUMPIXELS, random(5, 50), random(1, 15), hue2, random(4) & 14);
  Larsen1 l3(0, NUMPIXELS, random(5, 150), random(1, 15), hue3, random(4) & 14);
  l1.SetMaxBright(192);
  l2.SetMaxBright(192);
  l3.SetMaxBright(192);
  // second one starts at the far end
  l2.SetHeadPos(NUMPIXELS -1);
  l2.SetDirection( -1 );
  // third one starts in the middle
  l3.SetHeadPos(NUMPIXELS / 2);
  // middle one picks random direction
  if( random(2) ) {
    l3.SetDirection( -1 );
  }
  uint8_t valOut[NUMPIXELS];
  uint8_t updated = 0;
  uint8_t redraw = 0;
  uint16_t hueOut;

  while( now < end ) {
    now = millis();
    uint16_t redsum = 0;
    uint16_t grnsum = 0;
    uint16_t blusum = 0;
    int16_t l1_before = l1.GetHeadPos();
    int16_t l2_before = l2.GetHeadPos();
    int16_t l3_before = l3.GetHeadPos();
    bool res = l1.Update(now, valOut, hueOut, NUMPIXELS, updated);
    if( !res ) {
      l1.Reset(random(1, 50));
    }
    if( updated ) {
      redraw = 1;
    }
    
    res = l2.Update(now, valOut, hueOut, NUMPIXELS, updated);
    if( !res ) {
      l2.Reset(random(1, 50));
    }
    if( updated ) {
      redraw = 1;
    }

    res = l3.Update(now, valOut, hueOut, NUMPIXELS, updated);
    if( !res ) {
      l3.Reset(random(1, 50));
    }
    if( updated ) {
      redraw = 1;
    }

    if( redraw ) { 
      // collision tests
      int16_t l1_after = l1.GetHeadPos();
      int16_t l2_after = l2.GetHeadPos();
      int16_t l3_after = l3.GetHeadPos();
      uint8_t l1_dir = l1.GetDirection();
      uint8_t l2_dir = l2.GetDirection();
      uint8_t l3_dir = l3.GetDirection();

      uint8_t reverse_flags = 0;
      if( check_collision(l1_dir, l2_dir, l1_before, l2_before, l1_after, l2_after) ) {
        if( random(100) < 30 ) {
          reverse_flags |= 0x1;
          reverse_flags |= 0x2;
        }
      }
      if( check_collision(l1_dir, l3_dir, l1_before, l3_before, l1_after, l3_after) ) {
        if( random(100) < 30 ) {
          reverse_flags |= 0x1;
          reverse_flags |= 0x4;
        }
      }
      if( check_collision(l2_dir, l3_dir, l2_before, l3_before, l2_after, l3_after) ) {
        if( random(100) < 30 ) {
          reverse_flags |= 0x2;
          reverse_flags |= 0x4;
        }
      }
      if( (reverse_flags & 0x1) == 0x1 ) {
        l1.Reverse();
      }
      if( (reverse_flags & 0x2) == 0x2 ) {
        l2.Reverse();
      }
      if( (reverse_flags & 0x4) == 0x4 ) {
        l3.Reverse();
      }      

      strip.clear();
      
      l1.GetStripState(valOut, hueOut, NUMPIXELS);
      addToComposite(valOut, hueOut, strip);
      l2.GetStripState(valOut, hueOut, NUMPIXELS);
      addToComposite(valOut, hueOut, strip);
      l3.GetStripState(valOut, hueOut, NUMPIXELS);
      addToComposite(valOut, hueOut, strip);

      strip.show();
    }
  }

}


void doLarsen1(unsigned long now, Adafruit_NeoPixel& strip) {

  Larsen1 l(0, NUMPIXELS, random(10,70), 30, random(65536), 2);
  l.SetMaxBright(128);
  uint8_t valOut[NUMPIXELS];
  uint8_t updated = 0;
  uint16_t hueOut;
    
  while( l.Update(millis(), valOut, hueOut, NUMPIXELS, updated) ) {
    if( updated ) {
      for( int i = 0; i < NUMPIXELS; ++i ) {
        strip.setPixelColor(i, Adafruit_NeoPixel::ColorHSV(hueOut, 0xFF, valOut[i]));
      }
      strip.show();    
    }
  }
}

void gammaTest() {

  int increment = 10;
  uint8_t val = 0;
  for(int i = 0; i < NUMPIXELS; ++i) {
    strip.setPixelColor(i, strip.gamma32(strip.Color(val, 0, 0)));
    val += increment;
  }
  strip.show();
}

void loop() {

//  rainbow(20);

  unsigned long now = millis();
//  doLarsen1(now, strip);
  doBounceLarsen(30000);
//  strip.clear(); strip.show();
//  doRedGreenStars(30000);
//  strip.clear(); strip.show();
//  doBounceLarsen(60000);
//  strip.clear(); strip.show();
//  doMulticolorStars(30000);
//  strip.clear(); strip.show();
//  doBounceLarsen(60000);
//  strip.clear(); strip.show();
//  doBlueWhiteStars(30000);
//  strip.clear(); strip.show();

  // more ideas:


}
