//////////////////////////////////////////////////////////////////////////
//
// [11/13/2013 by kwanghyun won]
// 

#ifndef _BASIC_TYPES_H_
#define _BASIC_TYPES_H_

#include <stddef.h>         // For size_t
#include <cfloat>
#include "base/nml_defines.h"

typedef signed char         schar;
typedef signed char         int8;
typedef short               int16;
typedef int					int32;
typedef long long			int64;

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

const uint8  kuint8Max  = (( uint8) 0xFF);
const uint16 kuint16Max = ((uint16) 0xFFFF);
const uint32 kuint32Max = ((uint32) 0xFFFFFFFF);
const uint64 kuint64Max = ((uint64) NM_LONGLONG(0xFFFFFFFFFFFFFFFF));
const  int8  kint8Min   = ((  int8) 0x80);
const  int8  kint8Max   = ((  int8) 0x7F);
const  int16 kint16Min  = (( int16) 0x8000);
const  int16 kint16Max  = (( int16) 0x7FFF);
const  int32 kint32Min  = (( int32) 0x80000000);
const  int32 kint32Max  = (( int32) 0x7FFFFFFF);
const  int64 kint64Min  = (( int64) NM_LONGLONG(0x8000000000000000));
const  int64 kint64Max  = (( int64) NM_LONGLONG(0x7FFFFFFFFFFFFFFF));

const  int64 kNoTimestamp = kint64Min;
const double kNoPcrTime = 1.0;
#endif //_NCNF_BASIC_TYPES_H_



