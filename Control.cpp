/*
Accomplice GM Tool Version 2.XX - GM Tool for Ultima Online Emulators.
Copyright (C) 2001 Bryan "Zippy" Pass
	E-Mail: zippy@uoxdev.com or "Zippy-" on irc.stratics.com #uox
		Contact me electronicly reguarding written contact.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by 
the Free Software Foundation; either version 2 of the License, or (at 
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program; if not, write to:
Free Software Foundation, Inc., 
59 Temple Place, Suite 330
Boston, MA 02111-1307 USA
*/

#include "StdAfx.h"
#include "Accomplice.h"
#include "Control.h"

inline int getX( const char *word ) { int i = atoi(word); return BASE_X+(i*HSpace)+((i-1)*Col); }
inline int getY( const char *word ) { int i = atoi(word); return BASE_Y+(i*VSpace)+((i-1)*Row); }
inline int getXSize( const char *word ) { int i = atoi(word); return ((i-1)*HSpace)+(i*Col); }
inline int getYSize( const char *word ) { int i = atoi(word); return ((i-1)*VSpace)+(i*Row); }

void Fix( char *Line )
{//convert spaces outside of quotes to commas for strtok
	int l = strlen(Line);
	int i;

	for (i=0;i<l;i++)
	{
		if ( Line[i] == '\"' )
		{
			do {
				i++;
				if ( Line[i] == ',' || Line[i] == '=' || Line[i] == ';' )
					Line[i] = '_';
			} while (i<l && Line[i]!='\"');
			continue;
		} else if ( Line[i] == ' ' )
		{
			Line[i] = ',';
		}
	}
}

int HexNumber( const char *str )
{
	int ret;
	strstream strm;

	strm << str;
	strm >> hex >> ret >> dec;

	return ret;
}

void OffsetVal( const char *off, CString &Val )
{
	int v;

	if ( off[0] == '_' ) return;

	if ( Val[0] == '\x1b' )
	{
		v = HexNumber( &((LPCSTR)Val)[1] ) + atoi( off );
		Val.Format( "\x1b%x", v );
	} else {
		v = atoi( Val ) + atoi( off );
		Val.Format( "%i", v );
	}
}

Container::Container( const char *Name )
{
	strcpy( szName, Name );
	CurPos = 0;
}

Container::~Container()
{
	int c;
	for (c=0;c<Controls.size();c++)
	{
		if ( Controls[c] )
		{
			switch ( Controls[c]->type )
			{
			case LABEL:
				delete (Label *)Controls[c];
				break;

			case BUTTON:
				delete (Button *)Controls[c];
				break;

			case FRAME:
				delete (Frame *)Controls[c];
				break;

			case LISTBOX:
				delete (ListBox *)Controls[c];
				break;

			default:
				delete Controls[c];
				break;
			}

			Controls[c] = NULL;
		}
	}
}

