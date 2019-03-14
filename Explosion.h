// Explosion.h: interface for the CExplosion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPLOSION_H__9939D476_C570_11D4_A7CC_EA7F379C1C5F__INCLUDED_)
#define AFX_EXPLOSION_H__9939D476_C570_11D4_A7CC_EA7F379C1C5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyObject.h"

#define EXPLOSION_WIDTH		62
#define EXPLOSION_HEIGHT	63

class CExplosion : public CMyObject  
{
public:
	static void DeleteImage();
	static BOOL LoadImage();

	CRect GetRect() { return CRect(m_ptPos+CPoint(10,20), CSize(EXPLOSION_WIDTH-20, EXPLOSION_HEIGHT-40)); }
	int GetMulti()	{ return m_nMulti; }
	void SetMulti(int Multi)  { m_nMulti = Multi; }
	bool Draw(CDC* pDC, bool bPause);
	CExplosion(CPoint ptPos);
	virtual ~CExplosion();

private:
	int m_nProcess;
	// when an explosion trigger another explosion, the score should multiple
	int m_nMulti;
	
	static CImageList m_Images;
};

#endif // !defined(AFX_EXPLOSION_H__9939D476_C570_11D4_A7CC_EA7F379C1C5F__INCLUDED_)
