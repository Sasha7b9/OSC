﻿#pragma once
#include "SettingsChannel.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsTrig Settings Trig
 *  @{
 */

 /// Источник синхронизации
struct TrigSource
{
    enum E
    {
        A,    /// Канал 1
        B,    /// Канал 2
        Ext   /// Внешняя
    } value;
    TrigSource(E v = A) : value(v) {};
    operator uint8() const { return (uint8)value; }
};

#define TRIG_SOURCE             (set.trig_source)
#define TRIG_SOURCE_IS_A        (TRIG_SOURCE == TrigSource::A)
#define TRIG_SOURCE_IS_B        (TRIG_SOURCE == TrigSource::B)
#define TRIG_SOURCE_IS_EXT      (TRIG_SOURCE == TrigSource::Ext)

#define SET_TRIGLEV(ch)         (set.trig_lev[(uint8)ch])
#define SET_TRIGLEV_SOURCE      SET_TRIGLEV(TRIG_SOURCE.value)
#define SET_TRIGLEV_A           (SET_TRIGLEV(Chan::A))
#define SET_TRIGLEV_B           (SET_TRIGLEV(Chan::B))

#define TRIG_MODE_FIND          (set.trig_modeFind)
#define TRIG_MODE_FIND_HAND     (TRIG_MODE_FIND == TrigModeFind::Hand)
#define TRIG_MODE_FIND_AUTO     (TRIG_MODE_FIND == TrigModeFind::Auto)

#define START_MODE              (set.trig_startMode)
#define START_MODE_IS_AUTO      (START_MODE == StartMode::Auto)
#define START_MODE_IS_WAIT      (START_MODE == StartMode::Wait)
#define START_MODE_IS_SINGLE    (START_MODE == StartMode::Single)

#define TRIG_POLARITY           (set.trig_polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig_input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig_TimeDelay)

/** @}  @}
 */
