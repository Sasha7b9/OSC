#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Hex
{
public:
    Hex(uint v) : value(v) {};
    /// ����������� �������� � ��������� ������ � ����������������� ����. depth ����� ����������� ����� - 8, 16 ��� 32 ���.
    char* ToHex(int depth, char bufferOut[9]) const;
    /// ����������� �������� � ��������� ������ � �������� ����. depth ����� ����������� ����� - 8, 16 ��� 32 ���.
    char* ToBin(int depth, char bufferOut[36]) const;
    /// ���������� ������ � ������� pos �� ���������� (��������)
    char DigitInPosition(int pos) const;

    void Set(uint v) { value = v; };

    operator uint() const { return value; };
private:

    char *BinToString8(uint8 value, char buffer[9]) const;

    uint value;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Frequency
{
public:
    Frequency(float v) : value(v) {};

    char* ToString(char bufferOut[20]) const;
    /// ����������� freq ���� � ��������� ������. ��� ���� ����� ���������� ���� � ������ ����� numDigits
    char* ToStringAccuracy(char bufferOut[20], int numDigits) const;

private:
    float value;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Time
{
public:

    Time(float v) : value(v) {};
    /// ����������� ����� � ��������� ������
    char* ToString(bool alwaysSign,      ///< ���� true, ���� ������������ ���� ���� time > 0
                   char buffer[20]       ///< ���� ������������ ������������ ��������
    ) const;
    /// ����������� time ������ � ��������� ������
    char* ToStringAccuracy(bool alwaysSign,      ///< ���� true, ���� ������������ ���� ���� time > 0
                           char bufferOut[20],   ///< ���� ������������ ������������ ��������
                           int numDigits         ///< ���������� ���� � ������
    ) const;

private:

    float value;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Voltage
{
public:

    Voltage(float v) : value(v) {};

    char* ToString(bool alwaysSign, char buffer[20]) const;

private:

    float value;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Phase
{
public:

    Phase(float v) : value(v) {};

    char* ToString(char buffer[20]) const;

private:

    float value;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Float
{
public:

    Float(float v) : m_val(v) {};
    /// \brief ��������� ����� � ��������� ������ � ��������� ���. numDigits - ����� �������� ���������
    /// \attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ������������ ����.
    /// \retval ��������� �� ������ � ������.
    char *ToString(bool alwaysSign, int numDigits, char bufferOut[20]) const;

private:
    float m_val;
};
