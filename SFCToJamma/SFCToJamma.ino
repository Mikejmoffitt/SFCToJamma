/*
  JammaSFC
  Michael Moffitt
  http://mikejmoffitt.com
  
// Uncomment this line for single-player use with a regular Teensy 2.0

// #define TEENSY_PLUS
  
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
    
  // Pin assignment for a Teensy 2.0 (single player)
       P1   __ _____ __   P2
      GND -|  | USB |  |- 5V
      1 U -|0 |_____|21|- 2 U
      1 D -|1        20|- 2 D
      1 L -|2        19|- 2 L
      1 R -|3        18|- 2 R
      1 Y -|4        17|- 2 Y
      1 B -|5        16|- 2 B
      1 X -|6        15|- 2 X
      1 A -|7        14|- 2 A
  1 Start -|8        13|- 2 Start
 1 Select -|9        12|- 2 Select
  SFC D0  -|10_______11|- SFC D1
            | | | | |
     Clock 23       22 Latch   
                 
   // Assignments for the 2 x 2 Neo Geo layout if that's what you're into
   SFC      NEO GEO
   A        D
   B        B
   X        C
   Y        A
   
   
 */

// -------------- Teensy 2.0 pins ----------------------

// Which pin the sfcStates correspond to for a given index
const int sfcPins0[] = {
  5, 4, 9, 8, 
  0, 1, 2, 3, 
  7, 6};

const int sfcPins1[] = {
  16, 17, 12, 13,
  21, 20, 19, 18, 
  14, 15};

// Change the indicator LED based on the system in use
#define PIN_INDICATOR 11

// SFC controller pins
#define PIN_DATA0 10
#define PIN_DATA1 11
#define PIN_CLOCK 23
#define PIN_LATCH 22

// Number of positions to read from SFC controller
#define NUM_BUTTONS 12

// We ignore L and R
#define NUM_SCANNED 10

// SFC Inputs - Low means it is pressed
// B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R 
int sfcState[2][NUM_BUTTONS];


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
  
  for (int i = 0; i < NUM_SCANNED; i++)
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
  for (int i = 0; i < NUM_SCANNED; i++)
  {
      pset(sfcPins0[i],sfcState[0][i]);
      pset(sfcPins1[i],sfcState[1][i]);
  }
}

// Wait for a shorter time than delay(1);
void wait()
{
  for (int i = 0; i < 600; i++)
  {
    asm("nop\n\t"); 
  }
}

void get_sfc_state()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (i == 0)
    {
      // Pull latch high to reset SFC button counter
      digitalWrite(PIN_LATCH, HIGH);
      digitalWrite(PIN_CLOCK, LOW);
      wait();
    }
    else
    {
      digitalWrite(PIN_CLOCK, HIGH); 
    }
    wait();
    sfcState[0][i] = digitalRead(PIN_DATA0);
    
    #ifdef TEENSY_PLUS
    sfcState[1][i] = digitalRead(PIN_DATA1);
    #endif
    #ifndef TEENSY_PLUS
    sfcState[1][i] = 1;
    #endif
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
