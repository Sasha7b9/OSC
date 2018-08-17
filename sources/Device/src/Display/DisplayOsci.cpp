#include "DisplayOsci.h"
#include "Painter.h"
#include "Grid.h"
#include "BottomPart.h"
#include "HiPart.h"
#include "RShift.h"
#include "PainterData.h"
#include "Console.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGATypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DELTA 5


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayOsci::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();
    
    PainterData::DrawData();

    Painter::DrawRectangle(Grid::Left(), Grid::Top(), Grid::Width(), Grid::Height(), Color::FILL);

    BottomPart::Draw();

    RShift::Draw();

    DrawCursorTrigLevel();

    PainterData::DrawData();

    HiPart::Draw();

    Console::Draw();

    Menu::Draw();

    Painter::EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DisplayOsci::DrawCursorTrigLevel()
{
    TrigSource ch = TRIG_SOURCE;
    int trigLev = SET_TRIGLEV_SOURCE + (TRIG_SOURCE_IS_EXT ? 0 : (SET_RSHIFT(ch) - RShift::ZERO));
    float scale = 1.0f / ((TrigLev::MAX - TrigLev::MIN) / 2.4f / Grid::Height());
    int y0 = (Grid::Top() + Grid::Bottom()) / 2 + (int)(scale * (TrigLev::ZERO - TrigLev::MIN));
    int y = y0 - (int)(scale * (trigLev - TrigLev::MIN));

    if (!TRIG_SOURCE_IS_EXT)
    {
        y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();
    }

    int x = Grid::Right();
    Painter::SetColor(Color::Trig());

    if (y > Grid::Bottom())
    {
        Painter::DrawChar(x + 3, Grid::Bottom() - 11, SYMBOL_TRIG_LEV_LOWER);
        Painter::SetPoint(x + 5, Grid::Bottom() - 2);
        y = Grid::Bottom() - 7;
        x--;
    }
    else if (y < Grid::Top())
    {
        Painter::DrawChar(x + 3, Grid::Top() + 2, SYMBOL_TRIG_LEV_ABOVE);
        Painter::SetPoint(x + 5, Grid::Top() + 2);
    }
    else
    {
        Painter::DrawChar(x + 1, y - 4, SYMBOL_TRIG_LEV_NORMAL);
    }

    Painter::SetFont(TypeFont_5);

    const char symbols[3] = {'1', '2', '�'};
    int dY = 0;

    Painter::DrawChar(x + 5, y - 9 + dY, symbols[(uint8)TRIG_SOURCE], Color::BACK);
    Painter::SetFont(TypeFont_8);

    DrawScaleLine(Display::WIDTH - 11, true);
    int left = Grid::Right() + 9;
    int height = Grid::Height() - 2 * DELTA;
    int shiftFullMin = RShift::MIN + TrigLev::MIN;
    int shiftFullMax = RShift::MAX + TrigLev::MAX;
    scale = (float)height / (shiftFullMax - shiftFullMin);
    int shiftFull = SET_TRIGLEV_SOURCE + (TRIG_SOURCE_IS_EXT ? 0 : SET_RSHIFT(ch));
    int yFull = Grid::Top() + DELTA + height - (int)(scale * (shiftFull - RShift::MIN - TrigLev::MIN) + 4);
    Painter::FillRegion(left + 2, yFull + 1, 4, 6, Color::Trig());
    Painter::SetFont(TypeFont_5);
    Painter::DrawChar(left + 3, yFull - 5 + dY, symbols[(uint8)TRIG_SOURCE], Color::BACK);
    Painter::SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DisplayOsci::DrawScaleLine(int x, bool forTrigLev)
{
    int width = 6;
    int topY = Grid::Top() + DELTA;
    int x2 = width + x + 2;
    int bottomY = Grid::Bottom() - DELTA;
    int centerY = (Grid::Bottom() + Grid::Top()) / 2;
    int levels[] =
    {
        topY,
        bottomY,
        centerY,
        centerY - (bottomY - topY) / (forTrigLev ? 8 : 4),
        centerY + (bottomY - topY) / (forTrigLev ? 8 : 4)
    };

    for (int i = 0; i < 5; i++)
    {
        Painter::DrawLine(x + 1, levels[i], x2 - 1, levels[i], Color::FILL);
    }
}
