#ifndef __ACCOMPLICE_H_
#define __ACCOMPLICE_H_

#pragma pack(1)

////////////////////////////Macros////////////////////////////////////
#define BASE_X 18
#define BASE_Y 0

#define SPLASH_MIN 0
#define SPLASH_MAX 100
#define SPLASH_PS 33

#define SPLASH_STEP2 SPLASH_PS
#define SPLASH_STEP3 SPLASH_PS*2


#define Color16to32(c16) (((c16) & 0x7C00) >> 7) | (((c16) & 0x3E0) << 6) | (((c16) & 0x1F) << 19)

enum USER_MESSAGE
{
	WM_CLIENTSHOW = WM_USER+17,
	WM_CLIENTFOCUS,
	WM_SYSTRAY,
};

////////////////////////////Global Includes///////////////////////////
#include "Resource.h"
#include "Art.h"
#include "MapNav.h"
#include "Control.h"
#include "Splash.h"
#include "PreviewList.h"
#include "MainDialog.h"
#include "Prompt.h"
#include "Options.h"
#include "GenericDialog.h"
#include "UtilLib.h"
#include "Library.h"

////////////////////////////Classes/Structures////////////////////////
class CThisApp : public CWinApp
{
public:
	BOOL InitInstance( void );
	int ExitInstance( void );

	bool LoadTabs( void );
};

////////////////////////////Global Functions//////////////////////////
void PostCommand( const char *, ... );
LRESULT CALLBACK NewClientProc( HWND, UINT, WPARAM, LPARAM );
void TrimDir( char * );

////////////////////////////Global Varibles///////////////////////////
extern CThisApp			theApp;
extern CMainDialog		*pAccomplice;
extern CArtMul			*pArt;
extern CMapNav			*pNav;
extern CSplash			*pSplash;
extern COptions			*pOptions;
extern vector<Tab *>	Tabs;
extern int				Col, Row, HSpace, VSpace;
extern CString			CmdPrefix;
extern CString			csFontFace;
extern int				FontSize;
extern bool				Horiz;
extern BOOL				PrefixHex;
extern BOOL				AlwaysOnTop;
extern BOOL				Link2UO;
extern CPoint			Mouse;
extern CSize			Screen;

#endif //__ACCOMPLICE_H_

