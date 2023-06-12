/*
 *  pt2262.c
 *
 * 
 * 
 * 
 * 
 *  author: Nguyen Huy Hai /haihif 
*/

#include "pt2262.h"
#include <stdint.h>
#include <Arduino.h>

/*--------------------------------------------DEFINE------------------------------------------------*/
#define VERSION "0.1"

#define POWER_ON_PIN                  CONFIG_POWER_ON_PIN
#define LED_PIN                       CONFIG_LED_PIN

/*
 * The output signal pin connected 315MHz modulation input pin 
 */
#define DATA_OUT_PIN                  CONFIG_DATA_OUT_PIN

/*
 * Using for PT2262/2272 with 8 address bits and 4 data bits
 */
#define DATA_INPUT_PIN_A              CONFIG_DATA_INPUT_PIN_A
#define DATA_INPUT_PIN_B              CONFIG_DATA_INPUT_PIN_B
#define DATA_INPUT_PIN_C              CONFIG_DATA_INPUT_PIN_C
#define DATA_INPUT_PIN_D              CONFIG_DATA_INPUT_PIN_D

/*
 * A7->A6->A5->A4->A3->A2->A1->A0
 * Example: Bin 0b10011001, Hex 0x99    
 */
#define ADDRESS_RECEIVER_BITS         CONFIG_ADDRESS_RECEIVER_BITS

/*
 * D0->D1->D2->D3
 */
#define DATA_STATE_A                  (0x08)
#define DATA_STATE_B                  (0x04)
#define DATA_STATE_C                  (0x02)
#define DATA_STATE_D                  (0x01)
#define DATA_STATE_OFF                (0x00)
/*
 * Following datasheet, in this situation, my receiver has a resistor 680KOhm between OSC pins,
 * suggested oscillator resistor values of the sender is 3.3MOhm and the encode osc frequency is 10kHz
 * So that, the clock period should be 100 us
 */
#define PT2262_OSC_CLOCK_PERIOD       (100) 
#define PT2262_CLK_PERIOD_4           (PT2262_OSC_CLOCK_PERIOD*4)
#define PT2262_CLK_PERIOD_12          (PT2262_OSC_CLOCK_PERIOD*12)
#define PT2262_CLK_PERIOD_128         (PT2262_OSC_CLOCK_PERIOD*128)

/*
 * PT2262/2272 has 4 types of bit
 */
#define PT2262_BIT_0                  (0)
#define PT2262_BIT_1                  (1)
#define PT2262_BIT_FLOAT              (2)
#define PT2262_BIT_SYNC               (3)

#ifdef USING_DIGITAL_WRITE_FAST
#include "digitalWriteFast.h"
#else
#define digitalWriteFast(pin, level) digitalWrite(pin, level)
#endif


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(DATA_OUT_PIN, OUTPUT);
}

int loop() {
  
}

/*
 * A bit can be designated as Bit “0”, “1” or “f” if it is in low, high or floating 
 *state respectively. One bit waveform consists of 2 pulse cycles. Each pulse cycle
 *has 16 oscillating time periods. For further details, please refer to the diagram:    
 * @example
 *        _   _   _   _   _                          _   _   _                        _   _
 * OSC   |a|_| |_| |_| |_| |_...                 ...| |_| |_| |_...              ..._| |_| |_
 *       <--------------------------------- 32 Clock Period -------------------------------->
 *        _____________                              ______________
 * Bit 0:|     4a      |____________________________|      4a      |_________________________
 *        ____________________                       _______________________
 * Bit 1:|         12a        |_____________________|          12a          |________________
 *        _____________                              _______________________
 * Bit f:|     4a      |____________________________|          12a          |________________
 * 
 *       <------------------ 16a ------------------><------------------- 16a ---------------->
 * 
 * Sync Bit is 4 bits long with 1/8 bit width first is HIGH-level.
 *        _____________
 *       |     4a      |______________________________________________________________________
 *       <------------------------------------- 128a ---------------------------------------->
 * 
 * a = 1 Oscillating Clock Period
 * 
 * @param 
 * PT2262_BIT: "0", "1", "FLOAT" or "SYNC"
 * 
 */
