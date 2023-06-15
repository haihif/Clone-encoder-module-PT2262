#include"pt2262.h"

uint8_t DATA_STATE = 0x00; 

void setup() {
  Serial.begin(115200);
  Serial.println("PT2262 Remote ");

}

int loop() {
  getState(&DATA_STATE);
  sendFrame(0x00, DATA_STATE);
}