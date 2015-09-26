/*
 draw traingle

 This sketch is used to test seeed's Xadow - RGB OLED, 
 it will draw a traingle

 create on 2014/06/24, version: 0.1
 by lawliet.zou(lawliet.zou@gmail.com)
*/

#include <SGL.h>
#include <SSD1331_t3.h>
#include <SPI.h>

#define sclk   13
#define mosi   11
#define oc     10
#define dc     9
#define rst    8

SSD1331_t3 oled = SSD1331_t3(oc, dc, rst, mosi, sclk);

void setup(){
    Serial.begin(9600);
    Serial.println("Start to draw");
    oled.begin();
    oled.fillScreen(COLOR_BLACK);

}

void loop(){
    //nothing to do
    uint8_t x0 = random(0,96);
    uint8_t y0 = random(0,64);
    uint8_t x1 = random(0,96);
    uint8_t y1 = random(0,64);
    uint8_t x2 = random(0,96);
    uint8_t y2 = random(0,64);
    oled.drawTraingle(x0,y0,x1,y1,x2,y2,random(0,65536));
    delay(3000);
    oled.fillScreen(COLOR_BLACK);
}


