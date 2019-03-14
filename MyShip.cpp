

#include "stdafx.h"
#include "ShipDemo.h"
#include "MyShip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageList CMyShip::m_Images;

CMyShip::CMyShip(CPoint ptPos, int nIndex)
{
	m_ptPos = ptPos;
	m_nMotion = 0;
	m_nIndex=nIndex;
}

CMyShip::~CMyShip()
{

}

bool CMyShip::Draw(CDC *pDC, bool bPause)
{
	m_ptPos.x+=m_nMotion*5;
	if( m_ptPos.x> GAME_WIDTH-SHIP_WIDTH || m_ptPos.x<0 )
		m_ptPos.x-=m_nMotion*5;
	m_Images.Draw(pDC, m_nIndex, m_ptPos, ILD_TRANSPARENT);
	return false;
}

BOOL CMyShip::LoadImage()
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_SHIP);

	if(!m_Images.Create(SHIP_WIDTH, SHIP_HEIGHT, ILC_COLOR24|ILC_MASK, 2, 0))
		return FALSE;
	//use transparent
	m_Images.Add(&bmp, RGB(255, 0, 255));

	return TRUE;
}

void CMyShip::DeleteImage()
{
	m_Images.DeleteImageList();
}
