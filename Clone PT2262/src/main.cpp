#include <Arduino.h>
#include "pt2262.h"
#include <stdio.h>

st_pt2262 pPT2262;

void setup() {
  pt2262Init(&pPT2262, 12, 7, 3, 4, 5, 6);
  //Serial.begin(9600);
}

void loop() {
    getState(&pPT2262);
    if (pPT2262.DATA_STATE != 0)
        sendFrame(&pPT2262, pPT2262.DATA_STATE);
    pPT2262.DATA_STATE = 0;
}