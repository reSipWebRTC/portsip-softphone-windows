/*
 *	_NOTMGR.H
 *
 *	Purpose:
 *		Notification Manager declarations
 *
 *	Author:
 *		AlexGo	6/5/95
 */

#ifndef _NOTMGR_H_
#define _NOTMGR_H_

// forward declaration
class CNotifyMgr;

// Set cp to this to signal that the control has converted from rich to plain.
// BUGBUG: (alexgo) We need a seperate notification method.
const DWORD CONVERT_TO_PLAIN = 0xFFFFFFFE;


/*
 *	ITxNotify
 *
 *	Purpose:
 *		a notification sink for events happening to the backing store,
 *		used by the Notification Manager
 */
class ITxNotify
{
public:
	virtual void OnPreReplaceRange( DWORD cp, DWORD cchDel, DWORD cchNew,
					DWORD cpFormatMin, DWORD cpFormatMax ) = 0;
	virtual void OnPostReplaceRange( DWORD cp, DWORD cchDel, DWORD cchNew,
					DWORD cpFormatMin, DWORD cpFormatMax ) = 0;
	virtual void Zombie() = 0;

private:
	ITxNotify *	_pnext;

	friend class CNotifyMgr;	// so it can manipulate _pnext
};


/*
 *	CNotifyMgr
 *
 *	Purpose:
 *		the general notification manager; keeps track of all interested 
 *		notification sinks
 */

class CNotifyMgr
{
public:
	void Add( ITxNotify *pITN );
	void Remove( ITxNotify *pITN );
	void NotifyPreReplaceRange( ITxNotify *pITNignore, DWORD cp, DWORD cchDel, 
			DWORD cchNew, DWORD cpFormatMin, DWORD cpFormatMax );
	void NotifyPostReplaceRange( ITxNotify *pITNignore, DWORD cp, DWORD cchDel, 
			DWORD cchNew, DWORD cpFormatMin, DWORD cpFormatMax );

	CNotifyMgr();
	~CNotifyMgr();

private:

	ITxNotify *	_pitnlist;
};

#endif //_NOTMGR_H_
 

	
