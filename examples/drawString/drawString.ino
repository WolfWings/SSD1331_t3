/*
 draw string

 This sketch is used to test seeed's Xadow - RGB OLED, 
 it will draw a string

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

char* str = "Seeed";

void setup(){
    Serial.begin(9600);
    Serial.println("Start to draw");
    oled.begin();
    oled.fillScreen(COLOR_BLACK);
    oled.drawString(str,15,25,2,COLOR_GREEN);
    delay(2000);
    oled.setScolling(Horizontal,0,64,1);
}

void loop(){
    //nothing to do
}


