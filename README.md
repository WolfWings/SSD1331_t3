## SSD1331_t3
This is a library for the 0.96" 16-bit Color OLED with SSD1331 driver chip, optimized for the Teensy 3.x

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/684

These displays use SPI to communicate, 5 pins are required with this library to interface due to the 'RST' pin generally needing to be pulled low for 3μs but kept high otherwise, and the separate 'command/data' pin used to allow for faster bitmap image uploads, as well as the dedicated and normal 'Chip Select/Slave Select' pin used by SPI as a whole.

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution

### Introduction
The SSD1331 is a 16 bit 96*64 dot matrix OLED display module with RGB color based on the SSD1331 module; this is a single chip CMOS OLED/PLED driver with 288 segments and 64 common output, supporting up to 96 * 64 RGB dot matrix display. It uses SPI for communication.

### Feature
+ Resolution: 96 * 64 RGB dot matrix panel
+ 65k (5:6:5) color depth supported by embedded 96*64*16 bit GDDRAM display buffer
+ Graphic Accelerating Command (GAC) set with Continuous Horizontal, Vertical and Diagonal 
Scrolling
+ Programmable Frame Rate

### Interface
***Init the RGB OLED***

    void begin(void);

***Accelerated Graphics Operations***

    void drawPixel(uint16_t x, uint16_t y, uint16_t color);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawVerticalLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawHorizontalLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t outColor, uint16_t fillColor);
    void fillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    ……    

***Hardware Specific Operations/Extensions***

    void copyWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);
    void dimWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void clearWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setScolling(ScollingDirection direction, uint8_t rowAddr, uint8_t rowNum, uint8_t timeInterval);
    void enableScolling(bool enable);
    void setDisplayMode(DisplayMode mode);
    void setDisplayPower(DisplayPower power);
    void drawCallbackPixels(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
        uint16 *callback(uint16_t x, uint16_t y, void *state), void *state);

All other functionality present the in [SGL (Seeed Graphics Library)](https://github.com/Seeed-Studio/SGL) is present and available, the above only documents functionality natively re-implemented to take advantage of the hardware acceleration present in the SSD1331 chipset.

### Getting Started
Please take the example sketches in examples folder as reference, have fun!

----
This software is based on Adafruit's SSD1331 OLED Driver library and modified by lawliet zou [wei.zou@seeedstudio.com](wei.zou@seeedstudio.com) for seeed studio. 
It is licensed under [The BSD License](http://www.freebsd.org/copyright/freebsd-license.html). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by [forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>

----
This revision of the software is extended to take full advantage of the Teensy 3.x SPIFIFO functionality, modified by Wolf (wolfwings@gmail.com).
It is still licensed under [The BSD License](http://www.freebsd.org/copyright/freebsd-license.html). Check License.txt for more information.<br>
