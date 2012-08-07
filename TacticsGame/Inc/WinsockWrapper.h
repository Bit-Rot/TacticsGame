#ifndef WINSOCKWRAPPER_H
#define WINSOCKWRAPPER_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#endif //#ifndef WINSOCKWRAPPER_H