#pragma once

#ifdef TGE_WINDOWS
#ifdef DYNAMIC_TGE_BUILD
#ifdef TGE_DLL_BUILD
#define TGE_API __declspec(dllexport)
#else
#define TGE_API __declspec(dllimport)
#endif
#else
#define TGE_API
#endif
#else
#error Only windows supported
#endif