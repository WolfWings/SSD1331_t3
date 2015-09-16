/*
* SSD1331_t3.h
* A library for RGB OLED module
*
* Copyright (c) 2015, Wolf
* Copyright (c) 2014, seeed technology inc.
* Copyright (c) 2012, Adafruit Industries.
*
* All rights reserved.
*
* This library is rebuilt to take full advantage of the Teensy 3.x SPI FIFO
* capabilities, based on several existing libraries:
*
* @ Low-level SPIFIFO code based on ILI9341_t3 library, extended to match the
*   semantics used by the SSD1331 chipset.
*
* @ SGL-based graphics library is Seeed Studios, the interfaces here extend
*   their work, and are based on their SSD1331 library derivative.
*
* @ The core SSD1331 library is based on the original AdaFruit SSD1331 code,
*   as well as the original datasheet for the chipset.
*
* ****************************************************************************
* This is a library for the 0.96" 16-bit Color OLED with SSD1331 driver chip
*
*  Pick one up today in the adafruit shop!
*  ------> http://www.adafruit.com/products/684
*
* These displays use SPI to communicate.
*
* Adafruit invests time and resources providing this open source code, 
* please support Adafruit and open-source hardware by purchasing 
* products from Adafruit!
*
* Written by Limor Fried/Ladyada for Adafruit Industries.
* Modified by lawliet for Seeed Studio's RGB OLED module.
* Modified by Wolf to take full advantage of Teensy 3.x SPIFIFO
* BSD license, all text above must be included in any redistribution
* ****************************************************************************
*
* Software License Agreement (BSD License)
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holders nor the
* names of its contributors may be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
// https://github.com/WolfWings/SSD1331_t3

#include "SSD1331_t3.h"
#include <SPI.h>

// Teensy 3.1 can only generate 30 MHz SPI when running at 120 MHz (overclock)
// At all other speeds, SPI.beginTransaction() will use the fastest available clock
#define SPICLOCK 30000000

// The "CS" pin is called "OC" on the AdaFruit OLED + MicroSD breakout
SSD1331_t3::SSD1331_t3(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sclk) : SGL(RGB_OLED_WIDTH, RGB_OLED_HEIGHT)
{
	_cs   = cs;
	_dc   = dc;
	_rst  = rst;
	_mosi = mosi;
	_sclk = sclk;
}

static const uint8_t init_commands[] = {
	CMD_DISPLAY_OFF,                   // Display Off
	CMD_SET_CONTRAST_A,          0x91, // Set contrast for color A
	CMD_SET_CONTRAST_B,          0x50, // Set contrast for color B
	CMD_SET_CONTRAST_C,          0x7D, // Set contrast for color C
	CMD_MASTER_CURRENT_CONTROL,  0x06, // Master current control
	CMD_SET_PRECHARGE_SPEED_A,   0x64, // Set Second Pre-change Speed For ColorA
	CMD_SET_PRECHARGE_SPEED_B,   0x78, // Set Second Pre-change Speed For ColorB
	CMD_SET_PRECHARGE_SPEED_C,   0x64, // Set Second Pre-change Speed For ColorC
	CMD_SET_REMAP,               0x72, // Set remap & data format
	CMD_SET_DISPLAY_START_LINE,  0x00, // Set display Start Line
	CMD_SET_DISPLAY_OFFSET,      0x00, // Set display offset
	CMD_NORMAL_DISPLAY,                // Set display mode
	CMD_SET_MULTIPLEX_RATIO,     0x3F, // Set multiplex ratio
	CMD_SET_MASTER_CONFIGURE,    0x8E, // Set master configuration
	CMD_POWER_SAVE_MODE,         0x00, // Set Power Save Mode
	CMD_PHASE_PERIOD_ADJUSTMENT, 0x31, // Phase 1 and 2 period adjustment
	CMD_DISPLAY_CLOCK_DIV,       0xF0, // Display clock divider/oscillator frequency
	CMD_SET_PRECHARGE_VOLTAGE,   0x3A, // Set Pre-Change Level
	CMD_SET_V_VOLTAGE,           0x3E, // Set vcomH
	CMD_DEACTIVE_SCROLLING,            // Disable scrolling
};

void SSD1331_t3::begin(void) {
  const uint8_t items = sizeof(init_commands) / sizeof(init_commands[0]);
  uint8_t count;

  // verify SPI pins are valid;
  if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {
    SPI.setMOSI(_mosi);
    SPI.setSCK(_sclk);
	} else {
    return; // not valid pins...
	}
  
	SPI.begin();
	if (SPI.pinIsChipSelect(_cs, _dc)) {
		pcs_data = SPI.setCS(_cs);
		pcs_command = pcs_data | SPI.setCS(_dc);
	} else {
		pcs_data = 0;
		pcs_command = 0;
		return;
	}

  // Toggle the RST pin if requested on startup.
	if (_rst < 255) {
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);
		delayMicroseconds(3);
		digitalWrite(_rst, LOW);
		delayMicroseconds(3);
		digitalWrite(_rst, HIGH);
	}

	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  if (items & 1) {
    writecommand8_cont(init_commands[0]);
  }
  for (count = (items & 1); count < items; count += 2) {
    writecommand16_cont_split(init_commands[count], init_commands[count + 1]);
  }
  writecommand8_last(CMD_NORMAL_BRIGHTNESS_DISPLAY_ON);
	SPI.endTransaction();
}

void SSD1331_t3::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	if((x >= RGB_OLED_WIDTH) || (y >= RGB_OLED_HEIGHT)) return;

  SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  setAddr(x, y, RGB_OLED_WIDTH - 1, RGB_OLED_HEIGHT - 1);
  writedata16_last(color);
  SPI.endTransaction();
}

int16_t SSD1331_t3::drawLine_nodelay(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
  int16_t xd, yd, delayNeeded;

  SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  if (x0 >= RGB_OLED_WIDTH)  return 0;
  writecommand16_cont_split(CMD_DRAW_LINE, x0);
  if (y0 >= RGB_OLED_HEIGHT) return 0;
  if (x1 >= RGB_OLED_WIDTH)  return 0;
  writecommand16_cont_split(y0, x1);
  if (y1 >= RGB_OLED_HEIGHT) return 0;
  xd = (x1 - x0);
  writecommand16_cont_split(y1, (color & 0xF800) >> 11);
  if (xd < 0) { xd = -xd; }
  yd = (y1 - y0);
  if (yd < 0) { yd = -yd; }
  delayNeeded = (((xd > yd) ? xd : yd) / 16);
  writecommand16_last_split((color & 0x03E0) >> 5, color & 0x001F);
  SPI.endTransaction();
  return delayNeeded;
}

int16_t SSD1331_t3::drawRectangle_nodelay(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
  int16_t xd, yd, delayNeeded;

  SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  writecommand16_cont_split(CMD_FILL_WINDOW, DISABLE_FILL);
  if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_WIDTH - 1;
  if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_HEIGHT - 1;
  writecommand16_cont_split(CMD_DRAW_RECTANGLE, x0);
  if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_WIDTH - 1;
  if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_HEIGHT - 1;
  writecommand16_cont_split(y0, x1);
  xd = x1 - x0;
  yd = y1 - y0;
  writecommand16_cont_split(y1, (color & 0xF800) >> 11);
  if (xd < 0) { xd = -xd; }
  if (yd < 0) { yd = -yd; }
  writecommand16_cont_split((color & 0x3E0) >> 5, color & 0x1F);
  delayNeeded = ((xd + 1) * (yd + 1));
  writecommand16_cont_split(0, 0);
  delayNeeded = (delayNeeded / 16) - 32;
  if (delayNeeded < 0) {
    delayNeeded = 0;
  }
  writecommand8_last(0);
  SPI.endTransaction();
  return delayNeeded;
}

int16_t SSD1331_t3::drawFrame_nodelay(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t outColor, uint16_t fillColor) {
  int16_t xd, yd, delayNeeded;

  SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  writecommand16_cont_split(CMD_FILL_WINDOW, ENABLE_FILL);
  if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_WIDTH - 1;
  if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_HEIGHT - 1;
  writecommand16_cont_split(CMD_DRAW_RECTANGLE, x0);
  if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_WIDTH - 1;
  if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_HEIGHT - 1;
  writecommand16_cont_split(y0, x1);
  xd = x1 - x0;
  yd = y1 - y0;
  writecommand16_cont_split(y1, (outColor & 0xF800) >> 11);
  if (xd < 0) { xd = -xd; }
  if (yd < 0) { yd = -yd; }
  writecommand16_cont_split((outColor & 0x3E0) >> 5, outColor & 0x1F);
  delayNeeded = ((xd + 1) * (yd + 1));
  writecommand16_cont_split((fillColor & 0xF800) >> 11, (fillColor & 0x03E0) >> 5);
  delayNeeded = (delayNeeded / 16) - 32;
  if (delayNeeded < 0) {
    delayNeeded = 0;
  }
  writecommand8_last(fillColor & 0x1F);
  SPI.endTransaction();
  return delayNeeded;
}

void SSD1331_t3::copyWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2) {
	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  writecommand8_cont(CMD_COPY_WINDOW);
  writecommand16_cont_split(x0, y0);
  writecommand16_cont_split(x1, y1);
  writecommand16_last_split(x2, y2);
  SPI.endTransaction();
}

void SSD1331_t3::dimWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  writecommand8_cont(CMD_DIM_WINDOW);
  writecommand16_cont_split(x0, y0);
  writecommand16_last_split(x1, y1);
  SPI.endTransaction();
}

void SSD1331_t3::clearWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  writecommand8_cont(CMD_CLEAR_WINDOW);
  writecommand16_cont_split(x0, y0);
  writecommand16_last_split(x1, y1);
  SPI.endTransaction();
}

void SSD1331_t3::setScolling(ScollingDirection direction, uint8_t rowAddr, uint8_t rowNum, uint8_t timeInterval) {
  uint8_t scrolling_horizontal;
  uint8_t scrolling_vertical;
  switch(direction){
    case Horizontal:
      scrolling_horizontal = 0x01;
      scrolling_vertical = 0x00;
      break;
    case Vertical:
      scrolling_horizontal = 0x00;
      scrolling_vertical = 0x01;
      break;
    case Diagonal:
      scrolling_horizontal = 0x01;
      scrolling_vertical = 0x01;
      break;
    default:
      scrolling_horizontal = 0x00;
      scrolling_vertical = 0x00;
      break;
  }
	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  writecommand16_cont_split(CMD_CONTINUOUS_SCROLLING_SETUP, scrolling_horizontal);
  writecommand16_cont_split(rowAddr, rowNum);
  writecommand16_cont_split(scrolling_vertical, timeInterval);
  writecommand8_last(CMD_ACTIVE_SCROLLING);
  SPI.endTransaction();
}

void SSD1331_t3::enableScolling(bool enable) {
	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  if (enable) {
    writecommand8_last(CMD_ACTIVE_SCROLLING);
  } else {
    writecommand8_last(CMD_DEACTIVE_SCROLLING);
  }
  SPI.endTransaction();
}

void SSD1331_t3::setDisplayMode(DisplayMode mode) {
	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  writecommand8_last(mode);
  SPI.endTransaction();
}

void SSD1331_t3::setDisplayPower(DisplayPower power) {
	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  writecommand8_last(power);
  SPI.endTransaction();
}
