#pragma once
#include "Settings/SettingsChannel.h"
#include "Data/DataSettings.h"
#include "Settings/SettingsMemory.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup FPGA
 *  @{
 *  @defgroup Data
 *  @{
 *  @defgroup Interface ���������
 *  @{
 */

#ifndef _INCLUDE_DATA_
#define EXTERN extern
#else
#define EXTERN
#endif

/** @defgroup DataBuffer
 *  @{
 */

extern uint8 *dataIN[Chan::Number];  ///< ��������� ������ ������� ������
extern uint8 *dataOUT[Chan::Number]; ///< ��������� ������ ������� ������

#define IN(ch)  (dataIN[ch])
#define IN_A    IN(Chan::A)
#define IN_B    IN(Chan::B)
#define OUT(ch) (dataOUT[ch])
#define OUT_A   OUT(Chan::A)
#define OUT_B   OUT(Chan::B)

/// \brief ��� ����������� ���������. ������������ ��� ��������� ������ ����������, ������� �� ����� �� ����� ��������� ���������, 
/// �� ����� ������ ��� � ����� �������. ����� �������������� � ������� ������ malloc() ���������� ����������� ���������� ������, 
/// ������� ����� ������������� ������ free()
extern void *extraMEM;

#define MALLOC_EXTRAMEM(NameStruct, name)   extraMEM = malloc(sizeof(NameStruct));    \
                                            NameStruct *name = (NameStruct*)extraMEM
#define ACCESS_EXTRAMEM(NameStruct, name)   NameStruct *name = (NameStruct*)extraMEM
#define FREE_EXTRAMEM()                     free(extraMEM);

/** @}
*/

#define DS          pDS             ///< ��������� �� ��������� �������� ��������� �������.

EXTERN DataSettings *pDS;           ///< ��������� �� ��������� �������� ��������� �������. ���������� � ���� ������� ����� ������ DS.

#undef EXTERN
/** @defgroup GlobalData Global Data
 *  @brief ���������, ����������� ��� ������������� � ������ ������ �������
 *  @{
 */

#define TSHIFT_DS       (TSHIFT(DS))
#define TBASE_DS        (TBASE(DS))
#define INVERSE_DS(ch)  (INVERSE(DS, ch))
#define COUPLE_DS(ch)   (COUPLE(DS, ch))
#define DIVIDER_DS(ch)  (DIVIDER(DS, ch))
#define RANGE_DS(ch)    (RANGE(DS, ch))
#define RANGE_DS_A      (RANGE(DS, Chan::A))
#define RANGE_DS_B      (RANGE(DS, Chan::B))
#define ENABLED_DS(ch)  (ENABLED(DS, ch))
#define ENABLED_DS_A    (ENABLED(DS, Chan(Chan::A)))
#define ENABLED_DS_B    (ENABLED(DS, Chan(Chan::B)))
#define RSHIFT_DS(ch)   (RSHIFT(DS, ch))
#define RSHIFT_DS_A     (RSHIFT(DS, Chan::A))
#define RSHIFT_DS_B     (RSHIFT(DS, Chan::B))
#define PEAKDET_DS      (PEAKDET(DS))
#define TRIGLEV_DS(ch)  (TRIGLEV(DS))
#define ENUM_POINTS_DS  (ENUM_POINTS(DS))
#define ENUM_BYTES_DS   (ENUM_BYTES(DS))
#define NUM_BYTES_DS    (NUM_BYTES(DS))

#define TIME_TIME_DS    (TIME_TIME(DS))
#define TIME_DAY_DS     (TIME_DAY(DS))
#define TIME_HOURS_DS   (TIME_HOURS(DS))
#define TIME_MINUTES_DS (TIME_MINUTES(DS))
#define TIME_SECONDS_DS (TIME_SECONDS(DS))
#define TIME_MONTH_DS   (TIME_MONTH(DS))
#define TIME_YEAR_DS    (TIME_YEAR(DS))

/** @}
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// � ���� ��������� ����� ��������� �����, �������������� ��� ������ �� �����
typedef struct
{
    uint8    data[Chan::Number][281 * 2];    ///< ������ ����� �������. ����� � ��� ���� ������, ��� �� ������, ��� �������� ���������
    bool     needDraw[Chan::Number];         ///< ���� true, �� ����� 1 ���� ��������
    uint8    notUsed0[2];
    int      posBreak;                      ///< ������� � ����������� �����, �� ������� ����� �������� ����� ������� ����������� ������
    ModeWork forMode;                       ///< ��� ������ ������ ����������� ������
    uint8    notUsed1[3];
} StructDataDrawing;

class Reader
{
public:
    /// \brief ������ ������ �� ���, fromEnd c ����� (fromEnd == 0 - ��������� ��������� ������) � ��������� ������� inA(B), outA(B), DS.
    /// forMemoryWindow ����� ��� ����, ����� � ������ ������ �������� ������ ������� ��� ������ � ���� ������
    static void ReadFromRAM(int fromEnd, StructDataDrawing *dataStruct, bool forMemoryWindow);
    /// ������ ������ �� EEPROM. ����� ������� - ��������� NUM_ROM_SIGNAL � ��������� ������� inA(B), outA(B), DS.
    static bool ReadFromROM(StructDataDrawing *dataStruct);
    /// ������ ������, �������������� ������ �����
    static void ReadMin(StructDataDrawing *dataStruct);
    /// ������ ������, �������������� ������ ������
    static void ReadMax(StructDataDrawing *dataStruct);
};


/** @}  @}  @}
 */
