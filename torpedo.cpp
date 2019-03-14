

#include "stdafx.h"
#include "ShipDemo.h"
#include "Torpedo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageList CTorpedo::m_Images;

CTorpedo::CTorpedo(CPoint ptPos)
{
	m_ptPos = ptPos;
	m_nSpeed = 1;
	m_nDelay = 10;
}

CTorpedo::~CTorpedo()
{

}

bool CTorpedo::Draw(CDC *pDC, bool bPause)
{
	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	if(bPause)
		return false;

	m_ptPos.y -= m_nSpeed;
	if(m_ptPos.y<35)
	{
		m_ptPos.y += m_nSpeed;
		m_nDelay --;
		if(m_nDelay==0)
			return true;
	}
	
	return false;
}

BOOL CTorpedo::LoadImage()
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_TORPEDO);

	if(!m_Images.Create(TORPEDO_WIDTH, TORPEDO_HEIGHT, ILC_COLOR24|ILC_MASK, 1, 0))
		return FALSE;

	m_Images.Add(&bmp, RGB(255, 0, 255));

	return TRUE;
}

void CTorpedo::DeleteImage()
{
	m_Images.DeleteImageList();
}