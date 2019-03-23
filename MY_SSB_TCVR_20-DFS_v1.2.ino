// 20m SSB_MONOBAND_TCVR  DFS  (Digital Frequency Synthesizer, aka PLL) with Ladder filter @ 3.276800 mhZ
// Initial Code written by Antony Watts, M0IFA.
// Adaptation by SV1ONW to suit specific needs, frequency limits.
// SSB Filter center  frequency: 3.276800 MHz, Bandwidth 2.7 KHz
// Band freq 14.000000 -14.35000 MHz has DFS frequency = 10.723200 - 11.073200 MHz (L.O.)
// USB 3.276800 - 0.001350 = 3.2738 and LSB 3.276800 + 0.001350 = 3.275450
// CLK0 = L.O. DFS, CLK1 = USB/LSB Frequencies, CLK2 = Calibration Test Frequency 8.000000 MHz
  
#include "Oled.h"                                          // include header with predefined functions for U8g2 library (Library must be installed!)
#include <si5351.h>                                        // include library
#include "Rotary.h"                                        // include library

#define DT 2                                               // encoder connnections
#define CLK 3                                              // reverse pins 2 & 3 if wrong direction on rotary encoder!
#define SW 5                                             // the Digital pin where the Rotary Encoder's push button switch is connected
#define HOLD 500                                           // button hold time, used for button's second function, if it is pressed for  > 0.5sec
// Frequencies are written as xxYYYYYY00, where xx= Megacycles, YYYYYY= rest of frequency followed by two zeros. e,g, 3.2768 is written 327680000
#define CALIBRATION -5763                                  // frequency CALIBRATION, can be + or - , in my module is -57.63 Hz. Put your value here!

#define FREQMIN   1400000000                               // tune range 14.0-14.350 MHz , down limit
#define FREQMAX   1435000000                               // upper limit of 20 meter Band
#define FREQSSBFIL 327680000                               // SSB filter center frequency
#define FILTER_BW   270000                                 // filter width, here 2700 Hz

Si5351 dfs;                                                // create dfs object
Rotary enc = Rotary(CLK, DT);                              // create encoder object

uint64_t freq = 1420000000;                                // initial frequency cHz, where DFS starts when powered on!
uint64_t freqStep = 100000;                                // initial step cHz (1kHz), when powered on
uint64_t testFreq = 800000000;                             // test Frequency = 8.000000 MHz for Calibration purposes 

bool usb;                                                  // USB/LSB flag

void setup() {
  pinMode(CLK, INPUT_PULLUP);                              // encoder input, with pull-up
  pinMode(DT, INPUT_PULLUP);                               // encoder input, with pull-up
  pinMode(SW, INPUT_PULLUP);                               // button  input, with pull-up

  oled.begin();                                            // begin OLED display

  dfs.init(SI5351_CRYSTAL_LOAD_8PF, 27000000, CALIBRATION);// set xtal capacitance, CRYSTAL FREQUENCY = 27MHz. CALIBRATION value is defined above.
                                                           // initial xtal frequency of 27MHz is defined in "si5351.h" header file! 
                                                           // crystal load is 8pF for SV1AFN's Si5351a board!
  dfs.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);       // output drive 2 or 4 or 6 or 8ma. 8ma is aprox. +10dBm! 
  dfs.drive_strength(SI5351_CLK1, SI5351_DRIVE_2MA);       // output drive 2 or 4 or 6 or 8ma. 8ma is aprox. +10dBm!
  dfs.drive_strength(SI5351_CLK2, SI5351_DRIVE_4MA);       // output drive 2 or 4 or 6 or 8ma. 8ma is aprox. +10dBm!


  dfs.output_enable(SI5351_CLK0, 1);                       // enable CLK0 
  dfs.output_enable(SI5351_CLK1, 1);                       // enable CLK1
  dfs.output_enable(SI5351_CLK2, 1);                       // enable CLK2

  freqSet(freq - FREQSSBFIL, 0);                           // output frequency CLK0, 14.200000 MHz -3.276800 MHz = 10.923200 MHz to Local Oscillator
  freqSet(FREQSSBFIL - FILTER_BW/2,  1);                   // output frequency CLK1, 3.273800 MHz - 0.001350 MHz = 3.272450 MHz USB Freq for BFO/Carrier Osc.
  freqSet(testFreq, 2);                                    // output frequency CLK2, 8.000000 MHz Test Frequency for manual Calibration, you must enable it first (see below)!
  
  usb = true;                                              // start in USB. This is 20 meter Band.

  dispUpdate();                                            // display
}

// LOOP
void loop() {
  long hold;                                               // button hold time
  unsigned char result;                                    // encoder result

  if (digitalRead(SW) == LOW) {                            // enc button push
    hold = millis();                                       // start hold count
    while (!digitalRead(SW));                              // wait release
    if (millis() - hold > HOLD) {                          // button hold > HOLD time
      usb = !usb;                                          // flag for display "LSB" or "USB"
      if (usb)                                             // USB
        freqSet(FREQSSBFIL - FILTER_BW/2, 1);
      else                                                 // LSB
        freqSet(FREQSSBFIL + FILTER_BW/2, 1);
      dispUpdate();
    }
    else if (freqStep == 1000) freqStep = 1000000;         // update step i.e. 1000000 = 10kHz
    else freqStep = freqStep / 10;                         // step down
    dispUpdate();                                          // display
  }

  result = enc.process();                                  // read encoder
  if (result == DIR_CW && freq < FREQMAX) {                // freq up
    freq += freqStep;
    freqSet(freq - FREQSSBFIL, 0);                         // update 0/freq 
    dispUpdate();
  }
  if (result == DIR_CCW && freq > FREQMIN) {              // freq down
    freq -= freqStep;
    freqSet(freq - FREQSSBFIL, 0);                        // update 0/freq
    dispUpdate();
  }
}

void freqSet(uint64_t f, byte n) {                        // output frequency, 0/DFS actual freq, 1/usb_lsb freq,  2/test Frequency
  switch (n) {
    case 0:
      dfs.set_freq(f, SI5351_CLK0);                       // DFS VFO  actual 10.7232 - 11.0732 MHz
    case 1:
      dfs.set_freq(f, SI5351_CLK1);                       // usb_lsb freq
    case 2:
      dfs.set_freq(f, SI5351_CLK2);                       // test freq
  }
}

void dispUpdate() {                                       // OLED loop
  oled.firstPage();
  do {
    dispMsg(25, 0, "20m SSB-DFS");                       // display title
    dispFreq(15, 25, freq / 100, 0, 2);                  // display frequency, in kHz
    if (usb)                                             // display LSB/USB
      dispMsg(15, 50, "USB");
    else
      dispMsg(15, 50, "LSB");
    dispStep(80, 50, freqStep / 100);                    // display step
  } while ( oled.nextPage() );
}
