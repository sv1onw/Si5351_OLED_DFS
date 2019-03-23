// Oled.h
// V2.0 18-4-5 added UL numbers
// defines oled pins, creates "oled" object for 128x64 SH1106 display
// fonts github.com/olikraus/u8g2/wiki

// HEADERS & LIBRARIES
#include "U8g2lib.h"
#include "Wire.h"

// OBJECTS
// oled object, SH1106 controller, 128X64, HW I2C and normal orientation R0
U8G2_SH1106_128X64_NONAME_1_HW_I2C oled(U8G2_R0);

//FUNCTIONS

// BAR
// display bar at x, y, h)eight, l)ength (0-128 pixels)
void dispBar(u8g2_uint_t x, u8g2_uint_t y, byte h, byte l) {
  byte n;

  oled.drawFrame(x, y, 100, h+1);
  for ( n = 0; n < l; n++) {
    oled.drawLine(x + n, y, x  + n, y + h);
  }
}

// FREQ
// display freq at x, y, f (Hz) plus cf (cHz), d)ecimal places (max 3)
void dispFreq(u8g2_uint_t x, u8g2_uint_t y, double f, double cf, byte d) {
  oled.setFont(u8g2_font_10x20_tf); // font
  oled.setFontPosTop(); // origin top

  f = f / 1000.0;
  cf = cf / 100000.0;

  oled.setCursor(x, y);
  oled.print(f + cf, d);
  oled.print("kHz");
}

//MSG
// display small message at at x), y), *m)essage
void dispMsgS(u8g2_uint_t x, u8g2_uint_t y, char *m) {
  // sets font, cursor position and displays message
  oled.setFont(u8g2_font_5x8_tf); // font
  oled.setFontPosTop();
  oled.setCursor(x, y);
  oled.print(m);
}

// display message at at x), y), *m)essage
void dispMsg(u8g2_uint_t x, u8g2_uint_t y, char *m) {
  // sets font, cursor position and displays message
  oled.setFont(u8g2_font_7x13_tf); // font
  oled.setFontPosTop();
  oled.setCursor(x, y);
  oled.print(m);
}

// display large message at at x), y), *m)essage
void dispMsgL(u8g2_uint_t x, u8g2_uint_t y, char *m) {
  oled.setFont(u8g2_font_10x20_tf); // font
  oled.setFontPosTop();
  oled.setCursor(x, y);
  oled.print(m);
}

// display ultra large message at at x), y), *m)essage
void dispMsgUL(u8g2_uint_t x, u8g2_uint_t y, char *m) {
  oled.setFont(u8g2_font_logisoso30_tf); // font
  oled.setFontPosTop();
  oled.setCursor(x, y);
  oled.print(m);
}

// NUM
// display number at x), y), n)umber (double), d)ecimal places
void dispNum(u8g2_uint_t x, u8g2_uint_t y, double n, byte d) {
  oled.setFont(u8g_font_7x14); // fix font for now
  oled.setFontPosTop();
  oled.setCursor(x, y);
  oled.print(n, d);
}

// display number large at x), y), n)umber (double), d)ecimal places
void dispNumL(u8g2_uint_t x, u8g2_uint_t y, double n, byte d) {
  oled.setFont(u8g2_font_10x20_tf); // font
  oled.setFontPosTop();
  oled.setCursor(x, y);
  oled.print(n, d);
}

// display number ultra large at x), y), n)umber (double), d)ecimal places
void dispNumUL(u8g2_uint_t x, u8g2_uint_t y, double n, byte d) {
  oled.setFont(u8g2_font_logisoso24_tf); // font
  oled.setFontPosTop();
  oled.setCursor(x, y);
  oled.print(n, d);
}

// DATE
// display date
void dispDate(u8g2_uint_t x, u8g2_uint_t y, byte dw, byte da, byte mo, byte yr) {
  oled.setFont(u8g_font_7x14); // fix font for now
  oled.setFontPosTop();
  oled.setCursor(x, y);
  
  switch (dw) {
    case 1:
      oled.print("Mon");
      break;
    case 2:
      oled.print("Tue");
      break;
    case 3:
      oled.print("Wed");
      break;
    case 4:
      oled.print("Thu");
      break;
    case 5:
      oled.print("Fri");
      break;
    case 6:
      oled.print("Sat");
      break;
    case 7:
      oled.print("Sun");
      break;
  }

  oled.print(" ");
  oled.print(da);

  oled.print(" ");
  switch (mo)
  {
    case 1:
      oled.print("Jan");
      break;
    case 2:
      oled.print("Feb");
      break;
    case 3:
      oled.print("Mar");
      break;
    case 4:
      oled.print("Apr");
      break;
    case 5:
      oled.print("May");
      break;
    case 6:
      oled.print("Jun");
      break;
    case 7:
      oled.print("Jul");
      break;
    case 8:
      oled.print("Aug");
      break;
    case 9:
      oled.print("Sep");
      break;
    case 10:
      oled.print("Oct");
      break;
    case 11:
      oled.print("Nov");
      break;
    case 12:
      oled.print("Dec");
      break;
  }
  oled.print(" ");
  oled.print("20");
  oled.print(yr);
}

// TIME
// display time HH:MM:SS at x), y)
void dispTime(u8g2_uint_t x, u8g2_uint_t y, byte h, byte m, byte s) {
  oled.setFont(u8g_font_7x14); // fix font for now
  oled.setFontPosTop();
  oled.setCursor(x, y);
  if (h < 10)
    oled.print("0");
  oled.print(h);
  oled.print(":");
  if (m < 10)
    oled.print("0");
  oled.print(m);
  oled.print(":");
  if (s < 10)
    oled.print("0");
  oled.print(s);
}

// display time HH:MM:SS at x), y)
void dispTimeL(u8g2_uint_t x, u8g2_uint_t y, byte h, byte m, byte s) {
  oled.setFont(u8g2_font_10x20_tf); // font
  oled.setFontPosTop();
  oled.setCursor(x, y);
  if (h < 10)
    oled.print("0");
  oled.print(h);
  oled.print(":");
  if (m < 10)
    oled.print("0");
  oled.print(m);
  oled.print(":");
  if (s < 10)
    oled.print("0");
  oled.print(s);
}

// STEP
// display step at x) y) s)tep
void dispStep(u8g2_uint_t x, u8g2_uint_t y, unsigned int s) {
  oled.setFont(u8g_font_7x14); // fix font for now
  oled.setFontPosTop();
  
  oled.setCursor(x, y);
  switch (s) // display freqStep
  {
    case 10:
      oled.print(" 10Hz");
      break;
    case 100:
      oled.print("100Hz");
      break;
    case 500:
      oled.print("500Hz");
      break;
    case 1000:
      oled.print(" 1kHz");
      break;
    case 2500:
      oled.print("2.5kHz");
      break;
    case 10000:
      oled.print(" 10kHz");
      break;
    case 100000:
      oled.print("100kHz");
      break;
    case 1000000:
      oled.print(" 1MHz");
      break;
  }
}
