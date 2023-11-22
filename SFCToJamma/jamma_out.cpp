#include "jamma_out.h"

#include <Arduino.h>

namespace Mof
{

JammaPlayer::JammaPlayer(const Config &config) : m_config(config)
{
	for (int i = 0; i < static_cast<int>(Button::Count); i++)
	{
		m_user_mapping[i] = static_cast<Button>(i);
	}

	for (auto pin : m_config.output_pin_mapping)
	{
		pinMode(pin, INPUT);
		digitalWrite(pin, LOW);
	}
}

// Updates the mapping between user input and pin output.
void JammaPlayer::Assign(Button button_in, Button button_out)
{
	m_user_mapping[static_cast<int>(button_in)] = button_out;
}

// Sets a button's input state from the user.
void JammaPlayer::SetButton(Button button_id, bool value)
{
	m_user_state[static_cast<int>(button_id)] = value;
}

// Expresses the button state - with user remapping - on the output pins.
void JammaPlayer::SetOutput() const
{
	for (int i = 0; i < static_cast<int>(Button::Count); i++)
	{
		const bool button_pressed = m_user_state[i];
		const Button logical_button = m_user_mapping[i];
		const int output_pin = m_config.output_pin_mapping[static_cast<int>(logical_button)];
    
#ifdef WANT_TEST_SERIAL_OUT
		char msg[64];
		snprintf(msg, sizeof(msg), "Jamma %p : pin %d = btn %d : %d\n", this, output_pin, static_cast<int>(logical_button), button_pressed);
		Serial.write(msg);
#endif  // WANT_TEST_SERIAL_OUT
		// Set to drive LOW or set to HI-Z (input) based on button.
		pinMode(output_pin, button_pressed ? OUTPUT : INPUT);
	}
}

}  // namespace Mof
