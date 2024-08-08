#include "ir.hpp"
#include "lib/IR/IRremote.h"

#define PIN_IR 25

IRsend irsend;

void initIR()
{
  irsend.begin(PIN_IR);
}

void sendIR(decode_type_t brand)
{
  analogWrite(PIN_IR, 255);
  switch (brand) {
    case SAMSUNG:
      irsend.sendSAMSUNG(0xE0E040BF, 32);
      break;
    case LG:
      irsend.sendLG(0x88, 32, 3);
      break;
    case SONY:
      irsend.sendSony(0xa90, 3, 3);
      break;
    case PANASONIC:
      irsend.sendPanasonic(0x4004, 16, 3);
      break;
    case SHARP:
      irsend.sendSharp(0x40, 16, 3);
      break;
  }
  analogWrite(PIN_IR, 0);
}
