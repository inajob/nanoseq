/**
nanoseq

Copyright (c) 2016 @ina_ani

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include "Arduboy.h"

Arduboy arduboy;
#define M_TITLE 0
#define M_GAME 1

#define XSIZE 2
#define YSIZE 2

byte mode = 0;
byte scoreMap[WIDTH/XSIZE];

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.display();
  for(int i = 0; i < WIDTH/XSIZE; i ++){
    scoreMap[i] = 0xff;
  }
}


void title(){
  arduboy.clear();
  arduboy.setCursor(0,0);
  arduboy.print("nanoseq");
  arduboy.setCursor(50, 50);
  arduboy.print("by @ina_ani");
  arduboy.display();

  if(arduboy.pressed(A_BUTTON)){
    mode = M_GAME;
  }
}
const unsigned int PROGMEM _midi_word_note_frequencies[80] = {
262,277,294,311,330,349,370,392,415,440,466,494,523,554,587,622,659,
698,740,784,831,880,932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,
1865,1976,2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,4186,
4435,4699,4978,5274,5588,5920,6272,6645,7040,7459,7902,8372,8870,9397,9956,
10548,11175,11840,12544,13290,14080,14917,15804,16744,17740,18795,19912,21096,
22351,23680,25088 };

const byte PROGMEM space[] = {
 0xff,
  0, 2, 4, 5, 7, 9,11,
 12,14,16,17,19,21,23,
 24,26,28,29,31,33,35,
 36,38,40,41,43,45,47,
 48,50,52,53,55,57,59,
 60,62,64,65,67,69,71,
 72
};
#define SHARP(x) x | 0x80
const byte PROGMEM revSpace[] = {
  0, SHARP(0),
  1, SHARP(1), 
  2, 
  3, SHARP(3), 
  4, SHARP(4), 
  5, SHARP(5), 
  6
};

int x, y;
int counter = 0;
int keyTrigger = 0;
int upTrigger = 0;
int downTrigger = 0;
int leftTrigger = 0;
int rightTrigger = 0;

void setTone(byte diff){
  if(arduboy.pressed(A_BUTTON)){
      scoreMap[x] = pgm_read_byte(space + y) + diff;
  }
}

void game(){
  arduboy.clear();

  // show cursor
  arduboy.fillRect(x * XSIZE, (HEIGHT/YSIZE - y - 1) * YSIZE, XSIZE, YSIZE, (counter & 0b10)?1:0);
  
  // show grid
  for(int j = 0; j < HEIGHT/YSIZE; j += 7){
    for(int i = 0; i < WIDTH/XSIZE; i += 2){
      arduboy.drawPixel(i * XSIZE, (HEIGHT/YSIZE - j - 1) * YSIZE, 1);
    }
  }

  // show vertical line
  for(int i = 0; i < WIDTH/(XSIZE * 8); i ++){
      arduboy.drawLine(i * XSIZE * 8, 0, i * XSIZE * 8, HEIGHT, 1);
  }

  // show score
  for(int i = 0; i < WIDTH/XSIZE; i ++){
    byte ypos;
    byte rev;
    bool sharp = false;
    if(scoreMap[i] == 0xff){
      ypos = 0;
    }else{
      rev = pgm_read_byte(revSpace + ((scoreMap[i]) % 12));
      sharp = ((rev & 0x80) == 0x80);
      ypos = 7 * ((scoreMap[i]) / 12) +  (rev & 0x7f) + 1;
    }
    if(sharp){
      arduboy.drawLine(i * XSIZE, (HEIGHT/YSIZE - ypos - 1) * YSIZE - 2, i * XSIZE + XSIZE - 1, (HEIGHT/YSIZE - ypos - 1) * YSIZE - 2, 1);
    }
    arduboy.fillRect(i * XSIZE, (HEIGHT/YSIZE - ypos - 1) * YSIZE, XSIZE, YSIZE, 1);
  }
  
  // play tone
  if(scoreMap[x] != 0xff){
    unsigned int freq = pgm_read_word(_midi_word_note_frequencies + (scoreMap[x]));
    arduboy.tunes.tone(freq, -1);
  }else{
    arduboy.tunes.stopScore();
  }

  // button event
  if(arduboy.pressed(A_BUTTON)){
    keyTrigger ++;
  }else{
    keyTrigger = 0;
  }
  if(keyTrigger == 1){
      setTone(0);
  }
    
  if(arduboy.pressed(LEFT_BUTTON)){
    leftTrigger ++;
    if(leftTrigger < 2 || (leftTrigger > 5 && leftTrigger % 5 == 0)){
      x -= 1;
      if(x < 0){ x = 0;}
      setTone(0);
    }
  }else{
    leftTrigger = 0;
  }
  if(arduboy.pressed(RIGHT_BUTTON)){
    rightTrigger ++;
    if(rightTrigger < 2 || (rightTrigger > 5 && rightTrigger % 5 == 0)){
      x += 1;
      if(x >= WIDTH/XSIZE){ x = WIDTH/XSIZE - 1;}
      setTone(0);
    }
  }else{
    rightTrigger = 0;
  }
  if(arduboy.pressed(DOWN_BUTTON)){
    downTrigger ++;
    if(downTrigger < 2 || (downTrigger > 5 && downTrigger % 5 == 0)){
      setTone(-1);
      if(keyTrigger == 0){
        y -= 1;
        if(y < 0){ y = 0;}
      }
    }
  }else{
    downTrigger = 0;
  }
  if(arduboy.pressed(UP_BUTTON)){
    upTrigger ++;
    if(upTrigger < 2 || (upTrigger > 5 && upTrigger % 5 == 0)){
      setTone(1);
      if(keyTrigger == 0){
        y += 1;
        if(y >= HEIGHT/YSIZE){ y = HEIGHT/YSIZE - 1;}
      }
    }
  }else{
    upTrigger = 0;
  }
  
  counter ++;
  arduboy.display();  
}


void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;
  
  switch(mode){
    case M_TITLE:
      title();
    break;
    case M_GAME:
      game();
    break;
  }
}
