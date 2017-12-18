#ifndef __NOMAD_H__
#define __NOMAD_H__

#ifdef WIN32

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <crtdbg.h>
#include <tchar.h>

#endif // #ifdef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "nomad_base.h"
#include "nomad_errors.h"
#include "nomad_windef.h"

// To make it easy to port.
#include "gw.h"

#endif // __NOMAD_H__