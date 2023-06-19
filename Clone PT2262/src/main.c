#include <Arduino.h>
#include "pt2262.h"

st_pt2262 pPT2262;

void setup() {
  pt2262Init(&pPT2262, 12, 10, 2, 3, 4, 5);
}

void loop() {
    int i=0;
    while (i<16){
      sendFrame(&pPT2262, i);
      delay(1000);
      i++;
    }
}