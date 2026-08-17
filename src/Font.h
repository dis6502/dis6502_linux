#pragma once


#ifdef WIN32
#include <Windows.h>

#else

#ifndef HFONT
using HFONT = void*;
#endif

#endif

