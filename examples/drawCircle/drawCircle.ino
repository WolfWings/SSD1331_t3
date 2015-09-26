/*
 draw circle

 This sketch is used to test seeed's Xadow - RGB OLED, 
 it will draw a circle

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
    for(int i = 30; i > 0; i--){
        oled.drawCircle(48,32,i,COLOR_CYAN);
        delay(50);
    }
    for(int i = 1; i <= 30; i++){
        oled.drawCircle(48,32,i,COLOR_RED);
        delay(50);
    }
    for(int i = 30; i > 0; i--){
        oled.drawCircle(48,32,i,COLOR_PURPLE);
        delay(50);
    }
    for(int i = 1; i <= 30; i++){
        oled.drawCircle(48,32,i,COLOR_GOLDEN);
        delay(50);
    }
}


