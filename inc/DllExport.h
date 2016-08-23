#pragma once

#ifdef MYTHREADPOOL_EXPORTS
#define MYTHREADPOOL_DLL __declspec(dllexport)
#else
#define MYTHREADPOOL_DLL __declspec(dllimport)
#endif // MYTHREADPOOL_EXPORTS
