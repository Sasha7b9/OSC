#pragma once

#define DEBUG

#ifdef MSVC
#define __attribute(x)
#endif

#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>

#define MODEL_RU "�����������-����������"
#define MODEL_EN "Oscilloscope-multimeter"
#define NUM_VER "0.0.1"


typedef const char  *const  pString;
typedef unsigned char       uchar;
typedef signed char         int8;
typedef unsigned char       uint8;
typedef signed short int    int16;
typedef unsigned short int  uint16;
typedef unsigned int        uint;
typedef uint64_t            uint64;


typedef void (*pFuncVV)();
typedef void (*pFuncVB)(bool);
typedef bool (*pFuncBV)();
typedef void (*pFuncVI)(int);
typedef void (*pFuncVII)(int, int);
typedef void (*pFuncVpIII)(int *, int, int);
typedef void (*pFuncVpI8I8I8)(int8 *, int8, int8);

inline void EmptyFuncVV(){}
inline void EmptyFuncVB(bool){}
inline bool EmptyFuncBV() { return true; }
inline void EmptyFuncVI(int) {}
inline void EmptyFuncVII(int, int) {}

#define ERROR_VALUE_INT     INT_MAX
#define ERROR_VALUE_INT16   SHRT_MAX
    

#ifndef _WIN32
#if __ARMCLIB_VERSION < 6070001
#pragma anon_unions
#endif
#endif


#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))
#define _GET_BIT(value, numBit) ((value >> numBit) & 1)
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << bit)))
#define _SET_BIT(value, bit) ((value) |= (1 << (bit)))


#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))


#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_STRING_VALUE  "--.--"
#define ERROR_VALUE_UINT8   255

#define MAX_UINT 0xffffffff


// ��� ����������� ������� ������� ����� ������������ ����� ���������, ����� ����� ����� ���� ���������, ��� ����� ������ ����������
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]


#define ENABLE_RU "���"
#define ENABLE_EN "On"

#define DISABLE_RU "����"
#define DISABLE_EN "Off"

#define SAFE_FREE(x) if(x) free(x); (x) = 0;
