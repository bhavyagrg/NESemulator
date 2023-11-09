#include "Application.h"
#include "LibGui.h"
#include "raylib.h"

Application::Application() {
}

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
        Gui::DrawText(sOffset.c_str(), nRamX, nRamY);
        nRamY += 20;
    }
}

void Application::DrawCpu(int x, int y)
{
    std::string status = "STATUS: ";
    Gui::DrawText(x, y, status, WHITE);
    Gui::DrawText(x + 68, y, "N", bus.cpu.status & CPU::N ? GREEN : RED);
    Gui::DrawText(x + 80, y, "V", bus.cpu.status & CPU::V ? GREEN : RED);
    Gui::DrawText(x + 96, y, "-", bus.cpu.status & CPU::U ? GREEN : RED);
    Gui::DrawText(x + 112, y, "B", bus.cpu.status & CPU::B ? GREEN : RED);
    Gui::DrawText(x + 128, y, "D", bus.cpu.status & CPU::D ? GREEN : RED);
    Gui::DrawText(x + 144, y, "I", bus.cpu.status & CPU::I ? GREEN : RED);
    Gui::DrawText(x + 160, y, "Z", bus.cpu.status & CPU::Z ? GREEN : RED);
    Gui::DrawText(x + 178, y, "C", bus.cpu.status & CPU::C ? GREEN : RED);
    Gui::DrawText(x, y + 20, "PC: $" + hex(bus.cpu.pc, 4));
    Gui::DrawText(x, y + 40, "A: $" + hex(bus.cpu.ac, 2) + "  [" + std::to_string(bus.cpu.ac) + "]");
    Gui::DrawText(x, y + 60, "X: $" + hex(bus.cpu.x, 2) + "  [" + std::to_string(bus.cpu.x) + "]");
    Gui::DrawText(x, y + 80, "Y: $" + hex(bus.cpu.y, 2) + "  [" + std::to_string(bus.cpu.y) + "]");
    Gui::DrawText(x, y + 100, "Stack P: $" + hex(bus.cpu.stkp, 4));
}

void Application::DrawCode(int x, int y, int nLines)
{
    auto it_a = mapAsm.find(bus.cpu.pc);
    int nLineY = (nLines >> 1) * 20 + y;
    if (it_a != mapAsm.end())
    {
        Gui::DrawText(x, nLineY, (*it_a).second, BLUE);
        while (nLineY < (nLines * 20) + y)
        {
            nLineY += 20;
            if (++it_a != mapAsm.end())
            {
                Gui::DrawText(x, nLineY, (*it_a).second);
            }
        }
    }

    it_a = mapAsm.find(bus.cpu.pc);
    nLineY = (nLines >> 1) * 20 + y;
    if (it_a != mapAsm.end())
    {
        while (nLineY > y)
        {
            nLineY -= 20;
            if (--it_a != mapAsm.end())
            {
                Gui::DrawText(x, nLineY, (*it_a).second);
            }
        }
    }
}

bool Application::OnUserCreate()
{

    Gui::Window::Init(780, 480);
    
    std::string nesFile = "./assets/super_mario_bros.nes"; 
    
    std::cout << "[+] loading " << nesFile << std::endl;

    cart = std::make_shared<Cartridge>(nesFile);

    if (!cart->ImageValid())
    {
        std::cout << "[-] " << nesFile << " is not valid" << std::endl;
	return false;
    }

	// Insert cartridge into bus
    bus.insertCartridge(cart);
    std::cout << "[+] cartridge inserted" << std::endl;
    
	// Extract dissassembly
    mapAsm = bus.cpu.disassemble(0x0000, 0xFFFF);
    std::cout << "[+] bus disassembled" << std::endl;
	
    // Reset NES
	bus.reset();
    
    std::cout << "[+] " << nesFile <<  " file loaded successfully" << std::endl;

	
    return true;
}

void Application::HandleInput(){

    bus.controller[0] = 0x00;
	bus.controller[0] |= IsKeyDown(KEY_X) ? 0x80 : 0x00;  // A button
	bus.controller[0] |= IsKeyDown(KEY_Z) ? 0x40 : 0x00; // B button
	bus.controller[0] |= IsKeyDown(KEY_A) ? 0x20 : 0x00; //Select
	bus.controller[0] |= IsKeyDown(KEY_S) ? 0x10 : 0x00; //Start
	bus.controller[0] |= IsKeyDown(KEY_UP) ? 0x08 : 0x00;
	bus.controller[0] |= IsKeyDown(KEY_DOWN) ? 0x04 : 0x00;
	bus.controller[0] |= IsKeyDown(KEY_LEFT) ? 0x02 : 0x00;
	bus.controller[0] |= IsKeyDown(KEY_RIGHT) ? 0x01 : 0x00;

    if (IsKeyReleased(KEY_SPACE)) bEmulationRun = !bEmulationRun;
    if (IsKeyReleased(KEY_R)) bus.reset();
    if (IsKeyReleased(KEY_F)) bIsFullScreen = !bIsFullScreen;
}

bool Application::OnUserUpdate(float fElapsedTime)
{
    HandleInput();

    if (bEmulationRun)
    {
        if (fResidualTime > 0.0f)
            fResidualTime -= fElapsedTime;
        else
        {
            fResidualTime += (1.0f / 60.0f) - fElapsedTime;
            do { bus.clock(); } while (!bus.ppu.frame_complete);
            bus.ppu.frame_complete = false;
        }
    }
    else
    {
        // Emulate code step-by-step
        if (IsKeyReleased(KEY_C))
        {
            // Clock enough times to execute a whole CPU instruction
            do { bus.clock(); } while (!bus.cpu.complete());
            // CPU clock runs slower than system clock, so it may be
            // complete for additional system clock cycles. Drain
            // those out
            do { bus.clock(); } while (bus.cpu.complete());
        }

        // Emulate one whole frame
        if (IsKeyReleased(KEY_F))
        {
            // Clock enough times to draw a single frame
            do { bus.clock(); } while (!bus.ppu.frame_complete);
            // Use residual clock cycles to complete current instruction
            do { bus.clock(); } while (!bus.cpu.complete());
            // Reset frame completion flag
            bus.ppu.frame_complete = false;
        }
    }

    DrawGame();

    return true;
}

void Application::DrawGame(){
    const int res = 256;
    const int rightContentWidth = bIsFullScreen ? 0 : 224;

    int W = GetRenderWidth();
    W -= rightContentWidth;
    int H = GetRenderHeight();
    int scale = 1;

    if(W <= H){
        scale = W / res;
    }else{
        scale = H / res;
    }
   
    if(!bIsFullScreen){
        int end = GetRenderWidth() - rightContentWidth;
        DrawCpu(end, 2);
        DrawCode(end, 120, 16);
    }
    int x = W - (scale * res);
    x /= 2;
    int y = H - (scale * res);
    y /= 2;
    // std::cout <<W << "x" <<  H << std::endl;
    // std::cout <<x << ", " <<  y << std::endl;
    bus.ppu.GetScreen().draw(x, y, scale);
}

void Application::Start() {
    OnUserCreate();

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        OnUserUpdate(GetFrameTime());

        EndDrawing();
    }
}
