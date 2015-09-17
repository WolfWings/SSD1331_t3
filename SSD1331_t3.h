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

#ifndef _SSD1331_t3H_
#define _SSD1331_t3H_

#include "arduino.h"
#include "SGL.h"
#include <SPI.h>

// Teensy 3.1 can only generate 30 MHz SPI when running at 120 MHz (overclock)
// At all other speeds, SPI.beginTransaction() will use the fastest available clock
#define SPICLOCK 30000000

#define RGB_OLED_WIDTH                      96
#define RGB_OLED_HEIGHT                     64

#define CMD_DRAW_LINE                       0x21
#define CMD_DRAW_RECTANGLE                  0x22
#define CMD_COPY_WINDOW                     0x23
#define CMD_DIM_WINDOW                      0x24
#define CMD_CLEAR_WINDOW                    0x25
#define CMD_FILL_WINDOW                     0x26
    #define DISABLE_FILL    0x00
    #define ENABLE_FILL     0x01
#define CMD_CONTINUOUS_SCROLLING_SETUP      0x27
#define CMD_DEACTIVE_SCROLLING              0x2E
#define CMD_ACTIVE_SCROLLING                0x2F

#define CMD_SET_COLUMN_ADDRESS              0x15
#define CMD_SET_ROW_ADDRESS                 0x75
#define CMD_SET_CONTRAST_A                  0x81
#define CMD_SET_CONTRAST_B                  0x82
#define CMD_SET_CONTRAST_C                  0x83
#define CMD_MASTER_CURRENT_CONTROL          0x87
#define CMD_SET_PRECHARGE_SPEED_A           0x8A
#define CMD_SET_PRECHARGE_SPEED_B           0x8B
#define CMD_SET_PRECHARGE_SPEED_C           0x8C
#define CMD_SET_REMAP                       0xA0
#define CMD_SET_DISPLAY_START_LINE          0xA1
#define CMD_SET_DISPLAY_OFFSET              0xA2
#define CMD_NORMAL_DISPLAY                  0xA4
#define CMD_ENTIRE_DISPLAY_ON               0xA5
#define CMD_ENTIRE_DISPLAY_OFF              0xA6
#define CMD_INVERSE_DISPLAY                 0xA7
#define CMD_SET_MULTIPLEX_RATIO             0xA8
#define CMD_DIM_MODE_SETTING                0xAB
#define CMD_SET_MASTER_CONFIGURE            0xAD
#define CMD_DIM_MODE_DISPLAY_ON             0xAC
#define CMD_DISPLAY_OFF                     0xAE
#define CMD_NORMAL_BRIGHTNESS_DISPLAY_ON    0xAF
#define CMD_POWER_SAVE_MODE                 0xB0
#define CMD_PHASE_PERIOD_ADJUSTMENT         0xB1
#define CMD_DISPLAY_CLOCK_DIV               0xB3
#define CMD_SET_GRAY_SCALE_TABLE            0xB8
#define CMD_ENABLE_LINEAR_GRAY_SCALE_TABLE  0xB9
#define CMD_SET_PRECHARGE_VOLTAGE           0xBB
#define CMD_SET_V_VOLTAGE                   0xBE

#define RGB(R,G,B)                  (((R>>3)<<11) | ((G>>2)<<5) | (B>>3))
enum Color{
  COLOR_BLACK     = RGB(  0,  0,  0), // black
  COLOR_GREY      = RGB(192,192,192), // grey
  COLOR_WHITE     = RGB(255,255,255), // white
  COLOR_RED       = RGB(255,  0,  0), // red
  COLOR_PINK      = RGB(255,192,203), // pink
  COLOR_YELLOW    = RGB(255,255,  0), // yellow
  COLOR_GOLDEN    = RGB(255,215,  0), // golden
  COLOR_BROWN     = RGB(128, 42, 42), // brown
  COLOR_BLUE      = RGB(  0,  0,255), // blue
  COLOR_CYAN      = RGB(  0,255,255), // cyan
  COLOR_GREEN     = RGB(  0,255,  0), // green
  COLOR_PURPLE    = RGB(160, 32,240), // purple
};

enum DisplayMode{
  //reset the above effect and turn the data to ON at the corresponding gray level.
  NormalDisplay   = 0xA4,
  //forces the entire display to be at "GS63"
  DisplayOn       = 0xA5,
  //forces the entire display to be at gray level "GS0"
  DisplayOff      = 0xA6,
  //swap the gray level of display data
  InverseDisplay  = 0xA7
};

enum DisplayPower{
  DimMode         = 0xAC,
  SleepMode       = 0xAE,
  NormalMode      = 0xAF
};

