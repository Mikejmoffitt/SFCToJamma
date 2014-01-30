/*
  JammaSFC
  Michael Moffitt
  http://mikejmoffitt.com
  
// Uncomment this line for single-player use with a regular Teensy 2.0

#define TEENSY_PLUS
  
  == WHAT IS IT ==
  Created to facilitate interaction between a Super Famicom / SNES controller and JAMMA
  arcade boards. Can be used for Atari 2600/7800/800, Master System, other arcade PCBs, 
  and pad-hacks of practically any other system. The system is easy to solder to a JAMMA 
  slot, a cable for the above-mentioned systems, and a Sega Genesis with a tiny bit of
  additional logic.
  
  == WHICH PADS CAN I USE ON IT ==
  An original Super Famicom or Super Nintendo controller is the target use case. I have
  tested with a knockoff that worked equally well. 
  
  The SNES Mouse and other weird attachments have not been explicitly supported. I don't 
  know why anyone would expect this, nor what those would do if attached.
  
  An original NES or Famicom controller, however, *is* supported, as the SFC controller 
  protocol is simply an 8-bit extension of the original NES / Famicom one. For the NES/FC
  substitute the "B/Y" button nomenclature with "A/B". There is a good reason Nintendo made
  the Super Famicom output the buttons in the order it does, and it is for this NES/FC 
  protocol compatibility.
  
  == WHAT'S GOING ON =
  When buttons are held down on the SFC controller, the Teensy sets the corresponding pin
  to an output, and writes a LOW signal.
  
  When a button is released, the Teensy sets the corresponding pin to an input which puts
  the pin in a HIGH-Z state ( floating ). 
  
  This is so the system may mimic an actual button as much as possible. A real arcade 
  board's inputs are activated on a logic LOW and have pull up resistors on each input. 
  The arcade board expects the buttons to work this way, so I'm not going to be pulling up
  any lines from the board. 
  
  NOTE: For two Super Famicom controllers in the Teensy++ 2.0 configuration, the lines for
  CLOCK and LATCH are shared (run to both controller ports). Only DATA0 and DATA1 are 
  individual, so don't be alarmed thinking you missed something.
    
  // Pin assignment for a Teensy 2.0 (single player)
            __ _____ __
      GND -|  | USB |  |- 5V
          -|  |_____|21|- B
          -|         20|- Y
          -|         19|- Sel
          -|         18|- Start
          -|         17|- Up
        R -|5        16|- Down
          -|         15|- Left
SFC Data0 -|7        14|- Right
SFC Clock -|8        13|- A
SFC Latch -|9        12|- X
        L -|10_______11|- LED PIN (Shows that it works)
            | | | | |

   // Pin assignment for a Teensy++ 2.0 (two player)
            __ _____ __
      GND -|  | USB |  |- 5V
          -|  |_____|26|- 2-B
          -|         25|- 2-Y
          -|         24|- 2-Sel
          -|         23|- 2-Start
          -|         22|- 2-R
          -|         21|- 1-B
      1-R -|5        20|- 1-Y
  LED PIN -|6        19|- 1-Sel
SFC Data0 -|7        18|- 1-Start
SFC Clock -|8          |- GND
SFC Latch -|9          |- 
      1-L -|10       38|- 2-L
SFC Data1 -|11       39|- 
      1-X -|12       40|- 2-X
      1-A -|13       41|- 2-A
  1-Right -|14       42|- 2-Right
   1-Left -|15       43|- 2-Left
   1-Down -|16       44|- 2-Down
     1-Up -|17_o_o_o_45|- 2-Up
   
                 
   // Assignments for the 2 x 2 Neo Geo layout if that's what you're into
   SFC      NEO GEO
   A        D
   B        B
   X        C
   Y        A
   
   
 */

// -------------- Teensy 2.0 pins ----------------------

// Which pin the sfcStates correspond to for a given index
int sfcPins0[] = {
  21,20,19,18,
  17,16,15,14,
  13,12,10,5 };

int sfcPins1[] = {
  26,25,24,23,
  45,44,43,42,
  41,40,38,22 };

// Change the indicator LED based on the system in use
#ifndef TEENSY_PLUS
#define PIN_INDICATOR 11
#endif
#ifdef TEENSY_PLUS
#define PIN_INDICATOR 6
#endif

// SFC controller pins
#define PIN_DATA0 7
#define PIN_DATA1 11
#define PIN_CLOCK 8
#define PIN_LATCH 9

// SFC Inputs - Low means it is pressed
// B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R 
int sfcState[2][12];


// Reset vector
void setup() 
{   
  
  // SFC pins
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA0, INPUT);
  pinMode(PIN_INDICATOR, OUTPUT);
  #ifdef TEENSY_PLUS
  pinMode(PIN_DATA0, INPUT);
  #endif
  digitalWrite(PIN_INDICATOR, LOW);
  
  for (int i = 0; i < 12; i++)
  {
    // Set the pin to input (to float it)
    pinMode(sfcPins0[i], INPUT);
    pinMode(sfcPins1[i], INPUT);
    for (int j = 0; j < 2; j++)
    {
      // Clear the state
      sfcState[j][i] = 1; 
    }
  }  
  // LED pulse to show that it works
  for (int i = 0; i < 6; i++)
  { 
    delay(20); // 33% duty
    digitalWrite(PIN_INDICATOR, HIGH);
    delay(10);
    digitalWrite(PIN_INDICATOR, LOW);
  }
  digitalWrite(PIN_INDICATOR, HIGH);
  delay(300);
  digitalWrite(PIN_INDICATOR, LOW);
}

void pset(int pin, int state)
{
   if (state == 0)
   {
     pinMode(pin, OUTPUT);
     digitalWrite(pin, LOW);
   }
   else
   {
     pinMode(pin, INPUT);
   }
}

// Using the sfcState, set the corresponding pin LOW or HIGH-Z
void set_output()
{
  for (int i = 0; i < 12; i++)
  {
      pset(sfcPins0[i],sfcState[0][i]);
      pset(sfcPins1[i],sfcState[1][i]);
  }
}

// Wait for a shorter time than delay(1);
void wait()
{
  for (int i = 0; i < 800; i++)
  {
    asm("nop\n\t"); 
  }
}

void get_sfc_state()
{
  // Pull latch high to reset SFC button counter
  digitalWrite(PIN_LATCH, HIGH);
  for (int i = 0; i < 12; i++)
  {
    digitalWrite(PIN_CLOCK, HIGH);
    sfcState[0][i] = digitalRead(PIN_DATA0);
    
    #ifdef TEENSY_PLUS
    sfcState[1][i] = digitalRead(PIN_DATA1);
    #endif
    #ifndef TEENSY_PLUS
    sfcState[1][i] = 1;
    #endif
    wait();
    if (i == 0)
    {
      // Bring down the latch for the first run
      digitalWrite(PIN_LATCH, LOW);
    }
    
    // Clock for the next key
    digitalWrite(PIN_CLOCK, LOW);
    wait();
  }
}

// the loop routine runs over and over again forever:
void loop() 
{
  get_sfc_state(); // Grab SFC controller state
  set_output(); // Set the output pins based on SFC pad
  delay(6);
}
