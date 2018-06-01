#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AD9286
{
public:
    static void Init();

private:
    static void ConfigToWrite();
    static void ConfigToRead();
    static void SetPin(GPIO_TypeDef *gpio, uint16 pin);
    static void ResetPin(GPIO_TypeDef *gpio, uint16 pin);
    static int ReadPin(uint pin);
    static uint8 ReadByte(uint8 address);
    static void  WriteByte(uint8 address, uint8 byte);
};
