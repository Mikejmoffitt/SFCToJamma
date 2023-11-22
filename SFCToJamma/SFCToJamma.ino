// Super Famicom controller-based JAMMA controller
// Original (c) 2014 Michael Moffitt
// Rewrite (c) 2023 Michael Moffitt

// #define WANT_TEST_SERIAL_OUT

/*

  // Pin assignment for a Teensy 2.0 (single player)
       P2   __ _____ __   P1
      GND -|  | USB |  |- 5V
     Coin -|0 |_____|21|- Coin
    Start -|1        20|- Start
        D -|2        19|- D
        C -|3        18|- C
        B -|4        17|- B
        A -|5        16|- A
    Right -|6        15|- Right
     Left -|7        14|- Left
     Down -|8        13|- Down
       Up -|9        12|- Up
   SFC D0 -|10_______11|- SFC D1
            | | | | |
     Clock 23       22 Latch

 */

#include "sfc_decode.h"
#include "jamma_out.h"

#ifdef WANT_TEST_SERIAL_OUT
#include <stdio.h>
#endif  // WANT_TEST_SERIAL_OUT

namespace
{
	// Output pin mappings for players based on Teensy 2.0.
	constexpr Mof::JammaPlayer::Config kJammaConfigP1 =
	{
		{12, 13, 14, 15, 16, 17, 18, 19, 20, 21}
	};
	constexpr Mof::JammaPlayer::Config kJammaConfigP2 =
	{
		{9, 8, 7, 6, 5, 4, 3, 2, 1, 0}
	};

	constexpr Mof::SFCDecoder::Config kSFCDecoderConfig =
	{
		23, 22, {10, 11}
	};

	constexpr int kPinLed = 11;  // Shared with SFC D1...

	Mof::JammaPlayer jamma_p1{kJammaConfigP1};
	Mof::JammaPlayer jamma_p2{kJammaConfigP2};
	Mof::SFCDecoder sfc_decoder{kSFCDecoderConfig};

	void UpdateJamma(Mof::JammaPlayer &jamma, const uint16_t sfc_data)
	{
#ifdef WANT_TEST_SERIAL_OUT
		char msg[64];
		snprintf(msg, sizeof(msg), "Jamma %p : $%04X\n", &jamma, sfc_data);
		Serial.write(msg);
#endif  // WANT_TEST_SERIAL_OUT
		jamma.SetButton(Mof::JammaPlayer::Button::Up,
		                sfc_data & Mof::SFCDecoder::kBtnUp);
		jamma.SetButton(Mof::JammaPlayer::Button::Down,
		                sfc_data & Mof::SFCDecoder::kBtnDown);
		jamma.SetButton(Mof::JammaPlayer::Button::Left,
		                sfc_data & Mof::SFCDecoder::kBtnLeft);
		jamma.SetButton(Mof::JammaPlayer::Button::Right,
		                sfc_data & Mof::SFCDecoder::kBtnRight);
		jamma.SetButton(Mof::JammaPlayer::Button::A,
		                sfc_data & Mof::SFCDecoder::kBtnY);
		jamma.SetButton(Mof::JammaPlayer::Button::B,
		                sfc_data & Mof::SFCDecoder::kBtnB);
		jamma.SetButton(Mof::JammaPlayer::Button::C,
		                sfc_data & Mof::SFCDecoder::kBtnX);
		jamma.SetButton(Mof::JammaPlayer::Button::D,
		                sfc_data & Mof::SFCDecoder::kBtnA);
		jamma.SetButton(Mof::JammaPlayer::Button::Start,
		                sfc_data & Mof::SFCDecoder::kBtnStart);
		jamma.SetButton(Mof::JammaPlayer::Button::Coin,
		                sfc_data & Mof::SFCDecoder::kBtnSelect);
		jamma.SetOutput();
	}
}  // namespace

void setup()
{
	// Necessary data initialization is carried out in constructors.
#ifdef WANT_TEST_SERIAL_OUT
	Serial.begin(9600);
	Serial.write("Begin");
#endif  // WANT_TEST_SERIAL_OUT
}

void loop()
{
	sfc_decoder.Poll();
	// TODO: Interactive user button remapping and autofire
	UpdateJamma(jamma_p1, sfc_decoder.GetButtons(0));
	UpdateJamma(jamma_p2, sfc_decoder.GetButtons(1));
#ifdef WANT_TEST_SERIAL_OUT
	delay(100);
#endif  // WANT_TEST_SERIAL_OUT
}
