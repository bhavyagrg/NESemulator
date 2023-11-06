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
	bool pulse1_enable = false;
	double pulse1_sample = 0.0;
};

