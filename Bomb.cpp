// Missile.cpp: implementation of the CBomb class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShipDemo.h"
#include "Bomb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageList CBomb::m_Images;

CBomb::CBomb(int nVert, int nSpeed)
{
	m_ptPos.y = 50;
	m_ptPos.x = nVert;
	m_nSpeed = nSpeed;
}

CBomb::~CBomb()
{

}

bool CBomb::Draw(CDC *pDC, bool bPause)
{
	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	if(bPause)
		return false;

	m_ptPos.y += m_nSpeed;
	if(m_ptPos.y>GAME_HEIGHT)
		return true;
	
	return false;
}

BOOL CBomb::LoadImage()
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BOMB);

	if(!m_Images.Create(BOMB_WIDTH, BOMB_HEIGHT, ILC_COLOR24|ILC_MASK, 1, 0))
		return FALSE;

	m_Images.Add(&bmp, RGB(255, 0, 255));

	return TRUE;
}

void CBomb::DeleteImage()
{
	m_Images.DeleteImageList();
}