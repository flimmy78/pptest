#ifndef GW_H
#define GW_H

/**
 * This is the base header for all other gw series.
 * DON'T include class and function header files here.
 */

#ifndef GW_NAMESPACE_NAME

#define GW_NAMESPACE_NAME GW
#define GW_NAMESPACE_BEGIN namespace GW_NAMESPACE_NAME {
#define GW_NAMESPACE_END }

#define GW_NAMESPACE_USE using namespace GW_NAMESPACE_NAME;
#define USE_GW_NAMESPACE using namespace GW_NAMESPACE_NAME;

#endif

// extern c
#if defined(__cplusplus)

#if !defined(EXTERN_C_BEGIN)

#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }

#else

#define EXTERN_C_BEGIN
#define EXTERN_C_END

#endif

#endif

#include "gwport.h"

GW_NAMESPACE_BEGIN

#include "gwerror.h"

GW_NAMESPACE_END

#endif