void Tab::Load( ifstream &in, CProgressCtrl &Progress, int FileLen )
{
	const char *Delim = "=,;\n\r\"";
	char Buff[256], *word = NULL;
	char *Words[8];
	char Msg[256];
	int c = 0, i;
	Button *b;
	Label *l;
	Frame *f;
	ListBox *ls;
	float CurPos = in.tellg();
	int LastProg = -1;
	
	while ( !in.eof() && !in.fail() && in.peek() != '[' )
	{
		in.getline( Buff, 256 );

		if ( Buff[0] == 0 || (Buff[0] == '/' && Buff[1] == '/') ) continue;

		Fix( Buff );
		word = strtok( Buff, Delim );

		if ( !strcmpi( word, "Button" ) )
		{
			Controls.push_back( (Control *)(b = new Button) );

			for (i=0;i<7;i++)
			{
				Words[i] = strtok(NULL,Delim);
				if ( !Words[i] )
				{
					sprintf( Msg, "Error parsing Button (control #%i) of tab \"%s\".\nExpected (at least) 7 parameters, but got %i\nButton not created.", c+1, GetName(), i );
					pSplash->MessageBox( Msg, "Button Parse Error", MB_OK|MB_ICONERROR );
					break;
				}
			}
			if ( i != 7 ) continue;

			b->x1 = getX( Words[0] );
			b->y1 = getY( Words[1] );
			b->x2 = getXSize( Words[2] );
			b->y2 = getYSize( Words[3] );

			strncpy( b->szText, Words[4], 128 );
			strncpy( b->szCommand, Words[5], 64 );
			strncpy( b->szFmt, Words[6], 10 ); strupr(b->szFmt);

			if ( b->szFmt[0] == 'W' )
			{
				b->pPrompt = (char *)(new Dialog);
				((Dialog *)(b->pPrompt))->Load( b->szCommand );

				continue;
			}
			
			word = strtok(NULL,Delim);
			if ( word )
				strncpy( b->szLink, word, 32 );
			else
				strcpy( b->szLink, "PROMPT" );

			word = strtok(NULL,Delim);
			if ( word )
			{
				b->pPrompt = new char[strlen(word)+1];
				strcpy( b->pPrompt, word );
			}

			c++;
		} else if ( !strcmpi( word, "Label" ) )
		{
			Controls.push_back( (Control *)(l = new Label) );

			for (i=0;i<5;i++)
			{
				Words[i] = strtok(NULL,Delim);
				if ( !Words[i] )
				{
					sprintf( Msg, "Error parsing Label (control #%i) of tab \"%s\".\nExpected 5 parameters, but got %i\nLabel not created.", c+1, GetName(), i );
					pSplash->MessageBox( Msg, "Label Parse Error", MB_OK|MB_ICONERROR );
					break;
				}
			}
			if ( i != 5 ) continue;

			l->x1 = getX( Words[0] );
			l->y1 = getY( Words[1] );
			l->x2 = getXSize( Words[2] );
			l->y2 = getYSize( Words[3] );
			
			strncpy( l->szText, Words[4], 128 );

			c++;
		} else if ( !strcmpi( word, "Frame" ) )
		{
			Controls.push_back( (Control *)(f = new Frame) );

			for (i=0;i<5;i++)
			{
				Words[i] = strtok(NULL,Delim);
				if ( !Words[i] )
				{
					sprintf( Msg, "Error parsing Frame (control #%i) of tab \"%s\".\nExpected 5 parameters, but got %i\nFrame not created.", c+1, GetName(), i );
					pSplash->MessageBox( Msg, "Frame Parse Error", MB_OK|MB_ICONERROR );
					break;
				}
			}
			if ( i != 5 ) continue;

			f->x1 = getX( Words[0] );
			f->y1 = getY( Words[1] );
			f->x2 = getXSize( Words[2] );
			f->y2 = getYSize( Words[3] );
			
			strncpy( f->szText, Words[4], 128 );

			c++;
		} else if ( !strcmpi( word, "ListBox" ) )
		{
			Controls.push_back( (Control *)(ls = new ListBox) );

			for (i=0;i<6;i++)
			{
				Words[i] = strtok(NULL,Delim);
				if ( !Words[i] )
				{
					sprintf( Msg, "Error parsing Listbox (control #%i) of tab \"%s\".\nExpected 6 parameters, but got %i\nListbox not created.", c+1, GetName(), i );
					pSplash->MessageBox( Msg, "Listbox Parse Error", MB_OK|MB_ICONERROR );
					break;
				}
			}
			if ( i != 6 ) continue;

			ls->pList = new ListMenu();
			ls->pList->pParent = NULL;
			strcpy( ls->pList->szName, "ROOT" );

			ls->x1 = getX( Words[0] );
			ls->y1 = getY( Words[1] );
			ls->x2 = getXSize( Words[2] );
			ls->y2 = getYSize( Words[3] );
			
			strncpy( ls->szName, Words[4], 128 );
			ls->Load( Words[5] );

			c++;
		} else {
			sprintf( Msg, "Error processing tab \"%s\" (after control #%i)\nUnknown word \"%s\".\nIgnored.", GetName(), c, word );
			pSplash->MessageBox( Msg, "Tab Warning.", MB_OK|MB_ICONINFORMATION );
		}
		
		CurPos = (float)in.tellg();
		if ( (int)((CurPos/FileLen)*SPLASH_PS) != LastProg && CurPos >= 0 )
		{
			LastProg = (int)((CurPos/FileLen)*SPLASH_PS);
			Progress.SetPos( SPLASH_STEP2 + LastProg );
		}
	}
}

void Tab::HideControls( void )
{
	Control *c = NULL;
	for (c=First();!End();c=Next())
	{
		if ( !c ) continue;

		if ( c->pObject )
			c->pObject->ShowWindow( FALSE );
	}
}

