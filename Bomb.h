// Missile.h: interface for the CBomb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISSILE_H__EBBACAC6_1744_11D5_92C1_F7CF8823B376__INCLUDED_)
#define AFX_MISSILE_H__EBBACAC6_1744_11D5_92C1_F7CF8823B376__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyObject.h"


#define BOMB_WIDTH	32
#define BOMB_HEIGHT 32

class CBomb : public CMyObject  
{
public:
	static void DeleteImage();
	static BOOL LoadImage();
	
	CRect GetRect() { return CRect(m_ptPos+CPoint(14,8), CSize(BOMB_WIDTH-28, BOMB_HEIGHT-16)); }
	int GetType() { return m_nType; }
	bool Draw(CDC* pDC, bool bPause);
	CBomb( int nVert, int nSpeed);
	virtual ~CBomb();

private:
	int m_nType;
	int m_nSpeed;

	static CImageList m_Images;

};

#endif // !defined(AFX_MISSILE_H__EBBACAC6_1744_11D5_92C1_F7CF8823B376__INCLUDED_)
