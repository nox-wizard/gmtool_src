#ifndef __LIBRARY_H_
#define __LIBRARY_H_

class Library
{
public:
	Library();
	Library( const char * );
	~Library();

	void Load( const char * );
	void Unload( void );

	HMODULE GetModule( void );
	bool isLoaded( void );

protected:
	HMODULE hModule;
};

#endif

