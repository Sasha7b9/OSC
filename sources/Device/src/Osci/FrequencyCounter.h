#pragma once
#include "defines.h"
#include "structs.h"


#define FREQ_METER_ENABLED              (set.freq_enabled)
#define FREQ_METER_IS_ENABLED           (FREQ_METER_ENABLED == FPGA::FreqMeter::Enabled::On)
#define FREQ_METER_TIMECOUNTING         (set.freq_timeCounting)
#define FREQ_METER_FREQ_CLC             (set.freq_freqClc)
#define FREQ_METER_NUM_PERIODS          (set.freq_numberPeriods)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FrequencyCounter
{
public:
    static void Update(uint8 flag);

    /// ����� ����� �������.
    struct TimeCounting
    {
        enum E
        {
            _100ms,
            _1s,
            _10s
        } value;
        operator uint8() const
        {
            return (uint8)value;
        }
    };

    /// ������� ����������� ��������� ��� ����� �������.
    struct FreqClc
    {
        enum E
        {
            _100kHz,
            _1MHz,
            _10MHz,
            _100MHz
        } value;
        operator uint8() const
        {
            return (uint8)value;
        }
    };

    /// ���������� ��������.
    struct NumberPeriods
    {
        enum E
        {
            _1,
            _10,
            _100
        } value;
        operator uint8() const
        {
            return (uint8)value;
        }
    };

    struct Enabled
    {
        enum E
        {
            On,
            Off
        } value;
        operator uint8() const
        {
            return (uint8)value;
        }
    };

private:

    static void ReadFreq();

    static void ReadPeriod();

    static float FreqSetToFreq(const BitSet32 *fr);

    static float PeriodSetToFreq(const BitSet32 *period);
    /// ����� �������� ��������� �������������� �������� �������. ��� ������ � ������ �����������
    static BitSet32 freqActual;
    /// ����� �������� ��������� �������������� �������� �������. ��� ������ � ������ �����������
    static BitSet32 freqSet;

    static BitSet32 periodSet;

    static BitSet32 periodActual;

    static bool drawFreq;
    
    static bool drawPeriod;
    /// ������������� � true �������� ��������, ��� ������� ����� ������� �� �������� �������
    static bool readPeriod;

    static float prevFreq;

    static float frequency;

    static float period;
};