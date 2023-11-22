#include "sfc_decode.h"

#include <Arduino.h>

namespace Mof
{

SFCDecoder::SFCDecoder(const Config &config) : m_config(config)
{
	pinMode(m_config.pin_latch_out, OUTPUT);
	pinMode(m_config.pin_clock_out, OUTPUT);
	digitalWrite(m_config.pin_latch_out, LOW);
	digitalWrite(m_config.pin_clock_out, LOW);
	for (auto pin : m_config.pin_data)
	{
		pinMode(pin, INPUT_PULLUP);
	}
};

void SFCDecoder::Wait() const
{
	constexpr int kWaitCount = 600;
	for (int i = 0; i < kWaitCount; i++) asm("nop");
}

void SFCDecoder::Poll()
{
	// Reset pad data.
	for (auto &buttons : m_buttons)
	{
		buttons = 0x0000;
	}

	// Strobe the latch signal and wait a little.
	digitalWrite(m_config.pin_latch_out, HIGH);
	Wait();
	digitalWrite(m_config.pin_latch_out, LOW);

	// Scan for buttons, all players at once.
	for (int i = 0; i < kButtonCount; i++)
	{
		for (int j = 0; j < kControllerCount; j++)
		{
			m_buttons[j] |= (digitalRead(m_config.pin_data[j]) ? (1 << i) : 0);
		}
		digitalWrite(m_config.pin_clock_out, LOW);
		Wait();
		digitalWrite(m_config.pin_clock_out, HIGH);
	}
}

}  // namespace Mof
