#include "CPU.h"
#include "Bus.h"

CPU::CPU()
{
	//The table is one big initialiser list of initialiser lists -> corresponds to the table in the datasheet
	using a = CPU;
	lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZPO, 3 },{ "ASL", &a::ASL, &a::ZPO, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZPO, 3 },{ "AND", &a::AND, &a::ZPO, 3 },{ "ROL", &a::ROL, &a::ZPO, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZPO, 3 },{ "LSR", &a::LSR, &a::ZPO, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZPO, 3 },{ "ROR", &a::ROR, &a::ZPO, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPO, 3 },{ "STA", &a::STA, &a::ZPO, 3 },{ "STX", &a::STX, &a::ZPO, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZPO, 3 },{ "LDA", &a::LDA, &a::ZPO, 3 },{ "LDX", &a::LDX, &a::ZPO, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZPO, 3 },{ "CMP", &a::CMP, &a::ZPO, 3 },{ "DEC", &a::DEC, &a::ZPO, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZPO, 3 },{ "SBC", &a::SBC, &a::ZPO, 3 },{ "INC", &a::INC, &a::ZPO, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};


}

CPU::~CPU()
{
}


//-->>>> Bus Connectivity

uint8_t CPU::read(uint16_t addr)
{
	return bus->read(addr, false);
}

void CPU::write(uint16_t addr, uint8_t data)
{
	bus->write(addr, data);
}

// ------------------ Reset functions resets all the values of registers and memory to their default values------------------------------------------
// ------------------ It let us get a state where we can say we are ready to execute--------------------------
void CPU::reset()
{
	// Get address to set program counter to
	addr_memory = 0xFFFC;
	uint16_t lo = read(addr_memory + 0);
	uint16_t hi = read(addr_memory + 1);

	// Set it
	pc = (hi << 8) | lo;

	// Reset internal registers
	ac = 0;
	x = 0;
	y = 0;
	stkp = 0xFD;
	status = 0x00 | U;

	// Clear internal helper variables
	addr_rel = 0x0000;
	addr_memory = 0x0000;
	fetched = 0x00;

	// Reset takes time
	cycles_left = 8; // -- Default value
}

void CPU::interruptRequestSig()
{
	// If interrupts are allowed
	if (GetFlag(I) == 0)
	{
		// Push the program counter to the stack. It's 16-bits dont
		// forget so that takes two pushes
		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;
		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		// Then Push the status register to the stack
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + stkp, status);
		stkp--;

		// Read new program counter location from fixed address
		addr_memory = 0xFFFE;
		uint16_t lo = read(addr_memory + 0);
		uint16_t hi = read(addr_memory + 1);
		pc = (hi << 8) | lo;

		// IRQs take time
		cycles_left = 7;
	}
}

void CPU::nonMaskableInterruptRequestSiq()
{
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(0x0100 + stkp, status);
	stkp--;

	addr_memory = 0xFFFA;
	uint16_t lo = read(addr_memory + 0);
	uint16_t hi = read(addr_memory + 1);
	pc = (hi << 8) | lo;

	cycles_left = 8;
}