void Tab::ShowControls( void )
{
	Control *c = NULL;
	for (c=First();!End();c=Next())
	{
		if ( !c ) continue;

		if ( c->pObject ) 
			c->pObject->ShowWindow( TRUE );
	}
}

Control::Control()
{
	pObject = NULL;
	id = 0xFFFF;
}

Control::~Control()
{
	if ( pObject )
	{
		delete pObject;
		pObject = NULL;
	}
}

Button::~Button()
{ 
	if ( pPrompt ) 
	{ 
		if ( szFmt[0] == 'W' )
			delete (Dialog *)pPrompt;
		else
			delete[] pPrompt; 
		pPrompt = NULL; 
	} 
}

void Button::OnPress( Container *Cont, CWnd *Parent, BatchVar *Vars, int VarCount )
{
	CPrompt promptdlg(Parent);
	char Build[256];
	int i,a, Count = strlen(szFmt);
	Control *ctrl;
	CString Str;

	if ( Count <= 0 || Count > 8 )
	{
		sprintf( Build, "Error in Button\"%s\"!\nButton has %i Format parameters, when it needs 1 to 8!\nCommand not posted.", szText, Count );
	} else if ( szFmt[0] == 'B' )
	{
		ExecuteBatch( Cont, Parent );
	} else if ( szFmt[0] == 'N' ) 
	{
		PostCommand( szCommand );
	} else if ( szFmt[0] == 'M' )
	{
		pNav->Show( szCommand, pPrompt );
	} else if ( szFmt[0] == 'W' )
	{
		CGenericDialog dlg;
		dlg.DoModal((Dialog *)(pPrompt));
	} else if ( szFmt[0] == 'V' )
	{
		if ( VarCount <= 0 )
		{
			Parent->MessageBox( "Batch varible expected but not passed. Varibles may only be used in batch files.", "Batch Button Error", MB_OK|MB_ICONERROR );
			return;
		}

		for (i=0;i<VarCount;i++)
		{
			if ( !strcmpi( Vars[i].Name, szLink ) )
			{
				Str = szCommand;

				for (a=0;a<Vars[i].ValCount;a++)
				{
					Str += " ";
					if ( Vars[i].Vals[a][0] == '\x1b' )
					{
						if ( PrefixHex ) 
							Str += "0x";
						Str += &((LPCSTR)Vars[i].Vals[a])[1];
					} else 
						Str += Vars[i].Vals[a];
				}
				PostCommand( Str );
				return;
			}
		}

		Parent->MessageBox( "Warning: Batch Varible not found, command not posted.", "ERROR", MB_OK|MB_ICONWARNING );
	} else if ( !strcmpi( szLink, "PROMPT" ) )
	{
		if ( promptdlg.DoModal( Count, pPrompt, (strstr(szFmt,"S")!=NULL) ) == IDCANCEL )
			return;
		
		strcpy( Build, szCommand );
		for (i=0;i<Count;i++)
		{
			if ( szFmt[i] == 'H' && PrefixHex )
				strcat( Build, " 0x" );
			else
				strcat( Build, " " );

			strcat( Build, promptdlg.csValue[i] );
		}
		PostCommand( Build );
	} else {
		for (ctrl=Cont->First();!Cont->End();ctrl=Cont->Next())
		{
			if ( !ctrl ) continue;
			if ( ctrl->type != LISTBOX ) continue;

			if ( !strcmpi( ((ListBox *)ctrl)->szName, szLink ) )
			{
				Str = ((ListBox *)ctrl)->GetString( szFmt );
				if ( Str != "<--ERROR-->" )
					PostCommand( "%s %s", szCommand, (LPCSTR)Str );
				return;
			}
		}//end for

		if ( Cont->End() )
		{
			sprintf( Build, "Command Button->Listbox Link Error!\nButton \"%s\" was unnable to find Listbox \"s\".\n Command not sent.", szText, szLink );
			Parent->MessageBox( Build, "Link Error", MB_OK|MB_ICONERROR );
		}
	}//end else
}

