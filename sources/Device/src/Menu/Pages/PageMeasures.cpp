#include "defines.h"
#include "FPGA/FPGA.h"
#include "Osci/FrequencyCounter.h"
#include "Display/Painter.h"
#include "Hardware/Sound.h"
#include "Menu/Pages/Definition.h"
#include "PageMeasures.h"
#include "Utils/Math.h"
#include "Utils/Measure.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pMeasures;
extern const PageBase ppTune;

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cIsShow,                                                                                          //--- ��������� - ���������� ---
    "����������", "Show",
    "�������� ��� �� �������� ��������� �� �����",
    "Output or output measurements on screen",
    "���", "No",
    "��",  "Yes",
    SHOW_MEASURES, pMeasures, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Number()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_7(       cNumber,                                                                                          //--- ��������� - ���������� ---
    "����������", "Number"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
    ,
    "Sets the maximum number of output measurements:\n"
    "\"1\" - one measurement\n"
    "\"2\" - two measurements\n"
    "\"1x5\" - 1 line with the five dimensions\n"
    "\"2x5\" - two rows with five measurements in each\n"
    "\"3x5\" - 3 lines with five measurements in each"
    "\"6x1\" - 6 lines, one in each dimension\n"
    "\"6x2\" - 6 lines of two dimensions in each\n",
    "1", "1",
    "2", "2",
    "1x5", "1x5",
    "2x5", "2x5",
    "3x5", "3x5",
    "6x1", "6x1",
    "6x2", "6x2",
    NUM_MEASURES, pMeasures, IsActive_Number, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Channels()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_3(       cChannels,                                                                                            //--- ��������� - ������ ---
    "������", "Channels",
    "�� ����� ������� �������� ���������",
    "Which channels to output measurement",
    "1",     "1",
    "2",     "2",
    "1 � 2", "1 and 2",
    SOURCE_MEASURES, pMeasures, IsActive_Channels, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Mode()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_2(       cMode,                                                                                                   //--- ��������� - ��� ---
    "���", "View",
    "��������� ��� ��� ���� ������ ������� ��� ���������� ���������� ��� ���������� ���������",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "��� ����",  "As is",
    "���������", "Reduce",
    MODE_VIEW_SIGNALS, pMeasures, IsActive_Mode, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bTune_Exit,                                                                            //--- ��������� - ��������� - ����� ---
    ppTune, FuncActive, OnPressSB_Exit, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Tune_Markers()
{
    Measure::ShortPressOnSmallButonMarker();
}

static void Draw_Tune_Markers(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x60');
    Painter::SetFont(Font::Type::_8);
}

DEF_SMALL_BUTTON(   bTune_Markers,                                                                            //--- ��������� - ��������� - ������ ---
    "������", "Marker",
    "��������� ���������� ������� ��� ����������� �������� ���������",
    "Allows to establish markers for visual control of measurements",
    ppTune, FuncActive, OnPress_Tune_Markers, Draw_Tune_Markers
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Tune_Settings()
{
    Measure::pageChoiceIsActive = !Measure::pageChoiceIsActive;
    if (Measure::pageChoiceIsActive)
    {
        Measure::posOnPageChoice = (int8)set.meas_measures[Measure::posActive];
    }
}

static void Draw_Tune_Settings(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x62');
    Painter::SetFont(Font::Type::_8);
}

DEF_SMALL_BUTTON(   bTune_Settings,                                                                        //--- ��������� - ��������� - ��������� ---
    "���������", "Setup",
    "��������� ������� ����������� ���������",
    "Allows to choose necessary measurements",
    ppTune, FuncActive, OnPress_Tune_Settings, Draw_Tune_Settings
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Tune()
{
    return SHOW_MEASURES;
}

static void OnRegSet_Tune(int angle)
{
    static const int8 step = 3;
    static int8 currentAngle = 0;
    currentAngle += (int8)angle;
    if (currentAngle < step && currentAngle > -step)
    {
        return;
    }
    if (Measure::pageChoiceIsActive)
    {
        Measure::posOnPageChoice += (int8)Math::Sign(currentAngle);
        Sound::RegulatorSwitchRotate();
        if (Measure::posOnPageChoice < 0)
        {
            Measure::posOnPageChoice = Measure::Type::Number - 1;
        }
        else if (Measure::posOnPageChoice == Measure::Type::Number)
        {
            Measure::posOnPageChoice = 0;
        }
        set.meas_measures[Measure::posActive] = (Measure::Type)Measure::posOnPageChoice;
        Painter::ResetFlash();
    }
    else
    {
        Measure::ChangeActive(currentAngle);

        Sound::RegulatorSwitchRotate();
    }
    currentAngle = 0;
}

DEF_PAGE_SB(ppTune,                                                                                               // ��������� - ��������� ///
    "���������", "CONFIGURE",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    "Transition to rezhm of exact control of quantity and types of measurements",
    &bTune_Exit,
    0,
    0,
    0,
    &bTune_Markers,
    &bTune_Settings,
    Page::Name::SB_Measures_Tune, &pMeasures, IsActive_Tune, EmptyPressPage, FuncDrawPage, OnRegSet_Tune
)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageMeasures::pointer = &pMeasures;

DEF_PAGE_5(         pMeasures,                                                                                                        // ��������� ///
    "���������", "MEASURES",
    "�������������� ���������",
    "Automatic measurements",
    &cIsShow,       // ��������� - ����������
    &cNumber,       // ��������� - ����������
    &cChannels,     // ��������� - ������
    &ppTune,        // ��������� - ���������
    &cMode,         // ��������� - ���
    Page::Name::Measure, Menu::pageMain, FuncActive, EmptyPressPage
)
