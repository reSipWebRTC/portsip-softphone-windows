/*
 *	_RTEXT.H
 *	
 *	Purpose:
 *		Base classes for rich-text manipulation
 *	
 *	Authors:
 *		Original RichEdit code: David R. Fulmer
 *		Christian Fortini
 *		Murray Sargent
 *
 *	TODO (alexgo) 	comment/format this stuff for autodoc
 */

#ifndef _RTEXT_H
#define _RTEXT_H

#include "_edit.h"
#include "_array.h"
#include "_doc.h"
#include "_text.h"
#include "_runptr.h"
#include "_frunptr.h"
#include "_notmgr.h"



//#pragma warning(disable: 4250)	

#define yHeightCharMost	32760

class CTxtEdit;
class CTxtRange;
class CRchTxtPtr;

extern BOOL IsWhiteSpace(unsigned ch);

// ==============================  CRchTxtPtr  =====================================================
// Keeps physical positions corresponding to text character position (cp)
// within current text block, formatting runs, objects, unknown RTF runs,
// and floating ranges. 

class CRchTxtPtr : public ITxNotify
{
public:

#ifdef DEBUG
    BOOL Invariant( void ) const;
#endif  // DEBUG

	CTxtPtr			_rpTX;		// rp in the plain text array
	CFormatRunPtr	_rpCF;		// rp in character format runs
	CFormatRunPtr	_rpPF;		// rp in paragraph format runs

// Useful constructors

	CRchTxtPtr(CTxtEdit *ped);
	CRchTxtPtr(CTxtEdit *ped, DWORD cp);
	CRchTxtPtr(const CRchTxtPtr& rtp);
	CRchTxtPtr(const CDisplay * pdp);

	virtual CRchTxtPtr& operator =(const CRchTxtPtr& rtp)
	{
		_rpTX._ped = rtp._rpTX._ped;
		SetCp(rtp.GetCp());
		return *this;
	}

	LONG 	Advance(LONG cch);
	DWORD	SetCp( DWORD cp);
	void	BindToCp(DWORD cp);
	void	CheckFormatRuns();
	LONG	GetCp() const			{ return (LONG)_rpTX.GetCp(); }
	DWORD	GetTextLength() const	{ return _rpTX.GetTextLength(); }
	DWORD	GetObjectCount() const	{ return GetPed()->GetObjectCount(); }
	CTxtEdit *GetPed() const		{ return _rpTX._ped; }
	const TCHAR * GetPch(LONG &cchvalid) { return _rpTX.GetPch(cchvalid); }
	TCHAR 	GetChar()				{ return _rpTX.GetChar(); }
	void	ValidateCp(LONG &cp) const;

	// Text manipulation methods 

	// Range operations
	LONG	ReplaceRange(LONG cchOld, LONG cchNew, TCHAR const *pch,
						 IUndoBuilder *publdr, LONG iFormat);
	BOOL	ChangeCase(LONG cch, LONG Type, IUndoBuilder *publdr);
	LONG	UnitCounter (LONG iUnit, LONG &	cUnit, LONG cchMax);
	
	// Search and word-break support
	LONG	FindText(LONG cpMax, DWORD dwFlags, TCHAR const *pch,
					 LONG cchToFind);
	LONG	FindWordBreak(INT action, LONG cpMost = -1);

	// Text-run management
	LONG 	GetIchRunCF();
	LONG 	GetCchRunCF();
	LONG 	GetCchLeftRunCF();
	LONG 	GetCchLeftRunPF();
	
	// Character & paragraph format retrieval
	const CCharFormat* GetCF() const;
	const CParaFormat* GetPF() const;

    // ITxNotify methods
    virtual void    OnPreReplaceRange( DWORD cp, DWORD cchDel, DWORD cchNew,
    					DWORD cpFormatMin, DWORD cpFormatMax) { ; }
	virtual void 	OnPostReplaceRange( DWORD cp, DWORD cchDel, DWORD cchNew,
						DWORD cpFormatMin, DWORD cpFormatMax) { ; }
	virtual void	Zombie();

protected:
	BOOL 	AdvanceRunPtrs(LONG cp);
	BOOL	Check_rpCF();
	BOOL	Check_rpPF();
	void	InitRunPtrs(LONG cp);
	BOOL	IsRich();
	BOOL	IsIMERich() const;
	BOOL	SetRunPtrs(LONG cp, LONG cpFrom);

private:
	LONG	ReplaceRangeFormatting(LONG cchOld, LONG cchNew, LONG iFormat,
							IUndoBuilder *publdr,
							IAntiEvent **ppaeCF, IAntiEvent **ppaePF,
							LONG cchMove, LONG cchPrevEOP, LONG cchNextEOP);
};

#endif
