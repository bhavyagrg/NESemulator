#pragma once
#include<string>
#include<vector>
#include <map>

#ifdef LOGMODE
#include <stdio.h>
#endif

class Bus;

class CPU
{
public:

	CPU();
	~CPU();

	uint8_t ac = 0x00; //------ Accumulator Register;
	uint8_t x = 0x00; //------- X Register
	uint8_t y = 0x00; //------- Y Register
	uint8_t stkp = 0x00; //---- Stack Pointer (points to location o bus)
	uint16_t pc = 0x00; //----- Program Counter
	uint8_t status = 0x00; //-- Status Register

	// bus is passed as a pointer to the actual bus , and it's address is given to the cpu

	// --->> Addressing Modes
	uint8_t IMP(); uint8_t IMM();
	uint8_t ZPO(); uint8_t ZPX();
	uint8_t ZPY(); uint8_t REL();
	uint8_t ABS(); uint8_t ABX();
	uint8_t ABY(); uint8_t IND();
	uint8_t IZX(); uint8_t IZY();

	


	// --->> Opcodes :  An opcode is a named instruction. Instructions are of 8 bit in 6502 processor
	// ---->> There are 56 legal opcodes
	uint8_t ADC(); uint8_t AND(); uint8_t ASL(); uint8_t BCC();
	uint8_t BCS(); uint8_t BEQ(); uint8_t BIT(); uint8_t BMI();
	uint8_t BNE(); uint8_t BPL(); uint8_t BRK(); uint8_t BVC();
	uint8_t BVS(); uint8_t CLC(); uint8_t CLD(); uint8_t CLI();
	uint8_t CLV(); uint8_t CMP(); uint8_t CPX(); uint8_t CPY();
	uint8_t DEC(); uint8_t DEX(); uint8_t DEY(); uint8_t EOR();
	uint8_t INC(); uint8_t INX(); uint8_t INY(); uint8_t JMP();
	uint8_t JSR(); uint8_t LDA(); uint8_t LDX(); uint8_t LDY();
	uint8_t LSR(); uint8_t NOP(); uint8_t ORA(); uint8_t PHA();
	uint8_t PHP(); uint8_t PLA(); uint8_t PLP(); uint8_t ROL();
	uint8_t ROR(); uint8_t RTI(); uint8_t RTS(); uint8_t SBC();
	uint8_t SEC(); uint8_t SED(); uint8_t SEI(); uint8_t STA();
	uint8_t STX(); uint8_t STY(); uint8_t TAX(); uint8_t TAY();
	uint8_t TSX(); uint8_t TXA(); uint8_t TXS(); uint8_t TYA();

	uint8_t XXX(); // For capturing illegal opcodes
	
	// -->>> Functions performed by the CPU

	void clock();
	void reset();
	void interruptRequestSig();
	void nonMaskableInterruptRequestSiq(); // can never be disabled

	bool complete();

	void ConnectBus(Bus* bus) { this->bus = bus; }

	std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);

	// -->> Internal Helper functions
	uint8_t fetch();
	uint8_t fetched = 0x00; //------ fetched data will be stored here

	// memory locations from where we wanna read depend on the addressing modes
	uint16_t addr_memory = 0x0000; // Location from where we wanna read the data
	uint16_t addr_rel = 0x00; // relative address, for branch instructions
	uint8_t opcode = 0x00; // to store the opcode I am currently working with
	uint8_t cycles_left = 0;

	enum FLAGS
	{
		C = (1 << 0), // Carry Bit
		Z = (1 << 1), // Zero
		I = (1 << 2), // Disable Interuppts
		D = (1 << 3), // Decimal Mode
		B = (1 << 4), // Break
		U = (1 << 5), // Unused
		V = (1 << 6), // Overflow
		N = (1 << 7), // Negative
	};

private:
	Bus *bus = nullptr;
	uint8_t cpuRead(uint16_t addr);
	void cpuWrite(uint16_t addr, uint8_t data);

	//--->>> Convenience functions to access status register
	uint8_t GetFlag(FLAGS flgs);
	void SetFlag(FLAGS flgs, bool value);

	int clock_count = 0;


	// 16 * 16 table of opcodes
	struct INSTRUCTION
	{
		std::string name; // This holds the nemonic
		uint8_t(CPU::* operate)(void) = nullptr; // function pointer of type void, function pointer to the operation to be performed
		uint8_t(CPU::* addrmode)(void) = nullptr; // function pointer to the address mode
		uint8_t cycles = 0; // Number of clock cycles instruction requires to get executed
	};

	std::vector<INSTRUCTION> lookup; // Lookup is vector of INSTRUCTION type which stores all the information of the stuct INSTRUCTION

#ifdef LOGMODE
private:
	FILE* logfile = nullptr;
#endif
};




