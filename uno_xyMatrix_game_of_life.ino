#include <FastLED.h>

#define LED_PIN  3

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( y & 0x01) {
    // Odd rows run backwards
    uint8_t reverseX = (kMatrixWidth - 1) - x;
    i = (y * kMatrixWidth) + reverseX;
  } else {
    // Even rows run forwards
    i = (y * kMatrixWidth) + x;
  }

  return i;
}

CRGB leds[ NUM_LEDS ];

// Demo that USES "XY" follows code below


int16_t last[16];
int16_t universe[16];
int16_t buff[16];

void sync_buffer(int16_t* a, int16_t* b) {
  for (byte i = 0; i < 16; i++) {
    b[i] = a[i];
  }
}

bool get_cell(int16_t *arr, uint8_t x, uint8_t y) {
  uint16_t offset = (y * 16) + x;
  uint8_t row = offset / 16;
  uint8_t col = offset % 16;
  return (arr[row] & (0x01 << col)) != 0x00;
}

void set_cell(int16_t *arr, uint8_t x, uint8_t y, bool val) {
  uint16_t offset = (y * 16) + x;
  uint8_t row = offset / 16;
  uint8_t col = offset % 16;
  if (val) {
    arr[row] |= (0x01 << col);
  }
  else {
    arr[row] &= ~(0x01 << col);
  }
  
}

void update_cell_state(uint8_t x, uint8_t y) {
  uint8_t live = 0;
  for(int8_t i = -1; i < 2; i++) {
    for(int8_t j = -1; j < 2; j++) {
      if(i == j && j == 0) {
        continue;
      }
      if(get_cell(universe, (16+x+i)%16, (16+y+j)%16)) {
        live++;
      }
    }
  }
  if (live < 2 || live > 3) {
    set_cell(buff, x, y, false);
  }
  else {
    if(get_cell(universe, x, y)) {
      set_cell(buff, x, y, true);    
    }
    else if (live == 3) {
      set_cell(buff, x, y, true);
    }
    else {
      set_cell(buff, x, y, false);
    }
  }
}


void DrawOneFrame(int32_t n)
{
  for( byte y = 0; y < kMatrixHeight; y++) {
    for( byte x = 0; x < kMatrixWidth; x++) {
      uint16_t offset = XY(x, y);
      if (get_cell(universe, x, y)) {
        if (!get_cell(last, x, y)) {
//          leds[offset] += CHSV( (n/10) % 255, 255, 128);
//          leds[offset] /= 2;
          leds[offset] += CHSV( (n/10) % 255, 255, 128);
//          leds[offset]  = CRGB::White;
        }
      }
      else {
        if (leds[offset].r == leds[offset].g && leds[offset].g == leds[offset].b && leds[offset].b != 0) {
            leds[offset] = CHSV( (n/10) % 255, 255, 128);
        }
//        leds[ XY(x, y)].fadeToBlackBy(255);
//        leds[ XY(x, y)].fadeToBlackBy(196);
//        leds[ XY(x, y)].fadeToBlackBy(128);
        leds[ XY(x, y)].fadeToBlackBy(32);
//        leds[ XY(x, y)].fadeToBlackBy(16);
      }
    }
  }

  for( byte y = 0; y < kMatrixHeight; y++) {
    for( byte x = 0; x < kMatrixWidth; x++) {
      update_cell_state(x, y);
    }
  }

  sync_buffer(universe, last);
  sync_buffer(buff, universe);
}


uint16_t count = 0;
void loop()
{
    uint32_t ms = millis();
    if (random(25) == 1) {
      universe[random(16)]+= 15 << random(12);
    }
    DrawOneFrame(ms);
    count++;
    FastLED.show();
//    FastLED.delay(1000/30);
//    FastLED.delay(1000/4);
}

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  


//BOMB
//  universe[4] = 7<<7;
//  universe[3] = 1<<7;
//  universe[2] = 1<<8;
//
//  universe[1] = 7<<9;
//  universe[2] = 1<<9;
//  universe[3] = 1<<11;


//MULTIGLIDER
//  universe[12] = 7<<9;
//  universe[11] = 1<<9;
//  universe[10] = 1<<10;

//  universe[8] = 1<<10;
//  universe[7] = 1<<9;
//  universe[6] = 7<<9;
//
//  universe[4] = 7<<4;
//  universe[3] = 1<<4;
//  universe[2] = 1<<5;

//R-PENTOMINO
//  universe[4] = 1<<7;
//  universe[3] = 7<<7;
//  universe[2] = 1<<8;


//GLIDER
//  universe[4] = 7<<7;
//  universe[3] = 1<<7;
//  universe[2] = 1<<8;
  
//RANDOM
//  for (int16_t i = 0; i < 16; i++) {
//    universe[i] = random(0, 65536);
//  }

//BLINKER
//  universe[4] = 7<<7;

//  universe[7] = 7<<7;
//  universe[4] = 7<<7;

//    universe[7] = 4088;

}
