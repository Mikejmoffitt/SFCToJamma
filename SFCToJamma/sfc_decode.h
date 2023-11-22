// Class for decoding Super Famicom controllers.
#ifndef SFC_DECODE_H
#define SFC_DECODE_H

#include <stdint.h>

namespace Mof
{

class SFCDecoder
{
public:
	static constexpr int kButtonCount = 12;
	static constexpr int kControllerCount = 2;
	// Button bitfield entries.
	static constexpr uint16_t kBtnB = 1 << 0;
	static constexpr uint16_t kBtnY = 1 << 1;
	static constexpr uint16_t kBtnSelect = 1 << 2;
	static constexpr uint16_t kBtnStart = 1 << 3;
	static constexpr uint16_t kBtnUp = 1 << 4;
	static constexpr uint16_t kBtnDown = 1 << 5;
	static constexpr uint16_t kBtnLeft = 1 << 6;
	static constexpr uint16_t kBtnRight = 1 << 7;
	static constexpr uint16_t kBtnA = 1 << 8;
	static constexpr uint16_t kBtnX = 1 << 9;
	static constexpr uint16_t kBtnL = 1 << 10;
	static constexpr uint16_t kBtnR = 1 << 11;

	struct Config
	{
		int pin_clock_out;
		int pin_latch_out;
		int pin_data[kControllerCount];
	};

	SFCDecoder(const Config &config);

	// Updates button state and prepares data for GetButtons().
	void Poll();

	uint16_t GetButtons(int player) const { return m_buttons[player]; }

private:
	Config m_config;  // Initialized in constructor.
	uint16_t m_buttons[kControllerCount] = {0};

	void Wait() const;
};

}  // namespace Mof

#endif  // SFC_DECODE_H