void CPU::clock()
{
	if (cycles_left == 0)
	{
		opcode = read(pc);

	#ifdef LOGMODE
			uint16_t log_pc = pc;
	#endif

		// Always set the unused status flag bit to 1
		SetFlag(U, true);

		pc++;

		//Get Starting number of cycles
		cycles_left = lookup[opcode].cycles;

		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();// function call

		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		cycles_left += (additional_cycle1 & additional_cycle2);
	
		SetFlag(U, true);

	#ifdef LOGMODE
			// This logger dumps every cycle the entire processor state for analysis.
			// This can be used for debugging the emulation, but has little utility
			// during emulation. Its also very slow, so only use if you have to.
			if (logfile == nullptr)	logfile = fopen("olc6502.txt", "wt");
			if (logfile != nullptr)
			{
				fprintf(logfile, "%10d:%02d PC:%04X %s A:%02X X:%02X Y:%02X %s%s%s%s%s%s%s%s STKP:%02X\n",
					clock_count, 0, log_pc, "XXX", a, x, y,
					GetFlag(N) ? "N" : ".", GetFlag(V) ? "V" : ".", GetFlag(U) ? "U" : ".",
					GetFlag(B) ? "B" : ".", GetFlag(D) ? "D" : ".", GetFlag(I) ? "I" : ".",
					GetFlag(Z) ? "Z" : ".", GetFlag(C) ? "C" : ".", stkp);
			}
	#endif
		}

		// Increment global clock count - This is actually unused unless logging is enabled
		// but I've kept it in because its a handy watch variable for debugging
		clock_count++;
	// Everytime clock function is called, one cycle is consumed
	cycles_left--;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// --->>  Flag Functions

uint8_t CPU::GetFlag(FLAGS flgs)
{
	return ((status & flgs) > 0) ? 1 : 0;
}

void CPU::SetFlag(FLAGS flgs, bool value)
{
	if (value) status |= flgs;
	else status &= ~flgs;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Defining Addressing Modes

uint8_t CPU::IMP() // --> implied mode
{
	fetched = ac;
	return 0;
}

uint8_t CPU::IMM() //---> immedite mode
{
	addr_memory = pc++;
	return 0;
}

uint8_t CPU::ZPO() // -----> Zero Page Addressing
{
	addr_memory = read(pc);
	pc++;
	addr_memory &= 0x00FF;
	return 0;
}

uint8_t CPU::ZPX() //Zero Page Addressing with X register offset
{
	addr_memory = (read(pc) + x);
	pc++;
	addr_memory &= 0x00FF;
	return 0;
}

uint8_t CPU::ZPY() //Zero Page Addressing with Y register offset
{
	addr_memory = (read(pc) + y);
	pc++;
	addr_memory &= 0x00FF;
	return 0;
}

uint8_t CPU::ABS() // Absolute Addresing
{
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);
	pc++;

	addr_memory = (high << 8) | low;

	return 0;
}

uint8_t CPU::ABX() // Absolute Addresing with X register offset
{
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);
	pc++;

	addr_memory = (high << 8) | low;
	addr_memory += x;

	// after incrementing addr_mem if page changes to different page then we need to indicate the system that we may need an additional clock cycle so, return 1
	if ((addr_memory & 0xFF00) != (high << 8))
		return 1;
	else
		return 0;
}


uint8_t CPU::ABY() // Absolute Addresing with Y register offset
{
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);
	pc++;

	addr_memory = (high << 8) | low;
	addr_memory += y;

	if ((addr_memory & 0xFF00) != (high << 8))
		return 1;
	else
		return 0;
}



