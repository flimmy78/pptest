#ifndef GW_ERROR_H
#define GW_ERROR_H

/**
 * If you want to include gwerror.h, include gw.h.
 */
#if 1
enum GW_ERROR_CODE
{
	eERROR_UNKNOWN 				= 0x80000000,
	eERROR_INVALID_ARGS			= 0x80000001,
	eERROR_OUT_OF_RANGE			= 0x80000002,
	eERROR_NOT_FOUND			= 0x80000003,
	eERROR_ALREADY_EXIST		= 0x80000004,
};
#endif

#endif
