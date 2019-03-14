// MyShip.h: interface for the CMyShip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSHIP_H__9939D478_C570_11D4_A7CC_EA7F379C1C5F__INCLUDED_)
#define AFX_MYSHIP_H__9939D478_C570_11D4_A7CC_EA7F379C1C5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyObject.h"

#define SHIP_WIDTH		64
#define SHIP_HEIGHT		27

class CMyShip : public CMyObject  
{
public:
	static void DeleteImage();
	static BOOL LoadImage();
	bool Draw(CDC* pDC, bool bPause);
	void SetPos(int x, int y) 
	{ 
		m_ptPos.x = x; 
		m_ptPos.y = y; 
	}
	void SetMotion(int motion)	{	m_nMotion = motion; }
	int GetMotion()				{	return	m_nMotion;	}
	CPoint GetPos()				{	return m_ptPos;		}
	CRect GetRect() { return CRect(m_ptPos+CPoint(10,5), CSize(SHIP_WIDTH-20, SHIP_HEIGHT-10)); }
	CMyShip(CPoint ptPos, int nIndex);
	virtual ~CMyShip();

private:
//My Ship's motion
//-1: Left 1: Right  0: inaction
	int m_nMotion;
	int m_nIndex;
	static CImageList m_Images;

};

#endif // !defined(AFX_MYSHIP_H__9939D478_C570_11D4_A7CC_EA7F379C1C5F__INCLUDED_)
