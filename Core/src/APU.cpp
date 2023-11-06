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
		break;

	case 0x4001:
		break;

	case 0x4002:
		break;

	case 0x4003:
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
}

void APU::reset()
{
}

double APU::GetOutputSample()
{
	return pulse1_sample;
}
