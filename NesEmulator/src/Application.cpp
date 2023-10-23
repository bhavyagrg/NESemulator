#include "Application.h"
#include "LibGui.h"

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
            sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
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
    Gui::DrawText(x + 68, y, "N", nes.cpu.status & CPU::N ? GREEN : RED);
    Gui::DrawText(x + 80, y, "V", nes.cpu.status & CPU::V ? GREEN : RED);
    Gui::DrawText(x + 96, y, "-", nes.cpu.status & CPU::U ? GREEN : RED);
    Gui::DrawText(x + 112, y, "B", nes.cpu.status & CPU::B ? GREEN : RED);
    Gui::DrawText(x + 128, y, "D", nes.cpu.status & CPU::D ? GREEN : RED);
    Gui::DrawText(x + 144, y, "I", nes.cpu.status & CPU::I ? GREEN : RED);
    Gui::DrawText(x + 160, y, "Z", nes.cpu.status & CPU::Z ? GREEN : RED);
    Gui::DrawText(x + 178, y, "C", nes.cpu.status & CPU::C ? GREEN : RED);
    Gui::DrawText(x, y + 20, "PC: $" + hex(nes.cpu.pc, 4));
    Gui::DrawText(x, y + 40, "A: $" + hex(nes.cpu.ac, 2) + "  [" + std::to_string(nes.cpu.ac) + "]");
    Gui::DrawText(x, y + 60, "X: $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
    Gui::DrawText(x, y + 80, "Y: $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
    Gui::DrawText(x, y + 100, "Stack P: $" + hex(nes.cpu.stkp, 4));
}

void Application::DrawCode(int x, int y, int nLines)
{
    auto it_a = mapAsm.find(nes.cpu.pc);
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

    it_a = mapAsm.find(nes.cpu.pc);
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

    // Load the cartridge
    cart = std::make_shared<Cartridge>("assets/SuperMarioBros.nes");

    // Insert into NES
    nes.insertCartridge(cart);

    // Extract dissassembly
    mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

    // Reset NES
    nes.reset();

    return true;
}

bool Application::OnUserUpdate(float fElapsedTime)
{
    //Clear(BLACK);
    if (bEmulationRun)
    {
        if (fResidualTime > 0.0f)
            fResidualTime -= fElapsedTime;
        else
        {
            fResidualTime += (1.0f / 60.0f) - fElapsedTime;
            do { nes.clock(); } while (!nes.ppu.frame_complete);
            nes.ppu.frame_complete = false;
        }
    }
    else
    {
        // Emulate code step-by-step
        if (IsKeyReleased(KEY_C))
        {
            // Clock enough times to execute a whole CPU instruction
            do { nes.clock(); } while (!nes.cpu.complete());
            // CPU clock runs slower than system clock, so it may be
            // complete for additional system clock cycles. Drain
            // those out
            do { nes.clock(); } while (nes.cpu.complete());
        }

        // Emulate one whole frame
        if (IsKeyReleased(KEY_F))
        {
            // Clock enough times to draw a single frame
            do { nes.clock(); } while (!nes.ppu.frame_complete);
            // Use residual clock cycles to complete current instruction
            do { nes.clock(); } while (!nes.cpu.complete());
            // Reset frame completion flag
            nes.ppu.frame_complete = false;
        }
    }

    if (IsKeyReleased(KEY_SPACE)) bEmulationRun = !bEmulationRun;
    if (IsKeyReleased(KEY_R)) nes.reset();


    int scale = GetRenderWidth() / 256;
    scale -= 1;

    int end = 256 * scale + 8;
    //end = 516;
    DrawCpu(end, 2);
    DrawCode(end, 120, 16);


    nes.ppu.GetScreen().draw(0, 0, scale);

    return true;
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

