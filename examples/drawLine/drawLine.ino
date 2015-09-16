/*
 draw rectangle 

 This sketch is used to test seeed's Xadow - RGB OLED, 
 it will draw severial line with different color 

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
    oled.drawLine(5,7,65,48,COLOR_BLUE);
    delay(1000);
    oled.drawLine(5,40,48,7,COLOR_RED);
    delay(1000);
    oled.drawLine(65,5,47,50,COLOR_GREEN);
    delay(1000);
    oled.drawLine(3,10,80,21,COLOR_YELLOW);
}

void loop(){
    //nothing to do
}


