#pragma once

#include<cstdint>
#include<functional>

class APU
{
public:
	APU();
	~APU();

public:
	void CPUWrite(uint16_t addr, uint8_t data);
	uint8_t CPURead(uint16_t addr);
	void clock();
	void reset();

	double GetOutputSample();

private:

	uint32_t frame_clock_counter = 0; // it is used in maintaining the musical timing of the apu
	uint32_t clock_counter = 0;

	struct sequencer
	{
		uint32_t sequence = 0x00000000;//represents the data stored in the sequencer, inour instance it could be a 8 bit waveform
		uint16_t counter_value = 0x0000;
		uint16_t reload = 0x0000;// reload value at which our counter gets set to
		uint8_t output = 0x00;// instantaneous output for our sequencer

		uint8_t clock(bool bEnable, std::function<void(uint32_t& s)> funcManip)
		{
			//std::function<void(uint32_t& s)> funcManip ----->>> is a function pointer to a lamda function which implements the do something part of this structure
			if (bEnable)
			{
				counter_value--;
				if (counter_value == 0xFFFF)
				{
					// reset our counter value to reload value and set our output to be the least significant bit of our output data
					counter_value = reload + 1;
					funcManip(sequence); // this lambda function will be provided somewhere else but it's going to operate on the sequence variable contained in the sequencer struct 
					output = sequence & 0x00000001;
				}
			}
			return output;
		}
	};

	struct oscpulse // oscillator pulse
	{
		double frequency = 0;
		double dutycycle = 0;
		double amplitude = 1;
		double pi = 3.14159;
		double harmonics = 20;

		double sample(double t)
		{
			double a = 0;
			double b = 0;
			double p = dutycycle * 2.0 * pi;

			// to remove slow emission of sound
			auto approxsin = [](float t)
				{
					float j = t * 0.151915;
					j = j - (int)j;
					return 20.785 * j * (j - 0.5) * (j - 1.0);
				};

			for (double n = 1; n < harmonics; n++)
			{
				double c = n * frequency * 2.0 * pi * t;
				a += -approxsin(c) / n;
				b += -approxsin(c - p * n) / n;
			}

			return (2.0 * amplitude / pi) * (a - b);

		}
	};

	sequencer pulse1_seq;
	oscpulse pulse1_osc;
	bool pulse1_enable = false;
	double pulse1_sample = 0.0;

	double dGlobalTime = 0.0;
};

