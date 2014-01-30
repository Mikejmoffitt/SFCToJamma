SFCToJamma
==========

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
    
Pinouts are visible in the comments in the code.
   
== BORING THINGS ==

This software is licensed under the MIT license:

Copyright (c) 2014 Michael Moffitt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
