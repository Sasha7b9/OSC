#include "defines.h"
#include "FrequencyCounter.h"
#include "FPGA/FPGATypes.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include "Hardware/FSMC.h"
#include "Menu/Pages/PageFunction.h"
#include "Utils/Stack.h"
#include "Utils/Math.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BitSet32 FrequencyCounter::freqActual;
BitSet32 FrequencyCounter::freqSet;
BitSet32 FrequencyCounter::periodActual;
BitSet32 FrequencyCounter::periodSet;
bool     FrequencyCounter::drawFreq;
bool     FrequencyCounter::drawPeriod;
bool     FrequencyCounter::readPeriod;
float    FrequencyCounter::prevFreq;
float    FrequencyCounter::frequency;
uint16   FrequencyCounter::flag;

//                         0    1    2    3    4    5    6 
static char buffer[11] = {'0', '0', '0', '0', '0', '0', '0', 0, 0, 0, 0};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FrequencyCounter::Init()
{
    drawFreq = false;
    drawPeriod = false;

    uint8 data = 0;

    if (FREQ_METER_IS_ENABLED)
    {
        const uint16 maskTime[3] = {0, 1, 2};
        const uint16 maskFreqClc[4] = {0, (1 << 2), (1 << 3), ((1 << 3) + (1 << 2))};
        const uint16 maskPeriods[3] = {0, (1 << 4), (1 << 5)};

        data |= maskTime[FREQ_METER_TIMECOUNTING];
        data |= maskFreqClc[FREQ_METER_FREQ_CLC];
        data |= maskPeriods[FREQ_METER_NUM_PERIODS];
    }
    else
    {
        _SET_BIT(data, 2);
    }

    FSMC::WriteToFPGA8(WR_FREQMETER, data);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::Update(uint16 flag_)
{
    flag = flag_;

    bool freqReady = _GET_BIT(flag, FL_FREQ_READY) == 1;
    bool periodReady = _GET_BIT(flag, FL_PERIOD_READY) == 1;

    if (freqReady)
    {
        freqActual.Set(*RD_FREQ_BYTE_3, *RD_FREQ_BYTE_2, *RD_FREQ_BYTE_1, *RD_FREQ_BYTE_0);
        
        drawFreq = true;

        if (!readPeriod)
        {
            ReadFreq();
        }
    }

    if (periodReady)
    {
        periodActual.Set(*RD_PERIOD_BYTE_3, *RD_PERIOD_BYTE_2, *RD_PERIOD_BYTE_1, *RD_PERIOD_BYTE_0);

        drawPeriod = true;

        if (readPeriod)
        {
            ReadPeriod();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::ReadFreq()
{
    freqSet.Set(*RD_FREQ_BYTE_3, *RD_FREQ_BYTE_2, *RD_FREQ_BYTE_1, *RD_FREQ_BYTE_0);

    if (freqSet.word < 1000)
    {
        readPeriod = true;
    }
    else
    {
        float fr = FreqSetToFreq(&freqSet);
        if (fr < prevFreq * 0.9f || fr > prevFreq * 1.1f)
        {
            frequency = ERROR_VALUE_FLOAT;
        }
        else
        {
            frequency = fr;
        }
        prevFreq = fr;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::ReadPeriod()
{
    periodSet.Set(*RD_PERIOD_BYTE_3, *RD_PERIOD_BYTE_2, *RD_PERIOD_BYTE_1, *RD_PERIOD_BYTE_0);

    float fr = PeriodSetToFreq(&periodSet);
    if (fr < prevFreq * 0.9f || fr > prevFreq * 1.1f)
    {
        frequency = ERROR_VALUE_FLOAT;
    }
    else
    {
        frequency = fr;
    }
    prevFreq = fr;
    readPeriod = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::FreqSetToFreq(const BitSet32 *fr)
{
    const float k[3] = {10.0f, 1.0f, 0.1f};
    return FREQ_METER_IS_ENABLED ? (fr->word * k[FREQ_METER_TIMECOUNTING]) : (fr->word * 10.0f);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::PeriodSetToFreq(const BitSet32 *period_)
{
    if (period_->word == 0)
    {
        return 0.0f;
    }

    const float k[4] = {10e4f, 10e5f, 10e6f, 10e7f};
    const float kP[3] = {1.0f, 10.0f, 100.0f};

    return FREQ_METER_IS_ENABLED ? (k[FREQ_METER_FREQ_CLC] * kP[FREQ_METER_NUM_PERIODS] / (float)period_->word) : (10e5f / (float)period_->word);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::GetFreq()
{
    return frequency;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::Draw()
{
#define EMPTY_STRING "---.---"
#define SIZE 4

    if (!FREQ_METER_IS_ENABLED)
    {
        return;
    }

    int width = 241;
    int height = 74;
    int x = Grid::Left() + (Grid::Width() - width) / 2;
    int y = Grid::Top() + (Grid::Height() / 2 - height) / 2;

    
    Painter::FillRegion(x + 1,   y + 1, width - 2, height - 2, Color::BACK);
    Painter::DrawRectangle(x,    y,     width,     height,     Color::Trig());

    x += 2;
    y += 2;

    Painter::DrawBigText(x + 2,  y + 1,         SIZE, "F", Choice::ColorMenuField(PageFrequencyCounter::GetChoiceTimeF()));
    Painter::DrawBigText(x + 2,  y + 10 * SIZE, SIZE, "T", Choice::ColorMenuField(PageFrequencyCounter::GetChoiceNumPeriods()));
    int dX = 7 * SIZE;
    Painter::DrawBigText(x + dX, y + 1,         SIZE, "=", Choice::ColorMenuField(PageFrequencyCounter::GetChoiceTimeF()));
    Painter::DrawBigText(x + dX, y + 10 * SIZE, SIZE, "=", Choice::ColorMenuField(PageFrequencyCounter::GetChoiceNumPeriods()));
    
    float freq = FreqSetToFreq(&freqActual);

    bool condFreq = _SET_BIT(flag, FL_OVERFLOW_FREQ) == 1 || drawFreq == false || freq == 0.0f;

    dX = SIZE * 12;

    Painter::DrawBigText(x + dX, y + 1, SIZE, condFreq ? EMPTY_STRING : FreqSetToString(&freqActual),
                         Choice::ColorMenuField(PageFrequencyCounter::GetChoiceTimeF()));

    bool condPeriod = _GET_BIT(flag, FL_OVERFLOW_PERIOD) == 1 || drawPeriod == false || freq == 0.0f;

    Painter::DrawBigText(x + dX, y + 10 * SIZE, SIZE, condPeriod ? EMPTY_STRING : PeriodSetToString(&periodActual),
                         Choice::ColorMenuField(PageFrequencyCounter::GetChoiceNumPeriods()));


    width = 50;
    height = 27;
    x = 50;
    y = 120;
    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x - 1, y - 1, width + 2, height + 2, Color::FILL);
    Painter::DrawFormatText(x + 4, y + 4, "%d", freqActual.word);
    Painter::DrawFormatText(x + 4, y + 15, "%d", periodActual.word);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString FrequencyCounter::PeriodSetToString(const BitSet32 *pr)
{
    Hex value(pr->word);

    while(value.NumDigits() > 6)
    {
        value.Set(value / 10);
    }

    /*
    +----------+-----------+------------+------------+------------+
    | 1��� 1�� | 100 ���   | 1 ���      | 10 ���     | 100 ���    |
    +----------+-----------+------------+------------+------------+
    | 1        |    1e2    |     1e3    |     1e4    |     1e5    | - ��������� ��������
    |          |   1.00 �� |   1.000 �� |  1.0000 �� | 1.00000 �� | - ������������� �� �������
    |          |    1e7    |     1e6    |     1e5    |     1e4    | - �� ����� ����� ��������, ����� �������� ���������� ���������� � ���������
    +----------+-----------+------------|------------+------------+   ��������
    | 10       |    1e3    |     1e4    |     1e5    |     1e6    |
    |          |  1.000 �� |  1.0000 �� | 1.00000 �� | 1.00000 �� |
    |          |    1e6    |     1e5    |     1e4    |     1e3    |
    +----------+-----------+------------+------------+------------+
    | 100      |    1e4    |     1e5    |     1e6    |     1e7    |
    |          | 1.0000 �� | 1.00000 �� | 1.00000 �� | 1.00000 �� |
    |          |    1e5    |     1e4    |     1e3    |     1e2    |
    +----------+-----------+------------+------------+------------+
    */

    /*
    static const uint64 k[FrequencyCounter::NumberPeriods::Number][FrequencyCounter::FreqClc::Number] =
    {
        {10 * 1000 * 1000, 1000 * 1000, 100 * 1000, 10 * 1000},
        {     1000 * 1000,  100 * 1000,  10 * 1000,      1000},
        {      100 * 1000,   10 * 1000,       1000,       100}
    };
    */

    // �������� ���������� ����������
    //    uint64 numPS = ticks * k[FREQ_METER_NUM_PERIODS][FREQ_METER_FREQ_CLC];

    /*
    ���������� �������� | ����������� �������� | ����� �� ����� | ��������� �������� ������

    +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
    |          | 100 ���                    0        | 1 ���                      1        | 10 ���                     2        | 100 ���                    3        |
    +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
    | 1        | 10   ���        1    0.01 ��  1e-5  | 1 ���          1      1. ���  1e-6  | 100 ��         1     0.1 ���  1e-7  | 10  ��         1    0.01 ���  1e-8  | 
    |          | 100  ���       10    0.10 ��  1e-5  | 10 ���        10     10. ���  1e-6  | 1   ���       10     1.0 ���  1e-7  | 100 ��        10    0.10 ���  1e-8  |
    |          | 1    ��       100    1.00 ��  1e-5  | 100 ���      100    100. ���  1e-6  | 10  ���      100    10.0 ���  1e-7  | 1   ���      100    1.00 ���  1e-8  |
    |          | 10   ��      1000   10.00 ��  1e-5  | 1   ��      1000   1.000 ��   1e-6  | 100 ���     1000   100.0 ���  1e-7  | 10  ���     1000   10.00 ���  1e-8  |
    |          | 100  ��     10000  100.00 ��  1e-5  | 10  ��     10000  10.000 ��   1e-6  | 1   ��     10000  1.0000 ��   1e-7  | 100 ���    10000  100.00 ���  1e-8  |
    |          | 1    �     100000 1.00000 �   1e-5  | 100 ��    100000 100.000 ��   1e-6  | 10  ��    100000 10.0000 ��   1e-7  | 1   ��    100000 1.00000 ��   1e-8  |
    |          | 10   �    100000- 10.0000 �   1e-4  | 1   �    100000- 1.00000 �    1e-5  | 100 ��   100000- 100.000 ��   1e-6  | 10  �c   100000- 10.0000 ��   1e-7  |
    |          | 100  �   100000-- 100.000 �   1e-3  | 10  �   100000-- 10.0000 �    1e-4  | 1   �   100000-- 1.00000 �    1e-5  | 100 ��  100000-- 100.000 ��   1e-6  |
    |        0 | 1000 �   ������������               | 100 �   ������������                | 10  �   ������������                | 1   c   ������������                |
    +----------+-------------------------------------+-------------------------------------|-------------------------------------+-------------------------------------+
    | 10       |                                     |                                     |                                     | 1   ��         1      1. ��   1e-9  |
    |          |                                     |                                     |                         /           | 10  ��        10     10. ��   1e-9  |
    |          |                                     |                                     |                     /               | 100 ��       100    100. ��   1e-9  |
    |          |                                     |                                     |                  /                  | 1   ���     1000   1.000 ���  1e-9  |
    |          |                                     |                                     |             /                       | 10  ���    10000  10.000 ���  1e-9  | 
    |          |                                     |                                     |         /                           | 100 ���   100000 100.000 ���  1e-9  | 
    |          |                                     |                                     |   |  /                              | 1   ��   100000- 1.00000 ��   1e-8  | 
    |          |                                     |                                     |   |---                              | 10  ��  100000-- 10.0000 ��   1e-7  |
    |        1 |                                     |                                     |                                     | 100 ��  ������������                |
    +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
    | 100      |                                     |                                     |                                     | 0.1 ��         1     0.1 ��   1e-10 |
    |          |                                     |                                     |                           /         | 1   ��        10     1.0 ��   1e-10 |
    |          |                                     |                                     |                       /             | 10  ��       100    10.0 ��   1e-10 |
    |          |                                     |                                     |                    /                | 100 ��      1000   100.0 ��   1e-10 |
    |          |                                     |                                     |               /                     | 1   ���    10000  1.0000 ���  1e-10 |
    |          |                                     |                                     |           /                         | 10  ���   100000 10.0000 ���  1e-10 |
    |          |                                     |                                     |     |  /                            | 100 ���  100000- 100.000 ���  1e-9  |
    |          |                                     |                                     |     |---                            | 1   ��  100000-- 1.00000 ��   1e-8  |
    |        2 |                                     |                                     |                                     | 10  ��  ������������                |
    +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
    */

    // ������� ��������� ��������� �� ����� ���������
    uint ticks = pr->word;

    for(int i = 0; i < 7; i++)
    {
        buffer[i] = value.DigitInPosition(6 - i);
    }

#define e3 (            1000)
#define e4 (       10 * 1000)
#define e5 (      100 * 1000)
#define e6 (     1000 * 1000)
#define e7 (10 * 1000 * 1000)

#define WRITE_SUFFIX(suffix)                        strcpy(buffer + 7, suffix);

#define WRITE_SUFFIX_2(value, suffix1, suffix2)     \
    if(ticks < value) WRITE_SUFFIX(suffix1)         \
    else              WRITE_SUFFIX(suffix2)

#define WRITE_SUFFIX_3(value1, value2, suffix1, suffix2, suffix3)   \
    if(ticks < value1)      WRITE_SUFFIX(suffix1)                   \
    else if(ticks < value2) WRITE_SUFFIX(suffix2)                   \
    else                    WRITE_SUFFIX(suffix3)

#define SET_POINT(pos)                              memcpy(buffer, buffer + 1, pos); buffer[pos] = '.';

#define CHOICE_4(v1, pos1, v2, pos2, v3, pos3, pos4) \
    if(ticks < v1)      { SET_POINT(pos1) }          \
    else if(ticks < v2) { SET_POINT(pos2) }          \
    else if(ticks < v3) { SET_POINT(pos3) }          \
    else                { SET_POINT(pos4) }

    switch(FREQ_METER_NUM_PERIODS + FREQ_METER_FREQ_CLC)
    {
        case 0:
            WRITE_SUFFIX_2(e5, "��", "�");
            CHOICE_4(e5, 4, e6, 1, e7, 2, 3);
            break;
        case 1:
            WRITE_SUFFIX_3(e3, e6, "���", "��", "c");
            CHOICE_4(e3, 6, e6, 3, e7, 1, 2);
            break;
        case 2:
            WRITE_SUFFIX_3(e4, e7, "���", "��", "�");
            CHOICE_4(e4, 5, e6, 2, e7, 3, 1);
            break;
        case 3:
            WRITE_SUFFIX_2(e5, "���", "��");
            CHOICE_4(e5, 4, e6, 1, e7, 2, 3);
            break;
        case 4:
            WRITE_SUFFIX_3(e3, e6, "��", "���", "��");
            CHOICE_4(e3, 6, e6, 3, e7, 1, 2);
            break;
        case 5:
            WRITE_SUFFIX_3(e4, e7, "��", "���", "��");
            CHOICE_4(e4, 5, e6, 2, e7, 3, 1);
            break;
    }

    return buffer;
}



//----------------------------------------------------------------------------------------------------------------------------------------------------
pString FrequencyCounter::FreqSetToString(const BitSet32 *fr)
{
    Hex value(fr->word);

    while(value.NumDigits() > 6)
    {
        value.Set(value / 10);
    }

    for(int i = 0; i < 7; i++)
    {
        buffer[i] = value.DigitInPosition(6 - i);
    }

    uint freq = fr->word;

    uint giverFreq = freq;

/// ��� ����� * 10
#define E_2 (               100)
#define E_3 (              1000)
#define E_4 (         10 * 1000)
#define E_5 (        100 * 1000)
#define E_6 (       1000 * 1000)
#define E_7 (  10 * 1000 * 1000)
#define E_8 ( 100 * 1000 * 1000)
#define E_9 (1000 * 1000 * 1000)


#undef WRITE_SUFFIX
#define WRITE_SUFFIX(suffix_E4)    \
    if(giverFreq < E_4) { strcpy(buffer + 7, suffix_E4); } else if (giverFreq < E_7) { strcpy(buffer + 7, "���"); } else { strcpy(buffer + 7, "���"); }

#define HIGH_FREQ                       \
    if(giverFreq < E_8)                 \
    {                                   \
        memcpy(buffer, buffer + 1, 2);  \
        buffer[1] = '.';                \
    }                                   \
    else if (giverFreq < E_9)           \
    {                                   \
        memcpy(buffer, buffer + 1, 3);  \
        buffer[2] = '.';                \
    }                                   \
    else                                \
    {                                   \
        memcpy(buffer, buffer + 1, 3);  \
        buffer[3] = '.';                \
    }


    switch (FREQ_METER_TIMECOUNTING)
    {
        case FrequencyCounter::TimeCounting::_100ms:

            giverFreq *= 100;

            WRITE_SUFFIX("���");

            if(giverFreq < E_7)                         // ������ 1 ���
            {
                if(freq >= E_2)                         // ������ ��� ����� 10 ��
                {
                    memcpy(buffer, buffer + 1, 5);
                }
                buffer[4] = '.';
            }
            else
            {
                HIGH_FREQ;
            }            
            break;

        case FrequencyCounter::TimeCounting::_1s:

            giverFreq *= 10;

            WRITE_SUFFIX("��");

            if (giverFreq < E_7)                        // ������ 1 ���
            {
                if(giverFreq < E_4)                     // ������ 1 ���
                {
                    memcpy(buffer, buffer + 1, 6);
                    buffer[6] = '.';
                }
                else
                {
                    memcpy(buffer, buffer + 1, 4);
                    buffer[3] = '.';
                }
            }
            else
            {
                HIGH_FREQ;
            }
            break;

        case FrequencyCounter::TimeCounting::_10s:

            WRITE_SUFFIX("��");

            if (freq < E_7)                       // ������ 1 ���
            {
                if (giverFreq < E_4)             // ������ 1 ���
                {
                    memcpy(buffer, buffer + 1, 5);
                    buffer[5] = '.';
                }
                else
                {
                    memcpy(buffer, buffer + 1, 3);
                    buffer[2] = '.';
                }
            }
            else
            {
                HIGH_FREQ;
            }
            break;
        default:
            break;
    }

    return buffer;
}