void Button::ExecuteBatch( Container *Cont, CWnd *pParent )
{
	ifstream Batch( szCommand );
	CPrompt prompt(pParent);
	char Buff[256], Temp[256], *part, *words[2];
	const char *Delim = ",=\t\"\n\r";
	Button *b = NULL;
	BatchVar Vars[10];
	int v = 0, i, a;

	if ( !Batch )
	{
		pParent->MessageBox( "There was an error executing this batch file.\nThe batch file could not be opened for reading.", "Batch Error", MB_OK|MB_ICONERROR );
		return;
	}

	while ( !Batch.eof() && !Batch.fail() )
	{
		Batch.getline( Buff, 256 );
		if ( Buff[0] == 0 || ( Buff[0] =='/' && Buff[1] == '/' ) )
			continue;
		Fix( Buff );
		part = strtok( Buff, Delim );

		if ( !strcmpi( part, "wait" ) || !strcmpi( part, "pause" ) )
		{
			part = strtok( NULL, "\n\r" );
			if ( part ) sprintf( Temp, "Script Paused.\n%s\n", part );
			else strcpy( Temp, "Script Paused.\nPress Yes to continue, or No to stop processing." );
		
			if ( pParent->MessageBox( Temp, "Batch Pause", MB_YESNO|MB_ICONQUESTION ) == IDNO )
				break;
		} else if ( !strcmpi( part, "prompt" ) )
		{
			if ( v >= 10 )
			{
				pParent->MessageBox( "Error!\nBatch files may only have 10 varibles.", "Batch Error", MB_OK|MB_ICONERROR );
				continue;
			}

			part = strtok( NULL, Delim );
			words[0] = strtok( NULL, Delim );
			words[1] = strtok( NULL, Delim );
			if ( !part || !words[0] || !words[1] )
			{
				pParent->MessageBox( "Fatal Error!\nNot enough prompt parameters in batch file, processing stopped.", "Batch Error", MB_OK|MB_ICONERROR );
				break;
			}

			Vars[v].ValCount = strlen(words[0]);
			if ( prompt.DoModal( Vars[v].ValCount, part, (strstr(words[0],"S")!=NULL) ) == IDCANCEL )
				break;
			
			Vars[v].Name = words[1];
			for (i=0;i<Vars[v].ValCount;i++)
			{
				if ( words[0][i] == 'H' || words[0][i] == 'h' )
					Vars[v].Vals[i] = "\x1b"+prompt.csValue[i]; 
				else
					Vars[v].Vals[i] = prompt.csValue[i];
			}
			v++;
		} else if ( !strcmpi( part, "offset" ) )
		{
			part = strtok( NULL, Delim );
			for (i=0;i<v;i++)
			{
				if ( !strcmpi( part, Vars[i].Name ) )
				{
					a=0;
					while ( (part=strtok( NULL, Delim ))!=NULL && a < Vars[i].ValCount )
					{
						OffsetVal( part, Vars[i].Vals[a] );
						a++;
					}
					i=-1;
					break;
				}
			}
			if ( i==v ) pParent->MessageBox( "Batch Varible for offset not found.", "Batch Var Error", MB_OK|MB_ICONERROR );
		} else {
			if ( b ) delete b;
			b = new Button();
			strcpy( b->szText, "Batch Button" );
			
			strncpy( b->szCommand, part, 64 );
			
			part = strtok( NULL, Delim );
			if ( !part )
			{
				pParent->MessageBox( "Error!\nNot enough arguments for Batch file command.", "Batch Error", MB_OK|MB_ICONERROR );
				continue;
			}
			strncpy( b->szFmt, part, 10 ); strupr(b->szFmt);

			if ( b->szFmt[0] == 'W' || b->szFmt[0] == 'M' )
			{
				pParent->MessageBox( "Error!\nDialogs and MapNav cannot be lanuched from batch files.", "Batch Error", MB_OK|MB_ICONERROR );
				continue;
			}
			
			part = strtok(NULL,Delim);
			if ( part )
				strncpy( b->szLink, part, 32 );
			else
				strcpy( b->szLink, "PROMPT" );

			part = strtok(NULL,Delim);
			if ( part )
			{
				b->pPrompt = new char[strlen(part)+1];
				strcpy( b->pPrompt, part );
			} else b->pPrompt = NULL;

			b->OnPress( Cont, pParent, Vars, v );
		}
	}

	if ( b ) delete b;

	Batch.close();
}

ListBox::~ListBox()
{
	if ( pList )
	{
		delete pList;
		pList = NULL;
	}

	if ( pPrompt )
	{
		delete pPrompt;
		pPrompt = NULL;
	}
}

