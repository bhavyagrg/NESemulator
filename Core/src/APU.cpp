#include "APU.h"

APU::APU()
{

}

APU::~APU()
{
}

void APU::CPUWrite(uint16_t addr, uint8_t data)
{
	switch (addr)
	{
	case 0x4000:
		// responsible for setting the duty cycle of channel one pulse waveform
		switch ((data & 0xC0) >> 6)
		{
		case 0x00: pulse1_seq.sequence = 0b00000001; pulse1_osc.dutycycle = 0.125; break;
		case 0x01: pulse1_seq.sequence = 0b00000011; pulse1_osc.dutycycle = 0.250; break;
		case 0x02: pulse1_seq.sequence = 0b00001111; pulse1_osc.dutycycle = 0.500; break;
		case 0x03: pulse1_seq.sequence = 0b11111100; pulse1_osc.dutycycle = 0.750; break;
		}
		
		break;

	case 0x4001:
		break;

	case 0x4002:
		//frequency at which these are rotated is set by controlling the pulse1 sequencer reload value, this happens at address 4002 and 4003, because counter_value is 16 bits and we can only communicate 8 bits at a time via the cpu.
		pulse1_seq.reload = (pulse1_seq.reload & 0xFF00) | data;
		break;

	case 0x4003:
		pulse1_seq.reload = (uint16_t)((data & 0x07)) << 8 | (pulse1_seq.reload & 0x00FF);
		pulse1_seq.counter_value = pulse1_seq.reload; 
		break;

	case 0x4004:
		break;

	case 0x4005:
		break;

	case 0x4006:
		break;

	case 0x4007:
		break;

	case 0x4008:
		break;

	case 0x400C:
		break;

	case 0x400E:
		break;

	case 0x4015:
		// Whole channel can be enabled or disabled when cpu writes to address 4015
		pulse1_enable = data & 0x01;
		break;

	case 0x400F:
		break;
	}
}

uint8_t APU::CPURead(uint16_t addr)
{
	return 0;
}

void APU::clock()
{
	bool bQuarterFrameClock = false; // bQuarterFrameClock happens every quarter of frame
	bool bHalfFrameClock = false;// happens every half of a frame
	dGlobalTime += (0.3333333333 / 1789773);
	// clock of APU runs at the half the rate of the cpu clock and cpu clock runs at third the rate of ppu clock, hence ppu clock is 6 times faster than  apu clock
	// if clock_counter is evenly div by 6 then we change the state of the apu
	if (clock_counter % 6 == 0)
	{
		frame_clock_counter++;

		// 4-Step Sequence Mode-->> responsible of giving even timing over the duration of one of he nes's internal sample periods
		if (frame_clock_counter == 3729)
		{
			bQuarterFrameClock = true;
		}

		if (frame_clock_counter == 7457)
		{
			bQuarterFrameClock = true;
			bHalfFrameClock = true;
		}

		if (frame_clock_counter == 11186)
		{
			bQuarterFrameClock = true;
		}

		if (frame_clock_counter == 14916)
		{
			bQuarterFrameClock = true;
			bHalfFrameClock = true;
			frame_clock_counter = 0;
		}

		// Update functional units

		// Quater frame "beats" adjust the volume envelope
		if (bQuarterFrameClock)
		{
			/*pulse1_env.clock(pulse1_halt);
			pulse2_env.clock(pulse2_halt);
			noise_env.clock(noise_halt);*/
		}


		// Half frame "beats" adjust the duartion of the note  and
		// frequency sweepers
		if (bHalfFrameClock)
		{
			/*pulse1_lc.clock(pulse1_enable, pulse1_halt);
			pulse2_lc.clock(pulse2_enable, pulse2_halt);
			noise_lc.clock(noise_enable, noise_halt);
			pulse1_sweep.clock(pulse1_seq.reload, 0);
			pulse2_sweep.clock(pulse2_seq.reload, 1);*/
		}

		// Update Pulse1 Channel ==========================
		// seq_value is the sequence value stored in the sequencer struct
		//pulse1_seq.clock(pulse1_enable, [](uint32_t& seq_value)
		//	{
		//		// rotate the word
		//		// shift right by 1 bit , wrapping around
		//		seq_value = ((seq_value & 0x0001) << 7) | ((seq_value & 0x00FE) >> 1);
		//	});
		


		// above will generate a pulse square waveform, so setting output of sequencer to the pulse sample
		//pulse1_sample = (double)pulse1_seq.output;

		// function taken from nesdev wiki
		pulse1_osc.frequency = 1789773.0 / (16.0 * (double)(pulse1_seq.reload + 1));
		pulse1_sample = pulse1_osc.sample(dGlobalTime);
	}
	clock_counter++;
}

void APU::reset()
{
}

double APU::GetOutputSample()
{
	return pulse1_sample;
}
