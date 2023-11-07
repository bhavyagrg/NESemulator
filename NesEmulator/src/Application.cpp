#define OLC_PGE_APPLICATION
#include "Application.h"

Application::Application() { sAppName = "NES Emulator"; }

std::string Application::hex(uint32_t n, uint8_t d)
{
	std::string s(d, '0');
	for (int i = d - 1; i >= 0; i--, n >>= 4)
		s[i] = "0123456789ABCDEF"[n & 0xF];
	return s;
};

void Application::DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
{
	int nRamX = x, nRamY = y;
	for (int row = 0; row < nRows; row++)
	{
		std::string sOffset = "$" + hex(nAddr, 4) + ":";
		for (int col = 0; col < nColumns; col++)
		{
			sOffset += " " + hex(bus.cpuRead(nAddr, true), 2);
			nAddr += 1;
		}
		DrawString(nRamX, nRamY, sOffset);
		nRamY += 10;
	}
}

void Application::DrawCpu(int x, int y)
{
	std::string status = "STATUS: ";
	DrawString(x, y, "STATUS:", olc::WHITE);
	DrawString(x + 64, y, "N", bus.cpu.status & CPU::N ? olc::GREEN : olc::RED);
	DrawString(x + 80, y, "V", bus.cpu.status & CPU::V ? olc::GREEN : olc::RED);
	DrawString(x + 96, y, "-", bus.cpu.status & CPU::U ? olc::GREEN : olc::RED);
	DrawString(x + 112, y, "B", bus.cpu.status & CPU::B ? olc::GREEN : olc::RED);
	DrawString(x + 128, y, "D", bus.cpu.status & CPU::D ? olc::GREEN : olc::RED);
	DrawString(x + 144, y, "I", bus.cpu.status & CPU::I ? olc::GREEN : olc::RED);
	DrawString(x + 160, y, "Z", bus.cpu.status & CPU::Z ? olc::GREEN : olc::RED);
	DrawString(x + 178, y, "C", bus.cpu.status & CPU::C ? olc::GREEN : olc::RED);
	DrawString(x, y + 10, "PC: $" + hex(bus.cpu.pc, 4));
	DrawString(x, y + 20, "A: $" + hex(bus.cpu.ac, 2) + "  [" + std::to_string(bus.cpu.ac) + "]");
	DrawString(x, y + 30, "X: $" + hex(bus.cpu.x, 2) + "  [" + std::to_string(bus.cpu.x) + "]");
	DrawString(x, y + 40, "Y: $" + hex(bus.cpu.y, 2) + "  [" + std::to_string(bus.cpu.y) + "]");
	DrawString(x, y + 50, "Stack P: $" + hex(bus.cpu.stkp, 4));
}

void Application::DrawCode(int x, int y, int nLines)
{
	auto it_a = mapAsm.find(bus.cpu.pc);
	int nLineY = (nLines >> 1) * 10 + y;
	if (it_a != mapAsm.end())
	{
		DrawString(x, nLineY, (*it_a).second, olc::CYAN);
		while (nLineY < (nLines * 10) + y)
		{
			nLineY += 10;
			if (++it_a != mapAsm.end())
			{
				DrawString(x, nLineY, (*it_a).second);
			}
		}
	}

	it_a = mapAsm.find(bus.cpu.pc);
	nLineY = (nLines >> 1) * 10 + y;
	if (it_a != mapAsm.end())
	{
		while (nLineY > y)
		{
			nLineY -= 10;
			if (--it_a != mapAsm.end())
			{
				DrawString(x, nLineY, (*it_a).second);
			}
		}
	}
}

