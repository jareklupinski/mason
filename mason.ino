#include <Wire.h>
#include <DS1307new.h>
#include <digitalWriteFast.h>
#include <SPI.h>

const int latch = 9;
int milliTimer = 0;
int milliHolder = 0;
int secHolder = 0;

uint16_t startAddr = 0x0000;            // Start address to store in the NV-RAM
uint16_t lastAddr;                      // new address for storing in NV-RAM
uint16_t TimeIsSet = 0xaa55;            // Helper that time must not set again

void writeLeftDigit(int n)
{
  //This number from : this combo
  //0: HLLH , 9
  //1: HLLL , 8
  //2: HHLL , 12
  //3: HHLH , 13
  //4: LHLH , 5
  //5: LHLL , 4
  //6: LLHH , 3
  //7: LLHL , 2
  //8: LLLL , 0
  //9: LLLH , 1
  switch(n)
  {
     case 0:
       digitalWriteFast(14, HIGH);
       digitalWriteFast(15, LOW);
       digitalWriteFast(16, LOW);
       digitalWriteFast(17, HIGH);
       break;
     case 1:
       digitalWriteFast(14, HIGH);
       digitalWriteFast(15, LOW);
       digitalWriteFast(16, LOW);
       digitalWriteFast(17, LOW);
       break;
     case 2:
       digitalWriteFast(14, HIGH);
       digitalWriteFast(15, HIGH);
       digitalWriteFast(16, LOW);
       digitalWriteFast(17, LOW);
       break;
     case 3:
       digitalWriteFast(14, HIGH);
       digitalWriteFast(15, HIGH);
       digitalWriteFast(16, LOW);
       digitalWriteFast(17, HIGH);
       break;
     case 4:
       digitalWriteFast(14, LOW);
       digitalWriteFast(15, HIGH);
       digitalWriteFast(16, LOW);
       digitalWriteFast(17, HIGH);
       break;
     case 5:
       digitalWriteFast(14, LOW);
       digitalWriteFast(15, HIGH);
       digitalWriteFast(16, LOW);
       digitalWriteFast(17, LOW);
       break;
     case 6:
       digitalWriteFast(14, LOW);
       digitalWriteFast(15, LOW);
       digitalWriteFast(16, HIGH);
       digitalWriteFast(17, HIGH);
       break;
     case 7:
       digitalWriteFast(14, LOW);
       digitalWriteFast(15, LOW);
       digitalWriteFast(16, HIGH);
       digitalWriteFast(17, LOW);
       break;
     case 8:
       digitalWriteFast(14, LOW);
       digitalWriteFast(15, LOW);
       digitalWriteFast(16, LOW);
       digitalWriteFast(17, LOW);
       break;
     case 9:
       digitalWriteFast(14, LOW);
       digitalWriteFast(15, LOW);
       digitalWriteFast(16, LOW);
       digitalWriteFast(17, HIGH);
       break;
  }
}

void writeMiddleDigit(int n)
{
  switch(n)
  {
     case 0:
       digitalWriteFast(0, HIGH);
       digitalWriteFast(1, LOW);
       digitalWriteFast(2, LOW);
       digitalWriteFast(3, HIGH);
       break;
     case 1:
       digitalWriteFast(0, HIGH);
       digitalWriteFast(1, LOW);
       digitalWriteFast(2, LOW);
       digitalWriteFast(3, LOW);
       break;
     case 2:
       digitalWriteFast(0, HIGH);
       digitalWriteFast(1, HIGH);
       digitalWriteFast(2, LOW);
       digitalWriteFast(3, LOW);
       break;
     case 3:
       digitalWriteFast(0, HIGH);
       digitalWriteFast(1, HIGH);
       digitalWriteFast(2, LOW);
       digitalWriteFast(3, HIGH);
       break;
     case 4:
       digitalWriteFast(0, LOW);
       digitalWriteFast(1, HIGH);
       digitalWriteFast(2, LOW);
       digitalWriteFast(3, HIGH);
       break;
     case 5:
       digitalWriteFast(0, LOW);
       digitalWriteFast(1, HIGH);
       digitalWriteFast(2, LOW);
       digitalWriteFast(3, LOW);
       break;
     case 6:
       digitalWriteFast(0, LOW);
       digitalWriteFast(1, LOW);
       digitalWriteFast(2, HIGH);
       digitalWriteFast(3, HIGH);
       break;
     case 7:
       digitalWriteFast(0, LOW);
       digitalWriteFast(1, LOW);
       digitalWriteFast(2, HIGH);
       digitalWriteFast(3, LOW);
       break;
     case 8:
       digitalWriteFast(0, LOW);
       digitalWriteFast(1, LOW);
       digitalWriteFast(2, LOW);
       digitalWriteFast(3, LOW);
       break;
     case 9:
       digitalWriteFast(0, LOW);
       digitalWriteFast(1, LOW);
       digitalWriteFast(2, LOW);
       digitalWriteFast(3, HIGH);
       break;
  }
}