void ListBox::Load( const char *file )
{
	ifstream in(file);
	char Buff[256], *word;
	const char *Delim = "=\"\n\r", *Word;
	int line=0;
	bool bFmt = false;
	ListEntry *e;
	ListMenu *m;

	if ( !in ) return;

	while ( !in.eof() && !in.fail() )
	{
		in.getline( Buff, 256 );
		line++;
		if ( Buff[0] == 0 || Buff[0] == '}' || (Buff[0] == '/' && Buff[1] == '/') )
			continue;

		if ( ( strstr( Buff, "format=" ) == Buff || strstr( Buff, "FORMAT=" ) == Buff || strstr( Buff, "Format=" ) == Buff ) && !bFmt )//format=occurs at begining of string.
		{
			strtok( Buff, Delim );
			Word = strtok( NULL, Delim );
			if ( !Word )
			{
				sprintf( Buff, "WARNING: Format error in \"%s\"\nNo format parameters given.\n", file );
				pSplash->MessageBox( Buff, "lst Load Warning", MB_OK|MB_ICONERROR );
				continue;
			}
			strncpy( szFmt, Word, 16 ); strupr( szFmt );

			Word=strtok( NULL, Delim );
			if ( Word )
			{
				pPrompt = new char [strlen(Word)+1];
				strcpy( pPrompt, Word );
			} else pPrompt = NULL;

			bFmt = true;
		} else if ( strstr( Buff, "=" ) && !strstr( Buff, "{" ) )
		{//My Name=Value[=ArtID]
			pList->Entries.push_back( (ListObject *)(e=new ListEntry) );
			word = strtok( Buff, Delim );
			if ( !word )
			{
				sprintf( Buff, "WARNING: Unable to split parameters on line %i of \"%s\"\n", line, file );
				pSplash->MessageBox( Buff, "lst Load Error", MB_OK|MB_ICONERROR );
				continue;
			}
			strcpy( e->szName, word );

			word = strtok( NULL, Delim );
			if ( !word )
			{
				sprintf( Buff, "WARNING: Unable to split parameters on line %i of \"%s\"\n", line, file );
				pSplash->MessageBox( Buff, "lst Load Error", MB_OK|MB_ICONERROR );
				continue;
			}
			strcpy( e->szValue, word );

			word = strtok( NULL, Delim );
			if ( word )
				e->ArtID = HexNumber( word );
		} else if ( in.peek() == '{' )
		{
			pList->Entries.push_back( (ListObject *)(m=new ListMenu) );
			sprintf( m->szName, "> %s", Buff ); 
			m->pParent = pList;
			m->Load( in, file, line );
		} else {
			sprintf( Buff, "Error loading list file \"%s\"\nInvalid tag or missing brace \'{\'.", file );
			pSplash->MessageBox( Buff, "Warning", MB_OK|MB_ICONINFORMATION );
		}
	}

	in.close();
}

void ListBox::InitList( void )
{
	int i;
	ListObject *lo;
	CPreviewList *List = (CPreviewList *)pObject;

	for (i=0;i<pList->Entries.size();i++)
	{
		lo = pList->Entries[i];
		List->AddString( lo->szName ); //AddString( lo->szName );
	}

	pCurrent = pList;
}

unsigned short ListBox::GetArtID( int i )
{
	if ( pCurrent == NULL ) pCurrent = pList;

	if ( pCurrent != pList ) i--;
	
	if ( i < 0 || i>=pCurrent->Entries.size() ) return 0xFFFF;

	return pCurrent->Entries[i]->ArtID;
}