// OnUserCreate is called when a window is created
bool Application::OnUserCreate()
{
	// Load the cartridge
	cart = std::make_shared<Cartridge>("./nesfiles/popeye.nes");

	if (!cart->ImageValid())
		return false;

	// Insert cartridge into bus
	bus.insertCartridge(cart);

	// Extract dissassembly
	mapAsm = bus.cpu.disassemble(0x0000, 0xFFFF);

	pInstance = this;
	bus.SetSampleFrequency(44100);

	olc::SOUND::InitialiseAudio(44100, 1, 8, 512);// 44100 is the sample rate for our sysytem and is really imp
	olc::SOUND::SetUserSynthFunction(SoundOut);

	// Reset NES
	bus.reset();
	return true;
}

static float SoundOut(int nChannel, float fGlobalTime, float fTimeStamp)
{
	while (!pInstance->bus.clock())
	{

	}
	return static_cast<float>(pInstance->bus.dAudioSample);
}

bool OnUserDestroy()override
{
	olc::SOUND::DestroyAudio();
	return true;
}

bool Application::EmulatorUpdateWithAudio(float fElapsedTime)
{
	Clear(olc::BLACK);

	//held function pf pixelgame engine gives instantaneous state of any key on the keyboard to assemble the 8 - bit word which we will send to nes component 
	bus.controller[0] = 0x00;
	bus.controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;  // A button
	bus.controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00; // B button
	bus.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00; //Select
	bus.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00; //Start
	bus.controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
	bus.controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
	bus.controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
	bus.controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;

	if (GetKey(olc::Key::SPACE).bPressed)
		bEmulationRun = !bEmulationRun;
	if (GetKey(olc::Key::R).bPressed)
		bus.reset();
	if (GetKey(olc::Key::P).bPressed)
		(++nSelectedPalette) &= 0x07;



	DrawCpu(516, 2); // draw the state of the cpu
	//DrawCode(516, 72, 26);// draw some disassembled code

	// Draw OAM Contents (first 26 out of 64) ======================================
	for (int i = 0; i < 26; i++)
	{
		std::string s = hex(i, 2) + ": (" + std::to_string(bus.ppu.pOAM[i * 4 + 3])
			+ ", " + std::to_string(bus.ppu.pOAM[i * 4 + 0]) + ") "
			+ "ID: " + hex(bus.ppu.pOAM[i * 4 + 1], 2) +
			+" AT: " + hex(bus.ppu.pOAM[i * 4 + 2], 2);
		DrawString(516, 72 + i * 10, s);
	}

	// Draw Palettes & Pattern Tables ==============================================
	const int nSwatchSize = 6;
	for (int p = 0; p < 8; p++) // For each palette
		for (int s = 0; s < 4; s++) // For each index
			FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340,
				nSwatchSize, nSwatchSize, bus.ppu.GetColourFromPaletteRam(p, s));

	// Draw selection reticule around selected palette
	DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);


	DrawSprite(516, 348, &bus.ppu.GetPatternTable(0, nSelectedPalette));
	DrawSprite(648, 348, &bus.ppu.GetPatternTable(1, nSelectedPalette));


	DrawSprite(0, 0, &bus.ppu.GetScreen(), 2);

	//olc::Sprite& s = bus.ppu.GetPatternTable(1, nSelectedPalette);
	//for (uint8_t y = 0; y < 30; y++)
	//{
	//	for (uint8_t x = 0; x < 32; x++)
	//	{
	//		//DrawString(x * 16, y * 16, hex((uint32_t)bus.ppu.vRam[0][y * 32 + x], 2));
	//		uint8_t id = (uint32_t)bus.ppu.vRam[0][y * 32 + x];
	//		DrawPartialSprite(x * 16, y * 16, &s,
	//			(id & 0x0F) << 3, ((id >> 4) & 0x0F) << 3, 8, 8, 2);
	//	}
	//}

	return true;
}
	
