// Super Famicom controller-based JAMMA controller
// Original (c) 2014 Michael Moffitt
// Rewrite (c) 2023 Michael Moffitt

/*

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
   1 Coin -|9        12|- 2 Coin
   SFC D0 -|10_______11|- SFC D1
            | | | | |
     Clock 23       22 Latch

 */

#include "sfc_decode.h"
#include "jamma_out.h"

namespace
{
	// Output pin mappings for players based on Teensy 2.0.
	constexpr Mof::JammaPlayer::Config kJammaConfigP1 =
	{
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
	};
	constexpr Mof::JammaPlayer::Config kJammaConfigP2 =
	{
		{21, 20, 19, 18, 17, 16, 15, 14, 13, 12}
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
}

void loop()
{
	sfc_decoder.Poll();
	// TODO: Interactive user button remapping and autofire
	UpdateJamma(jamma_p1, sfc_decoder.GetButtons(0));
	UpdateJamma(jamma_p2, sfc_decoder.GetButtons(1));
}
