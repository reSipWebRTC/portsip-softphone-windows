/*
 *	_UTIL.H
 *
 *	Purpose:
 *		declarations for various useful utility functions
 *
 *	Author:
 *		alexgo (4/25/95)
 */

#ifndef __UTIL_H__
#define __UTIL_H__

HGLOBAL	TextHGlobalAtoW( HGLOBAL hglobal );
INT CountMatchingBits(const DWORD *a, const DWORD *b, INT total);
HRESULT ObjectReadSiteFlags(REOBJECT * preobj);


//Default values for drag scrolling
//(that aren't already defined by windows).
#define DEFSCROLLMAXVEL 100	//Cursor velocity above which we will not
							//drag scroll (units=.01 in/sec).
#define DEFSCROLLVAMOUNT 50	//Vert. scroll amount per interval (units=.01 in)
#define DEFSCROLLHAMOUNT 50 //Horz. scroll amount per interval (units=.01 in)

//Stuff from OLESTD samples

//Ole clipboard format defines.
#define CF_EMBEDSOURCE      "Embed Source"
#define CF_EMBEDDEDOBJECT   "Embedded Object"
#define CF_LINKSOURCE       "Link Source"
#define CF_OBJECTDESCRIPTOR "Object Descriptor"
#define CF_FILENAME         "FileName"
#define CF_OWNERLINK        "OwnerLink"

HRESULT OleStdSwitchDisplayAspect(
			LPOLEOBJECT			lpOleObj,
			LPDWORD				lpdwCurAspect,
			DWORD				dwNewAspect,
			HGLOBAL				hMetaPict,
			BOOL				fDeleteOldAspect,
			BOOL				fSetupViewAdvise,
			LPADVISESINK		lpAdviseSink,
			BOOL FAR *			lpfMustUpdate);
LPUNKNOWN OleStdQueryInterface(
			LPUNKNOWN			lpUnk,
			REFIID				riid);



/* 
 *	CSystemParams
 *
 *	@class	Class for various system-wide parameters.
 *
 */
class CSystemParams
{
public:
    CSystemParams();
    ~CSystemParams();
    void SetScrollInset(WORD inset) {_nScrollInset = inset;}
    void SetScrollDelay(WORD delay) {_nScrollDelay = delay;}
    void SetScrollInterval(WORD interval) {_nScrollInterval = interval;}
    void SetScrollVAmount(WORD amount) {_nScrollVAmount = amount;}
    void SetScrollHAmount(WORD amount) {_nScrollHAmount = amount;}
	void SetDragDelay(WORD delay) {_nDragDelay = delay;}
	void SetDragMinDist(WORD dist) {_nDragMinDist = dist;}
    WORD GetScrollInset();
    WORD GetScrollDelay();
    WORD GetScrollInterval();
	WORD GetScrollVAmount();
	WORD GetScrollHAmount();
	WORD GetDragDelay();
	WORD GetDragMinDist();
	HCURSOR GetSizeCursor(LPTSTR idcur);
	LONG GetRollerLineScrollCount();
    
private:
	    //Width of hot zone (in pixels) for auto-scrolling
    WORD _nScrollInset;
	    //Delay (in ms) before scrolling
    WORD _nScrollDelay;
		//Interval (in ms) at which we scroll
    WORD _nScrollInterval;
		//Amount of vertical scroll at each interval (pixels)
	WORD _nScrollVAmount;
		//Amount of horizontal scroll at each interval (pixels)
	WORD _nScrollHAmount;
		//North/South sizing cursor (double arrow)
	WORD _nDragDelay;
		//Amount of time to wait for determining start of drag operation
	WORD _nDragMinDist;
		//Minimun distance that must be traversed within drag delay time interval
	HCURSOR _hcurSizeNS;
		//West/East sizing cursor (double arrow)
	HCURSOR _hcurSizeWE;
		//Northwest/Southeast sizing cursor (double arrow)
	HCURSOR _hcurSizeNWSE;
		//Northeast/Southwest sizing cursor (double arrow)
	HCURSOR _hcurSizeNESW;
		//Number of Lines to scroll with a mouse roller wheel, -1 for pages
	LONG	_cLineScroll;

};

