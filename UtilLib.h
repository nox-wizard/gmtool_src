#ifndef __UTIL_LIB_H_
#define __UTIL_LIB_H_

#ifdef UTILLIB_EXPORTS
#	define UTILAPI __declspec(dllexport)
# else
#	define UTILAPI __declspec(dllimport)
#endif

HWND UTILAPI FindUOWindow( void );
bool UTILAPI InstallMaxMinHook( HWND hNotify, HWND hWatch, UINT ActivateMsg, UINT FocusMsg );
bool UTILAPI RemoveMaxMinHook( void );

#endif
