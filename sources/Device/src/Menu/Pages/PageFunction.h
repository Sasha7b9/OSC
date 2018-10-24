#pragma once
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PageFunction
{
public:
    static const PageBase *pointer;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PageFrequencyCounter
{

    /// \todo ����� ���������� ��������. ������ ���������� �� ������������ ��������

public:
    static const PageBase *pointer;
    /// ���������� ��������� �� ��������� - ���������� - ����� ����� F
    static const Choice *GetChoiceTimeF();
    /// ���������� ��������� �� ��������� - ���������� - ����� �������
    static const Choice *GetChoiceFreqClc();
    /// ���������� ��������� �� ��������� - ���������� - ���-�� ��������
    static const Choice *GetChoiceNumPeriods();
};