void ListBox::OnDoubleClick( CWnd *Parent, Container *Cont )
{
	CPreviewList *List = (CPreviewList *)pObject;
	ListObject *lo;
	int s=List->GetCurSel(), i;

	if ( !pCurrent ) return;
	else if ( s<0 || s>pCurrent->Entries.size() ) return;

	if ( s == 0 && pCurrent != pList )
	{//back button
		pCurrent = pCurrent->pParent;
		List->Clear();

		if ( pCurrent != pList )
			List->AddString( "< Back" );

		for (i=0;i<pCurrent->Entries.size();i++)
		{
			lo = pCurrent->Entries[i];
			List->AddString( lo->szName );
		}
	} else {
		if ( pCurrent != pList ) s--; 
		if ( s<0 || s>pCurrent->Entries.size() ) return;

		lo = pCurrent->Entries[s];
		if ( !lo ) return;

		if ( lo->type == MENU )
		{
			pCurrent = (ListMenu *)lo;

			List->Clear();
			List->AddString( "< Back" );

			for (i=0;i<pCurrent->Entries.size();i++)
			{
				lo = pCurrent->Entries[i];
				if ( !lo ) continue;
				List->AddString( lo->szName ); //AddString( lo->szName );
			}
		} else {
			Control *ctrl;
			CString Str;
			char Build[256];
			for (ctrl=Cont->First();!Cont->End();ctrl=Cont->Next())
			{
				if ( !ctrl ) continue;
				if ( ctrl->type != BUTTON ) continue;

				if ( !strcmpi( szName, ((Button *)ctrl)->szLink ) )
				{
					Str = GetString( ((Button *)ctrl)->szFmt );
					if ( Str != "<--ERROR-->" )
						PostCommand( "%s %s", ((Button *)ctrl)->szCommand, (LPCSTR)Str );
					return;
				}
			}//end for

			if ( Cont->End() )
			{
				sprintf( Build, "Command Button->Listbox Link Error!\nButton \"%s\" was unnable to find Listbox \"s\".\n Command not sent.", ((Button *)ctrl)->szText, ((Button *)ctrl)->szLink );
				Parent->MessageBox( Build, "Link Error", MB_OK|MB_ICONERROR );
			}
		}
	}
}

/*signed int ListBox::GetValue( void )
{
	CPreviewList *List = (CPreviewList *)pObject;
	int s = List->GetCurSel();

	if ( pCurrent != pList )
		s--;

	if ( s<0 || s>=pCurrent->Entries.size() || pCurrent->Entries[s]->type != VALUE )
	{
		pAccomplice->MessageBox( "You must select a value before executing that command!", "No Selection", MB_OK|MB_ICONINFORMATION );
		return -1;
	} else return atoi( ((ListEntry *)pCurrent->Entries[s])->szValue );
}*/

CString ListBox::GetString( char *ButtonFmt )
{
	CPreviewList *List = (CPreviewList *)pObject;
	int s = List->GetCurSel();
	int l, i;
	CPrompt prompt;
	CString Ret;

	if ( !ButtonFmt ) return "<--ERROR-->";

	if ( pCurrent != pList )
		s--;

	if ( s<0 || s>=pCurrent->Entries.size() || pCurrent->Entries[s]->type != VALUE )
	{
		pAccomplice->MessageBox( "You must select a value before executing that command!", "No Selection", MB_OK|MB_ICONINFORMATION );
		return "<--ERROR-->";
	} else {
		if ( strlen(szFmt) > 1 )
		{
			l = strlen(szFmt);
			prompt.DoModal( l-1, pPrompt, strstr(&szFmt[1], "S" ) ? true : false );
			if ( szFmt[0] == 'H' && PrefixHex )
				Ret = CString("0x") + ((ListEntry *)pCurrent->Entries[s])->szValue;
			else
				Ret = ((ListEntry *)pCurrent->Entries[s])->szValue;

			for (i=0;i<l-1;i++)
			{
				if ( szFmt[i+1] == 'H' && PrefixHex )
					Ret += " 0x" + prompt.csValue[i];
				else
					Ret += " " + prompt.csValue[i];
			}
		} else {
			if ( ButtonFmt[0] == 'H' || szFmt[0] == 'H' && PrefixHex )
				Ret = "0x";
			Ret += ((ListEntry *)pCurrent->Entries[s])->szValue;
		}
	}

	return Ret;
}

ListMenu::~ListMenu()
{ 
	int i; 
	for(i=0;i<Entries.size();i++) 
	{
		if ( Entries[i] )
		{
			if ( Entries[i]->type == MENU )
				delete (ListMenu *)Entries[i];
			else
				delete (ListEntry *)Entries[i];

			Entries[i] = NULL;
		}
	}
}

