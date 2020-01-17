#ifndef __LARSEN1_HEADER__
#define __LARSEN1_HEADER__

#include <stdint.h>
#include <Adafruit_NeoPixel.h>

class Larsen1 {
private:
  unsigned long previousMillis;   // will store last time LED was updated
  uint16_t mHue; // color of this scanner
  uint8_t mMaxVal; // max brightness
  uint16_t mMinIdx; // lower led idx bound for effect
  uint16_t mMaxIdx; // upper led idx bound for effect
  uint8_t mLength; // length of scanner tail
  uint8_t mSpeed;  // speed of scanner effect
  int16_t mHeadPos; // where the head of the scanner is
  int16_t mHeadMin; // where the head of the scanner turns around low end
  int16_t mHeadMax; // where the head of the scanner turns around high end
  int8_t  mHeadDir; // which direction the scanner is going, +1 or -1
  uint8_t mFadeStep; // change in brightness per index
  uint8_t mMode; // bit 1: bounce at edge or fall off, bit 2: speed up until quit
public:
    Larsen1();    
    Larsen1(uint16_t minIdx, uint16_t maxIdx, uint8_t aLength, uint8_t aSpeed, uint16_t aHue, uint8_t aMode);
    
    // returns false if needs reset
    bool Update(unsigned long curMillis, Adafruit_NeoPixel& strip);
    // takes a 32-bit color array and len
    // updated: out param indicating if strip was modified
    bool Update(unsigned long curMillis, uint8_t* strip, uint16_t& hueOut, const uint16_t stripLen, uint8_t& updated);
    void Reset(uint8_t aSpeed);
    void Reverse();
    void SetMaxBright(uint8_t val);
    void SetDirection(int8_t dir);
    void SetHeadPos(int16_t pos);
    int8_t GetDirection();
    int16_t GetHeadPos();
    void GetStripState(uint8_t* strip, uint16_t& hueOut, uint16_t stripLen);
};
#endif __LARSEN1_HEADER__
