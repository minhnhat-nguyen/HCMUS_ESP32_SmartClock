#pragma once

enum class Brand {
  PANASONIC = 0,
  SONY = 1,
  SAMSUNG = 2,
  LG = 3,
  SHARP = 4,
};

void initIR();
void sendIR(Brand brand);