void writeRightDigit(int n)
{
  switch(n)
  {
     case 0:
       digitalWriteFast(4, HIGH);
       digitalWriteFast(5, LOW);
       digitalWriteFast(6, LOW);
       digitalWriteFast(7, HIGH);
       break;
     case 1:
       digitalWriteFast(4, HIGH);
       digitalWriteFast(5, LOW);
       digitalWriteFast(6, LOW);
       digitalWriteFast(7, LOW);
       break;
     case 2:
       digitalWriteFast(4, HIGH);
       digitalWriteFast(5, HIGH);
       digitalWriteFast(6, LOW);
       digitalWriteFast(7, LOW);
       break;
     case 3:
       digitalWriteFast(4, HIGH);
       digitalWriteFast(5, HIGH);
       digitalWriteFast(6, LOW);
       digitalWriteFast(7, HIGH);
       break;
     case 4:
       digitalWriteFast(4, LOW);
       digitalWriteFast(5, HIGH);
       digitalWriteFast(6, LOW);
       digitalWriteFast(7, HIGH);
       break;
     case 5:
       digitalWriteFast(4, LOW);
       digitalWriteFast(5, HIGH);
       digitalWriteFast(6, LOW);
       digitalWriteFast(7, LOW);
       break;
     case 6:
       digitalWriteFast(4, LOW);
       digitalWriteFast(5, LOW);
       digitalWriteFast(6, HIGH);
       digitalWriteFast(7, HIGH);
       break;
     case 7:
       digitalWriteFast(4, LOW);
       digitalWriteFast(5, LOW);
       digitalWriteFast(6, HIGH);
       digitalWriteFast(7, LOW);
       break;
     case 8:
       digitalWriteFast(4, LOW);
       digitalWriteFast(5, LOW);
       digitalWriteFast(6, LOW);
       digitalWriteFast(7, LOW);
       break;
     case 9:
       digitalWriteFast(4, LOW);
       digitalWriteFast(5, LOW);
       digitalWriteFast(6, LOW);
       digitalWriteFast(7, HIGH);
       break;
  }
}

void sendAddrData(int n, int m){
  //0: 8
  //1: 1
  //2: 5
  //3: 2
  //4: 7
  //5: xx
  //6: xx
  //7: xx
  //8: 9
  //9: 0
  //A: 4
  //B: 3
  //C: 6
  //D: xx
  //E: xx
  //F: xx
  //This hex sends : this number
  byte data;
  switch(n)
  {
     case 0:
       data = 0x09;
       break;
     case 1:
       data = 0x01;
       break;
     case 2:
       data = 0x03;
       break;
     case 3:
       data = 0x0B;
       break;
     case 4:
       data = 0x0A;
       break;
     case 5:
       data = 0x02;
       break;
     case 6:
       data = 0x0C;
       break;
     case 7:
       data = 0x04;
       break;
     case 8:
       data = 0x00;
       break;
     case 9:
       data = 0x08;
       break;
  }
    switch(m)
  {
     case 0:
       data += 0x90;
       break;
     case 1:
       data += 0x10;
       break;
     case 2:
       data += 0x30;
       break;
     case 3:
       data += 0xB0;
       break;
     case 4:
       data += 0xA0;
       break;
     case 5:
       data += 0x20;
       break;
     case 6:
       data += 0xC0;
       break;
     case 7:
       data += 0x40;
       break;
     case 8:
       data += 0x00;
       break;
     case 9:
       data += 0x80;
       break;
  }
  SPI.transfer(data);
}

void setup() {
  SPI.begin();
  randomSeed(analogRead(0));
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  
  RTC.setRAM(0, (uint8_t *)&startAddr, sizeof(uint16_t));// Store startAddr in NV-RAM address 0x08 

/*
   Uncomment the next 2 lines if you want to SET the clock
   Comment them out if the clock is set.
   DON'T ASK ME WHY: YOU MUST UPLOAD THE CODE TWICE TO LET HIM WORK
   AFTER SETTING THE CLOCK ONCE.
*/
//  TimeIsSet = 0xffff;
//  RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));  

/*
  Control the clock.
  Clock will only be set if NV-RAM Address does not contain 0xaa.
  DS1307 should have a battery backup.
*/
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
  if (TimeIsSet != 0xaa55)
  {
    RTC.stopClock();
    RTC.second = 0;                       // DON'T USE '00' IF YOU MEAN '0' SECONDS!!!
    RTC.minute = 0;
    RTC.hour = 19;
    RTC.dow = 7;                          // SUN=7, MON=1, TUE=2, WED=3, THU=4, FRI=5, SAT=6
    RTC.date = 3;
    RTC.month = 4;
    RTC.year = 2011;
    RTC.setTime();
    RTC.startClock();
    TimeIsSet = 0xaa55;
    RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
  }
  RTC.ctrl = 0x00;                      // 0x00=disable SQW pin, 0x10=1Hz,
                                        // 0x11=4096Hz, 0x12=8192Hz, 0x13=32768Hz
  RTC.setCTRL();
}

void loop()
{ 
  RTC.getTime();
  milliTimer -= (millis() - milliHolder);
  milliHolder = millis();
  if (RTC.second != secHolder)
  {
    secHolder = RTC.second;
    milliTimer = 1000;
  }
  
  int hourInv = 23 - RTC.hour;
  int minInv = 59 - RTC.minute;
  int secInv = 59 - RTC.second;
  digitalWriteFast(latch, LOW);
  

  sendAddrData((RTC.hour / 10), (RTC.hour % 10));
  sendAddrData((RTC.minute / 10), (RTC.minute % 10));
  sendAddrData((RTC.second / 10), (RTC.second % 10));
  
  writeLeftDigit(9 - (milliTimer%1000) / 100);
  writeMiddleDigit(9 - (milliTimer%100) / 10);  
  writeRightDigit((milliTimer%1000) / 100);
  
  sendAddrData((secInv / 10), (secInv % 10));
  sendAddrData((minInv / 10), (minInv % 10));
  sendAddrData((hourInv / 10), (hourInv % 10));
  
  digitalWriteFast(latch, HIGH);  

}

