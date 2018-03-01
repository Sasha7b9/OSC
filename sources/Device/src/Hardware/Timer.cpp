#include "Timer.h"
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer::Init()
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    TIM_HandleTypeDef handleTIM2 =
    {
        TIM2,
        {
            0,
            TIM_COUNTERMODE_UP,
            (uint)-1,
            TIM_CLOCKDIVISION_DIV1
        }
    };
    HAL_TIM_Base_Init(&handleTIM2);
    HAL_TIM_Base_Start(&handleTIM2);
}
