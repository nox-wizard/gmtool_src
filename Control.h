#ifndef __CONTROL_H_
#define __CONTROL_H_

class Container;
	class Tab;
	class Dialog;

class Control;
	class Button;
	class Label;
	class Frame;
	class ListBox;

class ListObject;
	class ListMenu;
	class ListEntry;


struct BatchVar { CString Name; int ValCount; CString Vals[8]; };

////////////////////////////Container/////////////////////////////////
class Container
{
public:
	Container( const char * );
	~Container();

	Control *First() { CurPos = 0; return !End() ? Controls[CurPos] : NULL; }
	Control *Next( void ) {	CurPos++; return !End() ? Controls[CurPos] : NULL; }
	bool End( void ) const { return ( CurPos >= Controls.size() ); }
	char *GetName( void ) { return szName; }

protected:
	char szName[128];
	int CurPos;

	vector<Control *> Controls;
};

////////////////////////////Tab Container/////////////////////////////
class Tab : public Container
{
public:
	Tab( const char *Name ) : Container( Name ) {}

	void Load( ifstream &, CProgressCtrl &, int );

	void ShowControls();
	void HideControls();
};

////////////////////////////Custom Control Types//////////////////////
enum CONTROL_TYPE
{
	LABEL = 0,
	BUTTON,
	FRAME,
	LISTBOX
};

////////////////////////////Control Base Class////////////////////////
class Control
{
public:
	Control();
	~Control();
	CONTROL_TYPE type;

	int x1,y1,x2,y2;
	WORD id;
	CWnd *pObject;
};

////////////////////////////Button Class//////////////////////////////
class Button : public Control
{
public:
	Button() { type = BUTTON; pPrompt = NULL; szText[0] = szCommand[0] = szFmt[0] = szLink[0] = 0; }
	~Button();

	void OnPress( Container *, CWnd *, BatchVar *p = NULL, int = 0  );
	void ExecuteBatch( Container *, CWnd *);

	char szText[128];
	char szCommand[64];

	char szFmt[10];
	char szLink[32];

	char *pPrompt;
};

////////////////////////////Label Class///////////////////////////////
class Label : public Control
{
public:
	Label() { type = LABEL; szText[0] = 0; }
	char szText[128];
};

////////////////////////////ListBox Class/////////////////////////////
class ListBox : public Control
{
public:
	ListBox() { type = LISTBOX; pList = NULL; pCurrent = NULL; pPrompt = NULL; szName[0] = szFmt[0] = 0; }
	~ListBox();
	
	void Load( const char * );
	void InitList( void );
	void OnDoubleClick( CWnd *, Container * );

	unsigned short GetArtID( int );

//	signed int GetValue( void );
	CString GetString( char * );

	char szName[128];
	char szFmt[16];
	char *pPrompt;

	ListMenu *pList;
	ListMenu *pCurrent;
};

////////////////////////////Frame Class///////////////////////////////
class Frame : public Control
{
public:
	Frame() { type = FRAME; szText[0] = 0; }

	char szText[128];
};


////////////////////////////List Box helper classes///////////////////
enum LIST_OJECT_TYPE { MENU, VALUE };

class ListObject
{
public:
	ListObject() { szName[0]=0; ArtID = 0xFFFF; }
	~ListObject() {}

	LIST_OJECT_TYPE type;
	char szName[128];
	unsigned short ArtID;
};

class ListMenu : public ListObject
{
public:
	ListMenu() { pParent = NULL; type=MENU; }
	~ListMenu();

	void Load( ifstream &, const char *, int & );

	ListMenu *pParent;
	vector<ListObject *> Entries;
};

class ListEntry : public ListObject
{
public:
	ListEntry() { szValue[0]=0; type=VALUE; szValue[0] = 0; }
	~ListEntry() {}
	char szValue[64];
};

////////////////////////////Custom Dialog Class///////////////////////
class Dialog : public Container
{
public:
	Dialog();

	int getX( const char *word ) { int i = atoi(word); return (i*hspace)+((i-1)*col); }
	int getY( const char *word ) { int i = atoi(word); return (i*vspace)+((i-1)*row); }
	int getXSize( const char *word ) { int i = atoi(word); return ((i-1)*hspace)+(i*col); }
	int getYSize( const char *word ) { int i = atoi(word); return ((i-1)*vspace)+(i*row); }
	void Load( const char * );

	char szFile[32];

	CSize MySize;
	int hspace, vspace, col,row;
};



//////////////////////////////////////////////////////////////////////
//////////////////////////////EOF/////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#endif //__CONTROL_H_