void sendBit(uint8_t PT2262_BIT){
    switch (PT2262_BIT) {
        case PT2262_BIT_0: // Bit 0: 4 HIGH -> 12 LOW -> 4 HIGH -> 12 LOW
            digitalWriteFast(DATA_OUT_PIN, 1);
            delayMicroseconds(PT2262_CLK_PERIOD_4);
            digitalWriteFast(DATA_OUT_PIN, 0);
            delayMicroseconds(PT2262_CLK_PERIOD_12);
            digitalWriteFast(DATA_OUT_PIN, 1);
            delayMicroseconds(PT2262_CLK_PERIOD_4);
            digitalWriteFast(DATA_OUT_PIN, 0);
            delayMicroseconds(PT2262_CLK_PERIOD_12);
            break;
        case PT2262_BIT_1: // Bit 0: 12 HIGH -> 4 LOW -> 12 HIGH -> 4 LOW
            digitalWriteFast(DATA_OUT_PIN, 1);
            delayMicroseconds(PT2262_CLK_PERIOD_12);
            digitalWriteFast(DATA_OUT_PIN, 0);
            delayMicroseconds(PT2262_CLK_PERIOD_4);
            digitalWriteFast(DATA_OUT_PIN, 1);
            delayMicroseconds(PT2262_CLK_PERIOD_12);
            digitalWriteFast(DATA_OUT_PIN, 0);
            delayMicroseconds(PT2262_CLK_PERIOD_4);
            break;
        case PT2262_BIT_FLOAT: // Bit 0: 4 HIGH -> 12 LOW -> 12 HIGH -> 4 LOW
            digitalWriteFast(DATA_OUT_PIN, 1);
            delayMicroseconds(PT2262_CLK_PERIOD_4);
            digitalWriteFast(DATA_OUT_PIN, 0);
            delayMicroseconds(PT2262_CLK_PERIOD_12);
            digitalWriteFast(DATA_OUT_PIN, 1);
            delayMicroseconds(PT2262_CLK_PERIOD_12);
            digitalWriteFast(DATA_OUT_PIN, 0);
            delayMicroseconds(PT2262_CLK_PERIOD_4);
            break;
        case PT2262_BIT_SYNC: // Bit 0: 4 HIGH -> 128 LOW 
            digitalWriteFast(DATA_OUT_PIN, 1);
            delayMicroseconds(PT2262_CLK_PERIOD_4);
            digitalWriteFast(DATA_OUT_PIN, 0);
            delayMicroseconds(PT2262_CLK_PERIOD_128);
            break;
        default:
            digitalWriteFast(DATA_OUT_PIN, 0);
        break;
    }
}

/*
 * A group of Code Bits is called a Code Word. A Code Frame consists of 4 Code Words. 
 * A Code Word consists of 12 bits(8 ADD bits & 4 Data bits) followed by one Sync Bit.
 * @param 
 *  ADDRESS_RECEIVER_BITS: 8 address bits between the sender and the receiver
 * @param  
 * dataCode:  4 data bits   
 */
void sendFrame(uint8_t ADDRESS_RECEIVER_BITS, uint8_t dataCode){
    // 4 Code Word
    for(uint8_t i=0; i<4; i++){
        // 8 Address Bits
        for(uint8_t a=0; a<8; a++){
            uint8_t bitValue = ADDRESS_RECEIVER_BITS & 0x01;
            ADDRESS_RECEIVER_BITS = ADDRESS_RECEIVER_BITS >> 1;
            sendBit(bitValue);
        }
        // 4 Data Bits
        for(uint8_t d=0; d<4; d++){
            uint8_t bitValue = dataCode & 0x01;
            dataCode = dataCode >> 1;
            sendBit(bitValue);
        }
        // Sync bit
        sendBit(PT2262_BIT_SYNC);
    }
}

uint8_t getState(){
    if (digitalRead(DATA_INPUT_PIN_A) == 1)
        return DATA_STATE_A;
    else if (digitalRead(DATA_INPUT_PIN_B) == 1)
        return DATA_STATE_B;
    else if (digitalRead(DATA_INPUT_PIN_C) == 1)
        return DATA_STATE_C;
    else if (digitalRead(DATA_INPUT_PIN_D) == 1)
        return DATA_STATE_D;
    else 
        return DATA_STATE_OFF;
}

