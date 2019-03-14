

#include "stdafx.h"
#include "ShipDemo.h"
#include "Score.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CScore::m_nTotalScore=0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScore::CScore(CPoint ptPos,int nMuti, int nScore, DWORD nColor)
{
	m_ptPos = ptPos;
	m_nScore = nScore;
	m_nColor = nColor;
	m_nProcess = 0;
	m_nMuti = nMuti;
	m_nTotalScore += m_nMuti* m_nScore * 10;
}

CScore::~CScore()
{

}

bool CScore::Draw(CDC *pDC, bool bPause)
{
	pDC->SetTextColor(m_nColor);
	pDC->SetBkMode(TRANSPARENT);

	CString strScore;
	strScore.Format("%d0 X %d", m_nScore, m_nMuti);
	pDC->TextOut(m_ptPos.x, m_ptPos.y, strScore);

	if(bPause)
		return false;

	return m_nProcess++ > 50 ? true : false;
}