bool Application::OnUserUpdate(float fElapsedTime) override
{
	EmulatorUpdateWithAudio(fElapsedTime);
	return true;
}
// Any update to the window like resize, etc
bool Application::EmulatorUpdateWithoutAudio(float fElapsedTime)
{

	Clear(olc::BLACK);

	//held function pf pixelgame engine gives instantaneous state of any key on the keyboard to assemble the 8 - bit word which we will send to nes component 
	bus.controller[0] = 0x00;
	bus.controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;  // A button
	bus.controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00; // B button
	bus.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00; //Select
	bus.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00; //Start
	bus.controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
	bus.controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
	bus.controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
	bus.controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;

	if (GetKey(olc::Key::SPACE).bPressed)
		bEmulationRun = !bEmulationRun;
	if (GetKey(olc::Key::R).bPressed)
		bus.reset();
	if (GetKey(olc::Key::P).bPressed)
		(++nSelectedPalette) &= 0x07;

	if (bEmulationRun)
	{
		if (fResidualTime > 0.0f)
			fResidualTime -= fElapsedTime;
		else
		{
			fResidualTime += (1.0f / 60.0f) - fElapsedTime;
			do { 
				bus.clock();
			} while (!bus.ppu.frame_complete);
			bus.ppu.frame_complete = false;
		}
	}
	else
	{
		// Emulate code step-by-step
		if (GetKey(olc::Key::C).bPressed)
		{
			// Clock enough times to execute a whole CPU instruction
			do { bus.clock(); } while (!bus.cpu.complete());
			// CPU clock runs slower than system clock, so it may be
			// complete for additional system clock cycles. Drain
			// those out
			do { bus.clock(); } while (bus.cpu.complete());
		}

		// Emulate one whole frame
		if (GetKey(olc::Key::F).bPressed)
		{
			// Clock enough times to draw a single frame
			do { bus.clock(); } while (!bus.ppu.frame_complete);
			// Use residual clock cycles to complete current instruction
			do { bus.clock(); } while (!bus.cpu.complete());
			// Reset frame completion flag
			bus.ppu.frame_complete = false;
		}
	}


	

	DrawCpu(516, 2); // draw the state of the cpu
	//DrawCode(516, 72, 26);// draw some disassembled code

	// Draw OAM Contents (first 26 out of 64) ======================================
	for (int i = 0; i < 26; i++)
	{
		std::string s = hex(i, 2) + ": (" + std::to_string(bus.ppu.pOAM[i * 4 + 3])
			+ ", " + std::to_string(bus.ppu.pOAM[i * 4 + 0]) + ") "
			+ "ID: " + hex(bus.ppu.pOAM[i * 4 + 1], 2) +
			+" AT: " + hex(bus.ppu.pOAM[i * 4 + 2], 2);
		DrawString(516, 72 + i * 10, s);
	}

	// Draw Palettes & Pattern Tables ==============================================
	const int nSwatchSize = 6;
	for (int p = 0; p < 8; p++) // For each palette
		for (int s = 0; s < 4; s++) // For each index
			FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340,
				nSwatchSize, nSwatchSize, bus.ppu.GetColourFromPaletteRam(p, s));

	// Draw selection reticule around selected palette
	DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);


	DrawSprite(516, 348, &bus.ppu.GetPatternTable(0, nSelectedPalette));
	DrawSprite(648, 348, &bus.ppu.GetPatternTable(1, nSelectedPalette));


	DrawSprite(0, 0, &bus.ppu.GetScreen(), 2);

	//olc::Sprite& s = bus.ppu.GetPatternTable(1, nSelectedPalette);
	//for (uint8_t y = 0; y < 30; y++)
	//{
	//	for (uint8_t x = 0; x < 32; x++)
	//	{
	//		//DrawString(x * 16, y * 16, hex((uint32_t)bus.ppu.vRam[0][y * 32 + x], 2));
	//		uint8_t id = (uint32_t)bus.ppu.vRam[0][y * 32 + x];
	//		DrawPartialSprite(x * 16, y * 16, &s,
	//			(id & 0x0F) << 3, ((id >> 4) & 0x0F) << 3, 8, 8, 2);
	//	}
	//}

	return true;
}