void ListMenu::Load( ifstream &in, const char *file, int &line )
{
	char Buff[256], *word;
	const char *Delim="\t=\n\r";
	ListMenu *m;
	ListEntry *e;

	while ( !in.eof() && !in.fail() )
	{
		in.getline( Buff, 256 );
		line++;

		if ( Buff[0] == 0 || (Buff[0] == '/' || Buff[0] == '/') )
			continue;
		else if ( Buff[0] == '}' )
			break;

		if ( strstr( Buff, "=" ) && !strstr( Buff, "{" ) )
		{
			Entries.push_back( (ListObject *)(e=new ListEntry) );
			word = strtok( Buff, Delim );
			if ( !word )
			{
				sprintf( Buff, "WARNING: Unable to split parameters on line %i of \"%s\"\n", line, file );
				pSplash->MessageBox( Buff, "lst Load Error", MB_OK|MB_ICONERROR );
				continue;
			}
			strcpy( e->szName, word );

			word = strtok( NULL, Delim );
			if ( !word )
			{
				sprintf( Buff, "WARNING: Unable to split parameters on line %i of \"%s\"\n", line, file );
				pSplash->MessageBox( Buff, "lst Load Error", MB_OK|MB_ICONERROR );
				continue;
			}
			strcpy( e->szValue, word );

			word = strtok( NULL, Delim );
			if ( word )
				e->ArtID = HexNumber( word );
		} else if ( in.peek() == '{' )
		{
			Entries.push_back( (ListObject *)(m=new ListMenu) );
			sprintf( m->szName, "> %s", Buff ); 
			m->pParent = this;
			m->Load( in, file, line );
		} else if ( strstr( Buff, "{" ) )
		{
			word = strtok( Buff, Delim );
			if ( !word ) continue;

			word = strtok( NULL, Delim );
			if ( word ) 
				ArtID = HexNumber( word );
		} else {
			sprintf( Buff, "Error loading list file \"%s\"\nInvalid tag or missing brace \'{\'.", file );
			pSplash->MessageBox( Buff, "Warning", MB_OK|MB_ICONINFORMATION );
		}
	}
}

Dialog::Dialog( void ) 
	: Container( "No Title Given" ), MySize(100,100)
{
	strcpy( szFile, "" );

	row=col=1;
	hspace=vspace=0;
}

