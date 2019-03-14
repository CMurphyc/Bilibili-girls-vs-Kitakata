// Torpedo.h: interface for the CTorpedo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Torpedo_H__EBBACAC6_1744_11D5_92C1_F7CF8823B376__INCLUDED_)
#define AFX_Torpedo_H__EBBACAC6_1744_11D5_92C1_F7CF8823B376__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyObject.h"


#define TORPEDO_WIDTH	16
#define TORPEDO_HEIGHT	16

class CTorpedo : public CMyObject  
{
public:
	static void DeleteImage();
	static BOOL LoadImage();
	
	CRect GetRect() { return CRect(m_ptPos, CSize(TORPEDO_WIDTH, TORPEDO_HEIGHT)); }
	int GetType() { return m_nType; }
	bool Draw(CDC* pDC, bool bPause);
	CTorpedo( CPoint ptPos );
	virtual ~CTorpedo();

private:
	int m_nType;
	int m_nSpeed;
	int m_nDelay;

	static CImageList m_Images;

};

#endif // !defined(AFX_Torpedo_H__EBBACAC6_1744_11D5_92C1_F7CF8823B376__INCLUDED_)
