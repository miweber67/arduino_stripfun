#include "Larsen1.h"

Larsen1::Larsen1() {
  Larsen1(0, 8, 4, 10, 0, 0);
}

Larsen1::Larsen1(uint16_t minIdx, uint16_t maxIdx, uint8_t aLength, uint8_t aSpeed, uint16_t aHue, uint8_t aMode) {
  previousMillis = 0;
  mHue = aHue;
  mMaxVal = 0xFF;
  mMinIdx = minIdx;
  mMaxIdx = maxIdx;
  mLength = aLength;
  mSpeed = aSpeed;
  mHeadDir = 1;
  mHeadPos = mMinIdx;
  mFadeStep = 255 / aLength;
  mMode = aMode;
  mHeadMin = mMinIdx;
  mHeadMax = mMaxIdx;

  if( (mMode & 0x1) == 0x1) {
    mHeadMin = mMinIdx - aLength;
    mHeadMax = mMaxIdx + aLength;
  }

}

// convert strip to uint8_t representing absolute VAL and have external code map it to rgb.
bool Larsen1::Update(unsigned long curMillis, uint8_t* strip, uint16_t& hueOut, const uint16_t stripLen, uint8_t& updated) {
  updated = 0;
  if( curMillis < previousMillis || curMillis - previousMillis < mSpeed ) {
    return true;
  }
  
  hueOut = mHue;
  for( uint16_t i = 0; i < stripLen; ++i ) {
    strip[i] = 0;
  }

  if( mSpeed == 0 ) {
    return false;
  }

  // advance head pos
  mHeadPos += mHeadDir;

  // light head
  if( mHeadPos >= mMinIdx && mHeadPos < mMaxIdx ) {
    strip[mHeadPos] = mMaxVal;
  }

  for( int16_t i = 0; i < mLength; ++i ) {
    int16_t curs = mHeadPos + (i * mHeadDir);
    if( curs >= 0 && curs < stripLen ) {
      // ok to draw
      int16_t res;
      if( mHeadPos > 0 ) {
        res = mMaxVal - (mFadeStep * i);
      } else {
        res = mMaxVal - (mFadeStep * (mLength - i));        
      }
      if( 0 <= res ) {
        strip[curs] = res;
      } else {
        break;
      }
    }
  }

  // check if we need to change direction
  if( mHeadPos == mHeadMin || mHeadPos == mHeadMax) {
    mHeadDir *= -1;
    if( (mMode & 0x2) == 0x2 ) {
      mSpeed--;
    }
  }

  previousMillis = curMillis;
  updated = 1;
  return true;
  
}


void Larsen1::Reset(uint8_t aSpeed) {
  mSpeed = aSpeed;  
}

void Larsen1::Reverse() {
  mHeadDir *= -1;  
}

int8_t Larsen1::GetDirection() {
  return mHeadDir;
}

int16_t Larsen1::GetHeadPos() {
  return mHeadPos;
}

void Larsen1::SetMaxBright(uint8_t val) {
  mMaxVal = val;
}

void Larsen1::SetDirection(int8_t dir) {
  mHeadDir = dir;  
}

void Larsen1::SetHeadPos(int16_t pos) {
  mHeadPos = pos;
}

void Larsen1::GetStripState(uint8_t* strip, uint16_t& hueOut, uint16_t stripLen) {
  hueOut = mHue;
  for( uint16_t i = 0; i < stripLen; ++i ) {
    strip[i] = 0;
  }
  for( int16_t i = 0; i < mLength; ++i ) {
    int16_t curs = mHeadPos - (i * mHeadDir);
    if( curs >= 0 && curs < stripLen ) {
      // ok to draw
      int16_t res;
      if( mHeadPos > 0 ) {
        res = mMaxVal - (mFadeStep * i);
      } else {
        res = mMaxVal - (mFadeStep * (mLength - i));        
      }
      if( 0 <= res ) {
        strip[curs] = mMaxVal - (mFadeStep * i);
      } else {
        break;
      }
    }
  }
}