extern CSystemParams sysparam;
 
/****************************************************************************/
/*		     Stabilization classes				    						*/
/*        These are used to stabilize objects during re-entrant calls       */
/****************************************************************************/

//+-------------------------------------------------------------------------
//
//  Class: 	CSafeRefCount
//
//  Purpose: 	A concrete class for objects like the default handler to
//				inherit from.  CSafeRefCount will keep track of reference
//				counts, nesting counts, and zombie states, allowing objects
//				to easily manage the liveness of their memory images.
//
//  Interface:	
//
//  History:    dd-mmm-yy Author    Comment
//   			01-Aug-94 alexgo    author
//
//--------------------------------------------------------------------------

class CSafeRefCount
{
public:
	ULONG	SafeAddRef();
	ULONG	SafeRelease();
	ULONG	IncrementNestCount();
	ULONG	DecrementNestCount();
	BOOL	IsZombie();
   
			CSafeRefCount();
	virtual ~CSafeRefCount();

protected:
    VOID    Zombie();

private:

	ULONG	m_cRefs;
	ULONG	m_cNest;

	ULONG	m_fInDelete		:1;
	ULONG   m_fForceZombie	:1;
};

//+-------------------------------------------------------------------------
//
//  Class:	CStabilize
//
//  Purpose: 	An instance of this class should be allocated on the
//				stack of every object method that makes an outgoing call.
//				The contstructor takes a pointer to the object's base
//				CSafeRefCount class.
//
//  Interface:
//
//  History:    dd-mmm-yy Author    Comment
// 				01-Aug-94 alexgo    author
//
//  Notes:	The constructor will increment the nest count of the
//			object while the destructor will decrement it.
//
//--------------------------------------------------------------------------

class CStabilize
{
public:
	inline CStabilize( CSafeRefCount *pObjSafeRefCount );
	inline ~CStabilize();

private:
	CSafeRefCount *	m_pObjSafeRefCount;
};

inline CStabilize::CStabilize( CSafeRefCount *pObjSafeRefCount )
{
	pObjSafeRefCount->IncrementNestCount();
	m_pObjSafeRefCount = pObjSafeRefCount;
}

inline CStabilize::~CStabilize()
{
	m_pObjSafeRefCount->DecrementNestCount();
}

/*
 *	SafeReleaseAndNULL(IUnknown **ppUnk)
 *
 *	Purpose:
 *      Helper for getting stable pointers during destruction or other times
 *
 *	Notes: 
 *      Not thread safe, must provide higher level synchronization.
 */

inline void SafeReleaseAndNULL(IUnknown **ppUnk)
{
    if (*ppUnk != NULL)
    {
    IUnknown *pUnkSave = *ppUnk;
    *ppUnk = NULL;
    pUnkSave->Release();
    }
}

// a handy-dandy character test routine
BOOL IsWhite(TCHAR ch);

BOOL FIsIconMetafilePict(HGLOBAL hmfp);
HANDLE OleStdGetMetafilePictFromOleObject(
        LPOLEOBJECT         lpOleObj,
        DWORD               dwDrawAspect,
        LPSIZEL             lpSizelHim,
        DVTARGETDEVICE FAR* ptd);

// Default size for stack buffer
#define MAX_STACK_BUF 512


/*
 *	CTempCharBuf
 *	
 * 	@class	A wrapper for the temporary buffer allocater that returns a buffer of
 *			char's.
 *
 *	@base	private | CTempBuf
 */
class CTempCharBuf : private CTempBuf
{
//@access Public Data
public:

							//@cmember Get buffer of size cch characters
	char *					GetBuf(LONG cch);
};


/*
 *	CTempBuf::GetBuf
 *
 *	@mfunc	Get a buffer of the requested size
 *
 *	@rdesc	Pointer to buffer or NULL if one could not be allocated
 *
 */
inline char *CTempCharBuf::GetBuf(LONG cch)
{
	return (char *) CTempBuf::GetBuf(cch * sizeof(TCHAR));
}





#endif // !__UTIL_H__
