#ifndef __ART_H_
#define __ART_H_

#pragma pack(1)

struct ArtIdx
{
	long FilePos;
	long Length;
	long Unused;
};

struct ArtHeader
{
	long Header; // Unknown
	short Width;
	short Height;
	//followed by short LookupTable
};

/*
int Header
short width
short height
short lookupTable[height] 
	offset end of lookuptable to data (height*sizeof(short)=sizeof lookuptable)

  data 
  {
	short xSkip;//number of pixels skipped before the start of data
	short xRun;//number of pixels used
	short runColors[xRun];
  }
*/

class CArtMul
{
public:
	CArtMul();
	~CArtMul();
	
	bool GetTile( int, CDC*, CRect, unsigned char = 0x80, bool = true );
	bool GetTile( int id, CDC *pDC ) { return GetTile( id, pDC, CRect(0,0,0,0), 0x80, false ); }
		
protected:
	ifstream Idx;
	ifstream Mul;
};

#endif

