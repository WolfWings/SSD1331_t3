/*
 draw pixels via a callback fnuction

 This sketch demonstrates the drawCallbackPixels function by
 decompressing a pair of simple images directly to the OLED,
 without needing more than a handful of bytes of memory.
 
 Created on 2015-Sep-18
 by Wolf (wolfwings@gmail.com)
*/

#include <SGL.h>
#include "SSD1331_t3.h"
#include <SPI.h>
#include "glowy.h"
#include "redpanda.h"

#define sclk   13
#define mosi   11
#define oc     10
#define dc     9
#define rst    8

SSD1331_t3 oled = SSD1331_t3(oc, dc, rst, mosi, sclk);

struct _bitmap_state {
  uint8_t *curByte;
  uint8_t count;
  uint16_t color;
  uint16_t *colors;
  uint8_t minRLEcolor;
} bitmap_state;

uint16_t pixelCallback(uint16_t x, uint16_t y, void *state) {
  struct _bitmap_state *myState = (struct _bitmap_state *)state;
  uint8_t i;
  if (myState->count > 0) {
    myState->count--;
  } else {
    i = *myState->curByte++;
    myState->color = myState->colors[i];
    if (i < myState->minRLEcolor) {
      myState->count = 0;
    } else {
      myState->count = *myState->curByte++;
    }
  }
  return myState->color;
}

void setup() {
  oled.begin();
  oled.fillScreen(0xF800);
  oled.fillScreen(0);
}

bool i = false;

void loop() {
  if (i) {
    bitmap_state.curByte = redpanda_rle;
    bitmap_state.count = 0;
    bitmap_state.color = 0;
    bitmap_state.colors = redpanda_colors;
    bitmap_state.minRLEcolor = redpanda_minRLEcolor;
  } else {
    bitmap_state.curByte = glowy_eyes_rle;
    bitmap_state.count = 0;
    bitmap_state.color = 0;
    bitmap_state.colors = glowy_eyes_colors;
    bitmap_state.minRLEcolor = glowy_eyes_minRLEcolor;
  }
  oled.drawCallbackPixels(0, 0, 95, 63, pixelCallback, &bitmap_state);
  i = !i;
  delay(1000);
}

