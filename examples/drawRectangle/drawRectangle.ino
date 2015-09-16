/*
 draw rectangle 

 This sketch is used to test seeed's Xadow - RGB OLED, 
 it will draw a rectangle with blue color 

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
    for(int i = 1; i < 16; i++){
        oled.drawRectangle(3*i,2*i,95-6*i,63-4*i,COLOR_YELLOW);
        delay(50);
    }
    delay(2000);
    for(int i = 15; i > 0; i--){
        oled.drawRectangle(3*i,2*i,95-6*i,63-4*i,COLOR_BLUE);
        delay(50);
    }    
    delay(2000);
}


