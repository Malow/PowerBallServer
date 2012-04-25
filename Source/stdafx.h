//
// NEEDS REWRITE, HAVN'T WRITTEN MYSELF / MaloW
//

#ifndef _STDAFX__H
#define _STDAFX__H

#include <windows.h>
#include <iostream>

// Helpfiles from MaloW
#include "MaloW.h"
#include "MaloWFileDebug.h"
#include "Array.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>

/*! constants */
#define PI (3.14159265358979323846f)

/*! functions */
#define SAFE_RELEASE(x) { if(x) { (x)->Release(); (x) = NULL; } }
#define SAFE_DELETE(x) { if(x) { delete(x); (x) = NULL; } }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }
/*! returns random float between [0,1] */
inline float RndFloat() 
{
	//rand() = between 0 and RAND_MAX (32767)
	return (float)(rand()) / (float)RAND_MAX;
}
/*! returns random float between [a,b] */
inline float RndFloat(float a, float b)
{
	return a + RndFloat() * (b - a);
}

//////////////////////////////////////////////////////////////////////////
// to find memory leaks
//////////////////////////////////////////////////////////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define myMalloc(s)       _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myCalloc(c, s)    _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myRealloc(p, s)   _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myExpand(p, s)    _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myFree(p)         _free_dbg(p, _NORMAL_BLOCK)
#define myMemSize(p)      _msize_dbg(p, _NORMAL_BLOCK)
#define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define myDelete delete  // Set to dump leaks at the program exit.
#define myInitMemoryCheck() \
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#define myDumpMemoryLeaks() \
	_CrtDumpMemoryLeaks()
#else
#define myMalloc malloc
#define myCalloc calloc
#define myRealloc realloc
#define myExpand _expand
#define myFree free
#define myMemSize _msize
#define myNew new
#define myDelete delete
#define myInitMemoryCheck()
#define myDumpMemoryLeaks()
#endif 
//////////////////////////////////////////////////////////////////////////



#endif
