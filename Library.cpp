#include "StdAfx.h"
#include "Accomplice.h"
#include "Library.h"

Library::Library( void )
{
	hModule = NULL;
}

Library::Library( const char *szLibName )
{
	Load(szLibName);
}

Library::~Library()
{
	Unload();
}

void Library::Load( const char *szLibName )
{
	hModule = LoadLibrary( szLibName );
}

void Library::Unload( void )
{
	if ( hModule )
	{
		FreeLibrary( hModule );
		hModule = NULL;
	}
}

HMODULE Library::GetModule( void )
{
	return hModule;
}

bool Library::isLoaded( void )
{
	return (hModule != NULL);
}

