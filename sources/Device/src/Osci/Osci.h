#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Osci
{
public:
    class Graphics
    {
    public:

        static void Update();

    private:

        static void DrawCursorTrigLevel();

        static void DrawScaleLine(int x, bool forTrigLev);

        static void DrawMeasures();
    };
private:
};