uint8_t CPU::IND() // Indirect Addressing
{
	// 16 bit address that stores the info
	uint16_t ptr_low = read(pc);
	pc++;
	uint16_t ptr_high = read(pc);
	pc++;

	uint16_t ptr = (ptr_high << 8) | ptr_low;

	// reading the 16 bit data at the original address -> it is the new address

	if (ptr_low == 0x00FF) // Simulate page boundary hardware bug 
	{
		addr_memory = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else // Behave normally
	{
		//if low is 255 then high would be 256 , hence we need above if statement, page will be shifted : page boundary hardware bug 
		addr_memory = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}

	uint8_t CPU::IZX() // Indirect Addressing of zero page with x offset
	{
		uint16_t t = read(pc);
		pc++;

		uint16_t low = read((uint8_t)(t + (uint16_t)x) & 0x00FF);
		uint16_t high = read((uint8_t)(t + (uint16_t)x + 1) & 0x00FF);

		addr_memory = (high << 8) | low;

		return 0;
	}

	uint8_t CPU::IZY() // Indirect Addressing of zero page with y offset
	{
		uint16_t t = read(pc);
		pc++;

		uint16_t low = read(t & 0x00FF);
		uint16_t high = read((uint8_t)(t + 1) & 0x00FF);

		addr_memory = (high << 8) | low;
		addr_memory += y;

		if ((addr_memory & 0xFF00) != (high << 8))
			return 1;
		else
			return 0;
	}

	uint8_t CPU::REL() // Relative addressing, only applies to branching instructions
	{
		addr_rel = read(pc);
		pc++;
		if (addr_rel & 0x80)
			addr_rel |= 0xFF00;
		return 0;
	}


	// Instructions
	uint8_t CPU::fetch()
	{
		// fetch from every instruction except implied addresing mode instruction
		if (!(lookup[opcode].addrmode == &CPU::IMP))
			fetched = read(addr_memory);
		return fetched;
	}



	uint8_t CPU::AND()
	{
		fetch();
		ac = ac & fetched;
		SetFlag(Z, ac == 0x00);
		SetFlag(N, ac & 0x80);
		return 1;
	}

	uint8_t CPU::ASL() // Arithemetic shift left
	{
		fetch();
		uint16_t temp = (uint16_t)fetched << 1;
		SetFlag(C, (temp & 0xFF00) > 0);
		SetFlag(Z, (temp & 0x00FF) == 0x00);
		SetFlag(N, temp & 0x80);
		if (lookup[opcode].addrmode == &CPU::IMP)
			ac = temp & 0x00FF;
		else
			write(addr_memory, temp & 0x00FF);
		return 0;
	}


	uint8_t CPU::BCS() // Branching Instruction
	{
		if (GetFlag(C) == 1) // it's a branch if carry bit is set to 1
		{
			cycles_left++;
			addr_memory = pc + addr_rel;

			if ((addr_memory & 0xFF00) != (pc & 0xFF00))
				cycles_left++;

			pc = addr_memory;
		}
		return 0;
	}

	uint8_t CPU::BCC() // Branching carry clear Instruction
	{
		if (GetFlag(C) == 0) // it's a branch if carry bit is set to 1
		{
			cycles_left++;
			addr_memory = pc + addr_rel;

			if ((addr_memory & 0xFF00) != (pc & 0xFF00))
				cycles_left++;

			pc = addr_memory;
		}
		return 0;
	}

	uint8_t CPU::BEQ() // Branching equal Instruction
	{
		if (GetFlag(Z) == 1) // it's a branch if carry bit is set to 1
		{
			cycles_left++;
			addr_memory = pc + addr_rel;

			if ((addr_memory & 0xFF00) != (pc & 0xFF00))
				cycles_left++;

			pc = addr_memory;
		}
		return 0;
	}

	uint8_t CPU::BIT()
	{
		fetch();
		uint16_t temp = ac & fetched;
		SetFlag(Z, (temp & 0x00FF) == 0x00);
		SetFlag(N, fetched & (1 << 7));
		SetFlag(V, fetched & (1 << 6));
		return 0;
	}


	uint8_t CPU::BMI() // Branching negative Instruction
	{
		if (GetFlag(N) == 1) // it's a branch if carry bit is set to 1
		{
			cycles_left++;
			addr_memory = pc + addr_rel;

			if ((addr_memory & 0xFF00) != (pc & 0xFF00))
				cycles_left++;

			pc = addr_memory;
		}
		return 0;
	}

	uint8_t CPU::BNE() // Branching not equal Instruction
	{
		if (GetFlag(Z) == 0) // it's a branch if carry bit is set to 1
		{
			cycles_left++;
			addr_memory = pc + addr_rel;

			if ((addr_memory & 0xFF00) != (pc & 0xFF00))
				cycles_left++;

			pc = addr_memory;
		}
		return 0;
	}

	uint8_t CPU::BPL() // Branching positive Instruction
	{
		if (GetFlag(N) == 0) // it's a branch if carry bit is set to 1
		{
			cycles_left++;
			addr_memory = pc + addr_rel;

			if ((addr_memory & 0xFF00) != (pc & 0xFF00))
				cycles_left++;

			pc = addr_memory;
		}
		return 0;
	}

	uint8_t CPU::BRK()
	{
		pc++;

		SetFlag(I, 1);
		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;
		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		SetFlag(B, 1);
		write(0x0100 + stkp, status);
		stkp--;
		SetFlag(B, 0);

		pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
		return 0;
	}

	uint8_t CPU::BVC() // Branching overflowed Instruction
	{
		if (GetFlag(V) == 0) // it's a branch if carry bit is set to 1
		{
			cycles_left++;
			addr_memory = pc + addr_rel;

			if ((addr_memory & 0xFF00) != (pc & 0xFF00))
				cycles_left++;

			pc = addr_memory;
		}
		return 0;
	}

	uint8_t CPU::BVS() // Branching not overflowed Instruction
	{
		if (GetFlag(V) == 1) // it's a branch if carry bit is set to 1
		{
			cycles_left++;
			addr_memory = pc + addr_rel;

			if ((addr_memory & 0xFF00) != (pc & 0xFF00))
				cycles_left++;

			pc = addr_memory;
		}
		return 0;
	}

	uint8_t CPU::CLC() // clear the carry bit Instruction
	{
		// sets the bit in the status register
		SetFlag(C, false);
		return 0;
	}

	uint8_t CPU::CLD() 
	{
		SetFlag(D, false);
		return 0;
	}

	uint8_t CPU::CLI()
	{
		SetFlag(I, false);
		return 0;
	}

	uint8_t CPU::CLV()
	{
		SetFlag(V, false);
		return 0;
	}

	uint8_t CPU::CMP()
	{
		fetch();
		uint16_t temp = (uint16_t)ac - (uint16_t)fetched;
		SetFlag(C, ac >= fetched);
		SetFlag(Z, (temp & 0x00FF) == 0x0000);
		SetFlag(N, temp & 0x0080);
		return 1;
	}

	uint8_t CPU::CPX()
	{
		fetch();
		uint16_t temp = (uint16_t)x - (uint16_t)fetched;
		SetFlag(C, x >= fetched);
		SetFlag(Z, (temp & 0x00FF) == 0x0000);
		SetFlag(N, temp & 0x0080);
		return 0;
	}

	uint8_t CPU::CPY()
	{
		fetch();
		uint16_t temp = (uint16_t)y - (uint16_t)fetched;
		SetFlag(C, y >= fetched);
		SetFlag(Z, (temp & 0x00FF) == 0x0000);
		SetFlag(N, temp & 0x0080);
		return 0;
	}

	uint8_t CPU::DEC()
	{
		fetch();
		uint16_t temp = fetched - 1;
		write(addr_memory, temp & 0x00FF);
		SetFlag(Z, (temp & 0x00FF) == 0x0000);
		SetFlag(N, temp & 0x0080);
		return 0;
	}

	uint8_t CPU::DEX()
	{
		x--;
		SetFlag(Z, x == 0x00);
		SetFlag(N, x & 0x80);
		return 0;
	}

	uint8_t CPU::DEY()
	{
		y--;
		SetFlag(Z, y == 0x00);
		SetFlag(N, y & 0x80);
		return 0;
	}

	uint8_t CPU::EOR()
	{
		fetch();
		ac = ac ^ fetched;
		SetFlag(Z, ac == 0x00);
		SetFlag(N, ac & 0x80);
		return 1;
	}

	uint8_t CPU::INC()
	{
		fetch();
		uint16_t temp = fetched + 1;
		write(addr_memory, temp & 0x00FF);
		SetFlag(Z, (temp & 0x00FF) == 0x0000);
		SetFlag(N, temp & 0x0080);
		return 0;
	}

	uint8_t CPU::INX()
	{
		x++;
		SetFlag(Z, x == 0x00);
		SetFlag(N, x & 0x80);
		return 0;
	}

	// Instruction: Increment Y Register
	// Function:    Y = Y + 1
	// Flags Out:   N, Z
	uint8_t CPU::INY()
	{
		y++;
		SetFlag(Z, y == 0x00);
		SetFlag(N, y & 0x80);
		return 0;
	}

	// Instruction: Jump To Location
	// Function:    pc = address
	uint8_t CPU::JMP()
	{
		pc = addr_memory;
		return 0;
	}


	// Instruction: Jump To Sub-Routine
	// Function:    Push current pc to stack, pc = address
	uint8_t CPU::JSR()
	{
		pc--;

		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;
		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		pc = addr_memory;
		return 0;
	}


	// Instruction: Load The Accumulator
	// Function:    A = M
	// Flags Out:   N, Z
	uint8_t CPU::LDA()
	{
		fetch();
		ac = fetched;
		SetFlag(Z, ac == 0x00);
		SetFlag(N, ac & 0x80);
		return 1;
	}


	// Instruction: Load The X Register
	// Function:    X = M
	// Flags Out:   N, Z
	uint8_t CPU::LDX()
	{
		fetch();
		x = fetched;
		SetFlag(Z, x == 0x00);
		SetFlag(N, x & 0x80);
		return 1;
	}


	// Instruction: Load The Y Register
	// Function:    Y = M
	// Flags Out:   N, Z
	uint8_t CPU::LDY()
	{
		fetch();
		y = fetched;
		SetFlag(Z, y == 0x00);
		SetFlag(N, y & 0x80);
		return 1;
	}

	uint8_t CPU::LSR()
	{
		fetch();
		SetFlag(C, fetched & 0x0001);
		uint16_t temp = fetched >> 1;
		SetFlag(Z, (temp & 0x00FF) == 0x0000);
		SetFlag(N, temp & 0x0080);
		if (lookup[opcode].addrmode == &CPU::IMP)
			ac = temp & 0x00FF;
		else
			write(addr_memory, temp & 0x00FF);
		return 0;
	}

	uint8_t  CPU::NOP()
	{
		switch (opcode) {
		case 0x1C:
		case 0x3C:
		case 0x5C:
		case 0x7C:
		case 0xDC:
		case 0xFC:
			return 1;
			break;
		}
		return 0;
	}


	// Instruction: Bitwise Logic OR
	// Function:    A = A | M
	// Flags Out:   N, Z
	uint8_t CPU::ORA()
	{
		fetch();
		ac = ac| fetched;
		SetFlag(Z, ac == 0x00);
		SetFlag(N, ac & 0x80);
		return 1;
	}


	uint8_t CPU::ADC() //addition
	{
		fetch();
		uint16_t temp = (uint16_t)ac + (uint16_t)fetched + (uint16_t)GetFlag(C);
		SetFlag(C, temp > 255); // Carry Flag
		SetFlag(Z, (temp & 0x00FF) == 0); // Zero Flag
		SetFlag(N, temp & 0x80); // Negative Flag
		SetFlag(V,(~((uint16_t)ac ^ (uint16_t)fetched) & ((uint16_t)ac ^ (uint16_t)temp)) & 0x0080); // Overflowed Flag
		ac = temp & 0x00FF;
		return 1;
	}

	uint8_t CPU::SBC() //subtraction
	{
		fetch();
		uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
		uint16_t temp = (uint16_t)ac + value + (uint16_t)GetFlag(C);
		SetFlag(C, temp & 0xFF00);
		SetFlag(Z, ((temp & 0x00FF) == 0));
		SetFlag(V, (temp ^ (uint16_t)ac) & (temp ^ value) & 0x0080);
		SetFlag(N, temp & 0x0080);
		ac = temp & 0x00FF;
		return 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// Stack Instructions

	uint8_t CPU::PHA() // pushes ac to stack
	{
		write(0x0100 + stkp, ac);
		stkp--;
		return 0;
	}

	// Instruction: Push Status Register to Stack
	// Function:    status -> stack
	// Note:        Break flag is set to 1 before push
	uint8_t CPU::PHP()
	{
		write(0x0100 + stkp, status | B | U);
		SetFlag(B, 0);
		SetFlag(U, 0);
		stkp--;
		return 0;
	}

	// Instruction: Pop Accumulator off Stack
	// Function:    A <- stack
	// Flags Out:   N, Z
	uint8_t CPU::PLA()
	{
		stkp++;
		ac = read(0x0100 + stkp);
		SetFlag(Z, ac == 0x00);
		SetFlag(N, ac & 0x80);
		return 0;
	}


	// Instruction: Pop Status Register off Stack
	// Function:    Status <- stack
	uint8_t CPU::PLP()
	{
		stkp++;
		status = read(0x0100 + stkp);
		SetFlag(U, 1);
		return 0;
	}

	uint8_t CPU::ROL()
	{
		fetch();
		uint16_t temp = (uint16_t)(fetched << 1) | GetFlag(C);
		SetFlag(C, temp & 0xFF00);
		SetFlag(Z, (temp & 0x00FF) == 0x0000);
		SetFlag(N, temp & 0x0080);
		if (lookup[opcode].addrmode == &CPU::IMP)
			ac = temp & 0x00FF;
		else
			write(addr_memory, temp & 0x00FF);
		return 0;
	}

	uint8_t CPU::ROR()
	{
		fetch();
		uint16_t temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
		SetFlag(C, fetched & 0x01);
		SetFlag(Z, (temp & 0x00FF) == 0x00);
		SetFlag(N, temp & 0x0080);
		if (lookup[opcode].addrmode == &CPU::IMP)
			ac = temp & 0x00FF;
		else
			write(addr_memory, temp & 0x00FF);
		return 0;
	}

		uint8_t CPU::RTI()
	{
		stkp++;
		status = read(0x0100 + stkp);
		status &= ~B;
		status &= ~U;

		stkp++;
		pc = (uint16_t)read(0x0100 + stkp);
		stkp++;
		pc |= (uint16_t)read(0x0100 + stkp) << 8;
		return 0;
	}

	 
		uint8_t CPU::RTS()
		{
			stkp++;
			pc = (uint16_t)read(0x0100 + stkp);
			stkp++;
			pc |= (uint16_t)read(0x0100 + stkp) << 8;

			pc++;
			return 0;
		}


		// Instruction: Set Carry Flag
		// Function:    C = 1
		uint8_t CPU::SEC()
		{
			SetFlag(C, true);
			return 0;
		}


		// Instruction: Set Decimal Flag
		// Function:    D = 1
		uint8_t CPU::SED()
		{
			SetFlag(D, true);
			return 0;
		}


		// Instruction: Set Interrupt Flag / Enable Interrupts
		// Function:    I = 1
		uint8_t CPU::SEI()
		{
			SetFlag(I, true);
			return 0;
		}


		// Instruction: Store Accumulator at Address
		// Function:    M = A
		uint8_t CPU::STA()
		{
			write(addr_memory, ac);
			return 0;
		}


		// Instruction: Store X Register at Address
		// Function:    M = X
		uint8_t CPU::STX()
		{
			write(addr_memory, x);
			return 0;
		}


		// Instruction: Store Y Register at Address
		// Function:    M = Y
		uint8_t CPU::STY()
		{
			write(addr_memory, y);
			return 0;
		}


		// Instruction: Transfer Accumulator to X Register
		// Function:    X = A
		// Flags Out:   N, Z
		uint8_t CPU::TAX()
		{
			x = ac;
			SetFlag(Z, x == 0x00);
			SetFlag(N, x & 0x80);
			return 0;
		}


		// Instruction: Transfer Accumulator to Y Register
		// Function:    Y = A
		// Flags Out:   N, Z
		uint8_t CPU::TAY()
		{
			y = ac;
			SetFlag(Z, y == 0x00);
			SetFlag(N, y & 0x80);
			return 0;
		}


		// Instruction: Transfer Stack Pointer to X Register
		// Function:    X = stack pointer
		// Flags Out:   N, Z
		uint8_t CPU::TSX()
		{
			x = stkp;
			SetFlag(Z, x == 0x00);
			SetFlag(N, x & 0x80);
			return 0;
		}


		// Instruction: Transfer X Register to Accumulator
		// Function:    A = X
		// Flags Out:   N, Z
		uint8_t CPU::TXA()
		{
			ac = x;
			SetFlag(Z, ac == 0x00);
			SetFlag(N, ac & 0x80);
			return 0;
		}


		// Instruction: Transfer X Register to Stack Pointer
		// Function:    stack pointer = X
		uint8_t CPU::TXS()
		{
			stkp = x;
			return 0;
		}


		// Instruction: Transfer Y Register to Accumulator
		// Function:    A = Y
		// Flags Out:   N, Z
		uint8_t CPU::TYA()
		{
			ac = y;
			SetFlag(Z, ac == 0x00);
			SetFlag(N, ac & 0x80);
			return 0;
		}


		// This function captures illegal opcodes
		uint8_t CPU::XXX()
		{
			return 0;
		}





		///////////////////////////////////////////////////////////////////////////////
		// HELPER FUNCTIONS

		bool CPU::complete()
		{
			return cycles_left == 0;
		}

		// This is the disassembly function. Its workings are not required for emulation.
		// It is merely a convenience function to turn the binary instruction code into
		// human readable form. Its included as part of the emulator because it can take
		// advantage of many of the CPUs internal operations to do this.
		std::map<uint16_t, std::string> CPU::disassemble(uint16_t nStart, uint16_t nStop)
		{
			uint32_t addr = nStart;
			uint8_t value = 0x00, lo = 0x00, hi = 0x00;
			std::map<uint16_t, std::string> mapLines;
			uint16_t line_addr = 0;

			// A convenient utility to convert variables into
			// hex strings because "modern C++"'s method with 
			// streams is atrocious
			auto hex = [](uint32_t n, uint8_t d)
				{
					std::string s(d, '0');
					for (int i = d - 1; i >= 0; i--, n >>= 4)
						s[i] = "0123456789ABCDEF"[n & 0xF];
					return s;
				};

			// Starting at the specified address we read an instruction
			// byte, which in turn yields information from the lookup table
			// as to how many additional bytes we need to read and what the
			// addressing mode is. I need this info to assemble human readable
			// syntax, which is different depending upon the addressing mode

			// As the instruction is decoded, a std::string is assembled
			// with the readable output
			while (addr <= (uint32_t)nStop)
			{
				line_addr = addr;

				// Prefix line with instruction address
				std::string sInst = "$" + hex(addr, 4) + ": ";

				// Read instruction, and get its readable name
				uint8_t opcode = bus->read(addr, true); addr++;
				sInst += lookup[opcode].name + " ";

				// Get oprands from desired locations, and form the
				// instruction based upon its addressing mode. These
				// routines mimmick the actual fetch routine of the
				// 6502 in order to get accurate data as part of the
				// instruction
				if (lookup[opcode].addrmode == &CPU::IMP)
				{
					sInst += " {IMP}";
				}
				else if (lookup[opcode].addrmode == &CPU::IMM)
				{
					value = bus->read(addr, true); addr++;
					sInst += "#$" + hex(value, 2) + " {IMM}";
				}
				else if (lookup[opcode].addrmode == &CPU::ZPO)
				{
					lo = bus->read(addr, true); addr++;
					hi = 0x00;
					sInst += "$" + hex(lo, 2) + " {ZP0}";
				}
				else if (lookup[opcode].addrmode == &CPU::ZPX)
				{
					lo = bus->read(addr, true); addr++;
					hi = 0x00;
					sInst += "$" + hex(lo, 2) + ", X {ZPX}";
				}
				else if (lookup[opcode].addrmode == &CPU::ZPY)
				{
					lo = bus->read(addr, true); addr++;
					hi = 0x00;
					sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
				}
				else if (lookup[opcode].addrmode == &CPU::IZX)
				{
					lo = bus->read(addr, true); addr++;
					hi = 0x00;
					sInst += "($" + hex(lo, 2) + ", X) {IZX}";
				}
				else if (lookup[opcode].addrmode == &CPU::IZY)
				{
					lo = bus->read(addr, true); addr++;
					hi = 0x00;
					sInst += "($" + hex(lo, 2) + "), Y {IZY}";
				}
				else if (lookup[opcode].addrmode == &CPU::ABS)
				{
					lo = bus->read(addr, true); addr++;
					hi = bus->read(addr, true); addr++;
					sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
				}
				else if (lookup[opcode].addrmode == &CPU::ABX)
				{
					lo = bus->read(addr, true); addr++;
					hi = bus->read(addr, true); addr++;
					sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
				}
				else if (lookup[opcode].addrmode == &CPU::ABY)
				{
					lo = bus->read(addr, true); addr++;
					hi = bus->read(addr, true); addr++;
					sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
				}
				else if (lookup[opcode].addrmode == &CPU::IND)
				{
					lo = bus->read(addr, true); addr++;
					hi = bus->read(addr, true); addr++;
					sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
				}
				else if (lookup[opcode].addrmode == &CPU::REL)
				{
					value = bus->read(addr, true); addr++;
					sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
				}

				// Add the formed string to a std::map, using the instruction's
				// address as the key. This makes it convenient to look for later
				// as the instructions are variable in length, so a straight up
				// incremental index is not sufficient.
				mapLines[line_addr] = sInst;
			}

			return mapLines;
		}