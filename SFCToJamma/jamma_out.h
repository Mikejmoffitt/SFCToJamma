// Class to manage JAMMA outputs for a single player.
#ifndef JAMMA_OUT_H
#define JAMMA_OUT_H

namespace Mof
{

class JammaPlayer
{
public:
	enum class Button
	{
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3,
		A = 4,
		B = 5,
		C = 6,
		D = 7,
		Start = 8,
		Coin = 9,
		Count
	};

	struct Config
	{
		int output_pin_mapping[static_cast<int>(Button::Count)];
	};

	JammaPlayer(const Config &config);

	// Updates the mapping between user input and pin output.
	void Assign(Button button_in, Button button_out);

	// Sets a button's input state from the user.
	void SetButton(Button button_id, bool value);

	// Expresses the button state - with user remapping - on the output pins.
	void SetOutput() const;

private:
	Config m_config;  // Initialized in constructor.
	Button m_user_mapping[static_cast<int>(Button::Count)];  // Initialized in constructor.
	bool m_user_state[static_cast<int>(Button::Count)] = {false};
};

}  // namespace Mof


#endif  // JAMMA_OUT_H
