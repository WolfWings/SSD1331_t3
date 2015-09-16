/*
 draw traingle

 This sketch is used to test seeed's Xadow - RGB OLED, 
 it will draw a traingle

 create on 2014/06/24, version: 0.1
 by lawliet.zou(lawliet.zou@gmail.com)
*/

#include <SGL.h>
#include <SSD1331.h>
#include <SPI.h>

#define cs     A5
#define dc     3
#define mosi   16
#define sclk   15

SSD1331 oled = SSD1331(cs, dc, mosi, sclk);  

void setup(){
    Serial.begin(9600);
    Serial.println("Start to draw");
    oled.init();
    oled.fillScreen(COLOR_BLACK);
    delay(2000);

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


