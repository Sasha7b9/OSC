#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "TypeSymbols.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAKE_COLOR(r, g, b)     ((r << 16) | (g << 8) | b)


class Color
{
public:
    static const uint8 BACK         = 0x00;
    static const uint8 FILL         = 0x01;
    static const uint8 CHAN_A       = 0x02;
    static const uint8 CHAN_B       = 0x03;
    static const uint8 GRID         = 0x04;
    static const uint8 BLUE         = 0x05;
    static const uint8 GREEN        = 0x06;
    static const uint8 RED          = 0x07;
    static const uint8 CHAN_A_HALF  = 0x08;
    static const uint8 CHAN_B_HALF  = 0x09;

    static uint8 Chan(Channel ch);
    static uint8 ChanHalf(Channel ch);
};

typedef enum
{
    TypeFont_5,
    TypeFont_8,
    TypeFont_UGO,
    TypeFont_UGO2
} TypeFont;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Painter
{
public:
    static void BeginScene(uint8 color);

    static void EndScene();
    /// ���� �� ���������� �������� �����, �� ���� ��������� �������
    static void SetColor(uint8 color = 255);

    static void FillRegion(int x, int y, int width, int height, uint8 color = 255);

    static void DrawText(int x, int y, const char *text, uint8 color = 255);

    static void SetPalette(uint8 numColor, uint valueColor);

    static void DrawRectangle(int x, int y, int width, int height, uint8 color = 255);

    static void DrawFormatText(int x, int y, uint8 color, char *format, ...);

    static void DrawChar(int x, int y, char symbol, uint8 color = 255);

    static void DrawHLine(int y, int x0, int x1, uint8 color = 255);

    static void DrawHLine(float y, int x0, int x1, uint8 color = 255);

    static void DrawVLine(int x, int y0, int y1, uint8 color = 255);

    static void DrawVLine(int x, float y0, float y1, uint8 color = 255);

    static void DrawLine(int x0, int y0, int x1, int y1, uint8 color = 255);

    static void SetFont(TypeFont typeFont);

    static void SetPoint(int x, int y, uint8 color = 255);

    static void DrawBoundedRegion(int x, int y, int width, int height, uint8 colorFill, uint8 colorBound);

private:
};
