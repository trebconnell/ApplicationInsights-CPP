#pragma once

#include <codecvt>
#include <locale>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <type_traits>
#include <stdint.h>
#include <ctime>
#include <wchar.h>
#include <sys/timeb.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(WINAPI_FAMILY_PARTITION) // it's SOME kind of Windows

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) // Win32, no store, no phone
#include <winhttp.h>
#else
#include <ppltasks.h>
#endif

#else // Everything else - OS X, Linux, Droid; use Curl
#include <curl/curl.h>
#endif
