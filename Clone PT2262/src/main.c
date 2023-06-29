#include <Arduino.h>
#include "pt2262.h"
#include <stdio.h>
st_pt2262 pPT2262;

void setup() {
  pt2262Init(&pPT2262, 12, 2, 6, 3, 4, 5);
}

void loop() {
  sendFrame(&pPT2262, 0b00000001);
  delay(300);
  sendFrame(&pPT2262, 0b00000010);
  delay(300);
  sendFrame(&pPT2262, 0b00000100);
  delay(300);
  sendFrame(&pPT2262, 0b00001000);
  delay(300);
}