#include "lib/IR/IRremote.hpp"
#include "lib/IR/IRremoteInt.h"
#include "ir.hpp"
#define PIN_IR 25

IRsend irsend;

void initIR()
{
  irsend.begin(PIN_IR);
}

void sendIR(Brand brand)
{
  analogWrite(PIN_IR, 255);
  switch (brand) {
    case Brand::SAMSUNG:
      irsend.sendSAMSUNG(0xE0E040BF, 32);
      break;
    case Brand::LG:
      irsend.sendLG(0x88, 32, 3);
      break;
    case Brand::SONY:
      irsend.sendSony(0xa90, 3, 3);
      break;
    case Brand::PANASONIC:
      irsend.sendPanasonic(0x4004, 16, 3);
      break;
    case Brand::SHARP:
      irsend.sendSharp(0x40, 16, 3);
      break;
  }
  analogWrite(PIN_IR, 0);
}