enum ScollingDirection{
  Horizontal      = 0x00,
  Vertical        = 0x01,
  Diagonal        = 0x02
};

class SSD1331_t3 : public virtual SGL {
 public:
	SSD1331_t3(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13);

	void begin(void);
	void drawPixel(uint16_t x, uint16_t y, uint16_t color);

  void fillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) __attribute__((always_inline)) {
    _drawFrame(x, y, x + w - 1, y + h - 1, color, color, true);
  }

  void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

  void drawVerticalLine(uint16_t x, uint16_t y, uint16_t height,uint16_t color) __attribute__((always_inline)) {
    drawLine(x, y, x, y + height - 1, color);
  }

  void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color) __attribute__((always_inline)) {
    drawLine(x, y, x + width - 1, y, color);
  }

  void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) __attribute__((always_inline)) {
    _drawFrame(x0, y0, x1, y1, color, 0, false);
  }

  void drawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t outColor, uint16_t fillColor) __attribute__((always_inline)) {
    _drawFrame(x0, y0, x1, y1, outColor, fillColor, true);
  }
  
  /* Custom SSD1331 extensions */
  void copyWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);
  void dimWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void clearWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void setScolling(ScollingDirection direction, uint8_t rowAddr, uint8_t rowNum, uint8_t timeInterval);
  void enableScolling(bool enable);
  void setDisplayMode(DisplayMode mode);
  void setDisplayPower(DisplayPower power);

 protected:

  uint8_t  _rst, _cs, _dc, _mosi, _sclk;
	uint8_t pcs_data, pcs_command;
  elapsedMicros safeDelay;

  void _drawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t outColor, uint16_t fillColor, bool filled);

	//void waitFifoNotFull(void) __attribute__((always_inline)) {
	void waitFifoNotFull(void) {
		uint32_t sr;
		uint32_t tmp __attribute__((unused));
		do {
			sr = KINETISK_SPI0.SR;
			if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
		} while ((sr & (15 << 12)) > (3 << 12));
	}

	void waitFifoEmpty(void) {
		uint32_t sr;
		uint32_t tmp __attribute__((unused));
		do {
			sr = KINETISK_SPI0.SR;
			if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
		} while ((sr & 0xF0F0) > 0);             // wait both RX & TX empty
	}

	void waitTransmitComplete(void) __attribute__((always_inline)) {
		uint32_t tmp __attribute__((unused));
		while (!(KINETISK_SPI0.SR & SPI_SR_TCF)) ; // wait until final output done
		tmp = KINETISK_SPI0.POPR;                  // drain the final RX FIFO word
	}

	void waitTransmitComplete(uint32_t mcr) __attribute__((always_inline)) {
		uint32_t tmp __attribute__((unused));
		while (1) {
			uint32_t sr = KINETISK_SPI0.SR;
			if (sr & SPI_SR_EOQF) break;  // wait for last transmit
			if (sr &  0xF0) tmp = KINETISK_SPI0.POPR;
		}
		KINETISK_SPI0.SR = SPI_SR_EOQF;
		SPI0_MCR = mcr;
		while (KINETISK_SPI0.SR & 0xF0) {
			tmp = KINETISK_SPI0.POPR;
		}
	}

	void writecommand8_cont(uint8_t c) __attribute__((always_inline)) {
		KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}

	void writecommand16_cont(uint16_t c) __attribute__((always_inline)) {
		KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}

  void writecommand16_cont_split(uint8_t x, uint8_t y) __attribute__((always_inline)) {
    writecommand16_cont((x << 8) | y);
  }
  
	void writedata8_cont(uint8_t c) __attribute__((always_inline)) {
		KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}

	void writedata16_cont(uint16_t d) __attribute__((always_inline)) {
		KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}

	void writecommand8_last(uint8_t c) __attribute__((always_inline)) {
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}

	void writecommand16_last(uint16_t c) __attribute__((always_inline)) {
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}

  void writecommand16_last_split(uint8_t x, uint8_t y) __attribute__((always_inline)) {
    writecommand16_last((x << 8) | y);
  }

	void writedata8_last(uint8_t c) __attribute__((always_inline)) {
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}

	void writedata16_last(uint16_t d) __attribute__((always_inline)) {
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}
  
  void spi_begin(void) __attribute__((always_inline)) {
    unsigned long mask = (~(0UL) ^ (~(0UL) >> 1));
    while (safeDelay & mask) { ; }
    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  }

  void spi_end(int16_t delayNeeded) __attribute__((always_inline)) {
    SPI.endTransaction();
    safeDelay = -delayNeeded;
  }
};

#endif
