/*
 draw string

 This sketch is used to test seeed's Xadow - RGB OLED, 
 it will draw a string

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

char* str = "Seeed";

void setup(){
    Serial.begin(9600);
    Serial.println("Start to draw");
    oled.init();
    oled.fillScreen(COLOR_BLACK);
    delay(2000);
    oled.drawString(str,15,25,2,COLOR_GREEN);
    delay(2000);
    oled.setScolling(Horizontal,0,64,1);
}

void loop(){
    //nothing to do
}


