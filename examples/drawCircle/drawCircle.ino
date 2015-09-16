/*
 draw circle

 This sketch is used to test seeed's Xadow - RGB OLED, 
 it will draw a circle

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