void Dialog::Load( const char *file )
{
	const char *Delim = "=,;\n\r\"";
	char Buff[256], *word = NULL, *part2;
	char *Words[8];
	char Msg[256];
	int c = 0, i;
	Button *b;
	Label *l;
	Frame *f;
	ListBox *ls;
	ifstream in( file );

	if ( !in )
	{
		sprintf( Buff, "Custom Dialog initialization error!\nThe file \"%s\" could not be opened.\n", file );
		pSplash->MessageBox( Buff, "File Input Error", MB_OK|MB_ICONERROR );
		return;
	}

	strncpy( szFile, file, 32 );
	
	while ( !in.eof() && !in.fail() && in.peek() != '[' )
	{
		in.getline( Buff, 256 );

		if ( Buff[0] == 0 || (Buff[0] == '/' && Buff[1] == '/') ) continue;

		word = strtok( Buff, Delim );
		if ( !strcmpi(word, "title") )
		{//don't Fix() the title.
			strncpy( szName, &Buff[6], 128 );
			continue;
		}
		
		Fix( Buff );
		if ( !strcmpi(word, "columns") )
		{
			part2 = strtok(NULL,Delim);
			if ( !part2 ) continue;

			col = atoi(part2);
		} else if ( !strcmpi(word, "rows") )
		{
			part2 = strtok(NULL,Delim);
			if ( !part2 ) continue;

			row = atoi(part2);
		} else if ( !strcmpi(word, "vspace") )
		{
			part2 = strtok(NULL,Delim);
			if ( !part2 ) continue;

			vspace = atoi(part2);
		} else if ( !strcmpi(word, "hspace") )
		{
			part2 = strtok(NULL,Delim);
			if ( !part2 ) continue;

			hspace = atoi(part2);
		} else if ( !strcmpi(word, "XSize") )
		{
			part2 = strtok(NULL,Delim);
			if ( !part2 ) continue;

			MySize.cx = atoi(part2);
		} else if ( !strcmpi(word, "YSize") )
		{
			part2 = strtok(NULL,Delim);
			if ( !part2 ) continue;

			MySize.cy = atoi(part2);
		} else if ( !strcmpi( word, "Button" ) )
		{
			Controls.push_back( (Control *)(b = new Button) );

			for (i=0;i<7;i++)
			{
				Words[i] = strtok(NULL,Delim);
				if ( !Words[i] )
				{
					sprintf( Msg, "Error parsing Button (control #%i) of file \"%s\".\nExpected (at least) 7 parameters, but got %i\nButton not created.", c+1, file, i );
					pSplash->MessageBox( Msg, "Button Parse Error", MB_OK|MB_ICONERROR );
					break;
				}
			}
			if ( i != 7 ) continue;

			b->x1 = getX( Words[0] );
			b->y1 = getY( Words[1] );
			b->x2 = getXSize( Words[2] );
			b->y2 = getYSize( Words[3] );

			strncpy( b->szText, Words[4], 128 );
			strncpy( b->szCommand, Words[5], 64 );
			strncpy( b->szFmt, Words[6], 10 ); strupr(b->szFmt);

			if ( b->szFmt[0] == 'W' )
			{
				sprintf( Msg, "WARNING: Calling a dialog from a dialog is NOT SUPPORTED!\nButton \"%s\" in dialog \"%s\" [%s] cannot create a dialog, since it is on one!\nThis WILL cause a crash!\n", b->szText, szName, szFile );
				pSplash->MessageBox( Msg, "Dialog Call from Dialog", MB_OK|MB_ICONERROR );
			}
			
			word = strtok(NULL,Delim);
			if ( word )
				strncpy( b->szLink, word, 32 );
			else
				strcpy( b->szLink, "PROMPT" );

			word = strtok(NULL,Delim);
			if ( word )
			{
				b->pPrompt = new char[strlen(word)+1];
				strcpy( b->pPrompt, word );
			}

			c++;
		} else if ( !strcmpi( word, "Label" ) )
		{
			Controls.push_back( (Control *)(l = new Label) );

			for (i=0;i<5;i++)
			{
				Words[i] = strtok(NULL,Delim);
				if ( !Words[i] )
				{
					sprintf( Msg, "Error parsing Label (control #%i) of file \"%s\".\nExpected 5 parameters, but got %i\nLabel not created.", c+1, file, i );
					pSplash->MessageBox( Msg, "Label Parse Error", MB_OK|MB_ICONERROR );
					break;
				}
			}
			if ( i != 5 ) continue;

			l->x1 = getX( Words[0] );
			l->y1 = getY( Words[1] );
			l->x2 = getXSize( Words[2] );
			l->y2 = getYSize( Words[3] );
			
			strncpy( l->szText, Words[4], 128 );

			c++;
		} else if ( !strcmpi( word, "Frame" ) )
		{
			Controls.push_back( (Control *)(f = new Frame) );

			for (i=0;i<5;i++)
			{
				Words[i] = strtok(NULL,Delim);
				if ( !Words[i] )
				{
					sprintf( Msg, "Error parsing Frame (control #%i) of file \"%s\".\nExpected 5 parameters, but got %i\nFrame not created.", c+1, file, i );
					pSplash->MessageBox( Msg, "Frame Parse Error", MB_OK|MB_ICONERROR );
					break;
				}
			}
			if ( i != 5 ) continue;

			f->x1 = getX( Words[0] );
			f->y1 = getY( Words[1] );
			f->x2 = getXSize( Words[2] );
			f->y2 = getYSize( Words[3] );
			
			strncpy( f->szText, Words[4], 128 );

			c++;
		} else if ( !strcmpi( word, "ListBox" ) )
		{
			Controls.push_back( (Control *)(ls = new ListBox) );

			for (i=0;i<6;i++)
			{
				Words[i] = strtok(NULL,Delim);
				if ( !Words[i] )
				{
					sprintf( Msg, "Error parsing Listbox (control #%i) of file \"%s\".\nExpected 6 parameters, but got %i\nListbox not created.", c+1, file, i );
					pSplash->MessageBox( Msg, "Listbox Parse Error", MB_OK|MB_ICONERROR );
					break;
				}
			}
			if ( i != 6 ) continue;

			ls->pList = new ListMenu();
			ls->pList->pParent = NULL;
			strcpy( ls->pList->szName, "ROOT" );

			ls->x1 = getX( Words[0] );
			ls->y1 = getY( Words[1] );
			ls->x2 = getXSize( Words[2] );
			ls->y2 = getYSize( Words[3] );
			
			strncpy( ls->szName, Words[4], 128 );
			ls->Load( Words[5] );

			c++;
		} else {
			sprintf( Msg, "Error processing file \"%s\" (after control #%i)\nUnknown word \"%s\".\nIgnored.", file, c, word );
			pSplash->MessageBox( Msg, "Dialog Warning.", MB_OK|MB_ICONINFORMATION );
		}
	}

	in.close();
}
