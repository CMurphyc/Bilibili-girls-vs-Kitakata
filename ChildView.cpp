

#include "stdafx.h"
#include "graphics.h"
#include "ShipDemo.h"
#include "ChildView.h"

#include "Explosion.h"
#include "MyObject.h"
#include "Score.h"
#include "MyShip.h"
#include "Submarine.h"
#include "Bomb.h"
#include "Torpedo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SHIP_VERT_POS		20
#define SKY_COLOR			RGB(0, 255, 255)
#define SEA_COLOR			RGB(0, 128, 255)
#define MAX_BOMBNUM 10
//indicate m_Objects[]     
//Fox example: use 'm_Objects[MYSHIP]' to handle "my ship"
#define MYSHIP		0
#define SUBMARINE	1
#define MY_BOMB		2
#define	ENEMY_BOMB	3
#define	EXPLOSION	4
#define	SCORE		5




/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
// load image list;
	CExplosion::LoadImage();
	CMyShip::LoadImage();
	CSubmarine::LoadImage();
	CBomb::LoadImage();
	CTorpedo::LoadImage();


// create my ship object, this is the 1st object in object list
	m_Objects[MYSHIP].AddTail(new CMyShip(CPoint((GAME_WIDTH-SHIP_WIDTH)*1/3, SHIP_VERT_POS),0 ));
	m_Objects[MYSHIP].AddTail(new CMyShip(CPoint((GAME_WIDTH-SHIP_WIDTH)*2/3, SHIP_VERT_POS),1 ));
	m_bGamePause = false;
}

CChildView::~CChildView()
{
// free memory

// delete all objects
	POSITION pos = m_Objects[MYSHIP].GetHeadPosition();
	int i;
	for(i=0;i<6;i++)
	{
		while(pos!=NULL)
		{
			delete m_Objects[i].GetNext(pos);
		}
		m_Objects[i].RemoveAll();
	}

// clear image lists
	CExplosion::DeleteImage();
	CMyShip::DeleteImage();
	CSubmarine::DeleteImage();
	CBomb::DeleteImage();
	CTorpedo::DeleteImage();
	
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_GAMEPAUSE, OnGamepause)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

//	cs.dwExStyle |= WS_EX_CLIENTEDGE;  // Wanghaitao remove this line
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	// TODO: Add your message handler code here
	CPaintDC dc(this); // device context for painting

// repaint window so easily
	dc.BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, &m_VirtualDC, 0, 0, SRCCOPY);

	// Do not call CWnd::OnPaint() for painting messages
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

// create virtual back buffer 
	CClientDC dc(this);
	m_VirtualBitmap.CreateCompatibleBitmap(&dc, GAME_WIDTH, GAME_HEIGHT);
	m_VirtualDC.CreateCompatibleDC(&dc);
	m_VirtualDC.SelectObject(&m_VirtualBitmap);

// create loop
	SetTimer(100, 30, NULL);
	srand( (unsigned)time( NULL ) );
	return 0;
}

void CChildView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int nCreator = random(15) +15 ;
	CClientDC dc(this);

	int i,j,k;
	CRect l_Rect;
	CMyObject* pObject;

	if(m_bGamePause)
		return;

// create new sub
	if(nCreator==0)
	{
		m_Objects[SUBMARINE].AddTail(new CSubmarine(random(2)%2?true:false, random(GAME_HEIGHT-SHIP_VERT_POS-SHIP_HEIGHT*2-30)+SHIP_VERT_POS+SHIP_HEIGHT+30, random(4), random(4)+3));
		nCreator = random(15) + 15;
	}

// Draw sky and see
	m_VirtualDC.FillSolidRect(0, 0, GAME_WIDTH, SHIP_VERT_POS+SHIP_HEIGHT-5, SKY_COLOR);
	m_VirtualDC.FillSolidRect(0, SHIP_VERT_POS+SHIP_HEIGHT-5, GAME_WIDTH, GAME_HEIGHT, SEA_COLOR);

// Draw all objects on the virtual dc, Be careful using "POSITION"
	POSITION pos1, pos2, pos3, pos4;
	CSubmarine* pSub;
	CMyShip* pMyShip;
	if(m_Objects[MYSHIP].GetCount()!=0)
		pMyShip=(CMyShip*)m_Objects[MYSHIP].GetHead();//Get my ship

	for(j=0;j<6;j++)
	for(i=0,pos1=m_Objects[j].GetHeadPosition(); (pos2=pos1)!=NULL; i++)
   	{
		pObject=(CMyObject*)m_Objects[j].GetNext(pos1);

		if(pObject->Draw(&m_VirtualDC, m_bGamePause))  // object terminated
		{
			m_Objects[j].RemoveAt(pos2);
			delete pObject;
		}
	//submarine throw torpedo
		else if(m_Objects[MYSHIP].GetCount()!=0 && j==SUBMARINE ) 
		{
			//double "my ship"
			for(k=0;k<m_Objects[MYSHIP].GetCount();k++)
			{
				if(0 == k)
					pMyShip=(CMyShip*)m_Objects[MYSHIP].GetHead();
				if(1 == k)
					pMyShip=(CMyShip*)m_Objects[MYSHIP].GetTail();
				//if "my ship" is above
				if( pObject->GetPos().x+SUBMARINE_WIDTH/2 > pMyShip->GetPos().x &&
				pObject->GetPos().x+SUBMARINE_WIDTH/2 < pMyShip->GetPos().x+SHIP_WIDTH )
				{
				//reduce the torpedo number
					if( ((CSubmarine*)pObject)->GetFireFlag() && random(10)==2)
					{
					CTorpedo *pTorpedo=new CTorpedo( CPoint(pObject->GetPos().x+SUBMARINE_WIDTH/2-TORPEDO_WIDTH/2, pObject->GetPos().y) );
					m_Objects[ENEMY_BOMB].AddTail(pTorpedo);
					((CSubmarine*)pObject)->SetFireFlag();
					}
				}//if
			}//for(i=0;i<2;i++)
		}
	}//for

//Find collision between explosion and submarine
//Notice that an explosion can trigger an explosion of the submarines nearby

	for(pos1=m_Objects[EXPLOSION].GetHeadPosition(); (pos2=pos1)!=NULL;)
	{
			pObject=(CMyObject*)m_Objects[EXPLOSION].GetNext(pos1);
		
			for(pos3=m_Objects[SUBMARINE].GetHeadPosition(); (pos4=pos3)!=NULL;)
   			{
					pSub = (CSubmarine*)m_Objects[SUBMARINE].GetNext(pos3);  
					l_Rect=pObject->GetRect();

					//if there is collision between explosion and submarine
					if( (l_Rect.IntersectRect(pSub->GetRect(), l_Rect )) )
					{
						// here we first create explosion object to make score always above that explosion
						CExplosion *pExplosion=new CExplosion(pSub->GetPos());

						// when an explosion trigger another explosion, the score should multiple
						pExplosion->SetMulti( ((CExplosion*)pObject)->GetMulti()+1 );

						m_Objects[EXPLOSION].AddTail(pExplosion);
						m_Objects[SCORE].AddTail(new CScore(pSub->GetPos(), pExplosion->GetMulti(), pSub->GetType()+1, RGB(random(255), random(255), random(255))));
						
						m_Objects[SUBMARINE].RemoveAt(pos4);
						delete pSub;
						break;
											
					}//if
			}//for
						
	}//for

//Find bombs which shoot on submarine
	for(pos1=m_Objects[MY_BOMB].GetHeadPosition(); (pos2=pos1)!=NULL;)
	{
			pObject=(CMyObject*)m_Objects[MY_BOMB].GetNext(pos1);
			
			for(pos3=m_Objects[SUBMARINE].GetHeadPosition(); (pos4=pos3)!=NULL;)
   			{
					pSub = (CSubmarine*)m_Objects[SUBMARINE].GetNext(pos3);  // save for deletion
					l_Rect=((CBomb*)pObject)->GetRect();

					//if there is collision between this bomb and submarine
					if( (l_Rect.IntersectRect(pSub->GetRect(), l_Rect )) )
					{
						// here we first create explosion object to make score always above that explosion
						m_Objects[EXPLOSION].AddTail(new CExplosion(pSub->GetPos()));
						m_Objects[SCORE].AddTail(new CScore(pSub->GetPos(),1,pSub->GetType()+1, RGB(random(255), random(255), random(255))));
						
						m_Objects[SUBMARINE].RemoveAt(pos4);
						delete pSub;
						m_Objects[MY_BOMB].RemoveAt(pos2);					
						delete pObject;
						// break is important! to avoid next recycle use a bomb not exist
						break;
											
					}//if
			}//for
						
	}//for
	
//Find enemy bombs which shoot on "my ship"
	if(m_Objects[MYSHIP].GetCount()!=0)
	for(pos1=m_Objects[ENEMY_BOMB].GetHeadPosition(); (pos2=pos1)!=NULL;)
	{
			pObject=(CMyObject*)m_Objects[MY_BOMB].GetNext(pos1);
		
			
			for(pos3=m_Objects[MYSHIP].GetHeadPosition(); (pos4=pos3)!=NULL;)
   			{
					pMyShip = (CMyShip*)m_Objects[MYSHIP].GetNext(pos3);  
					l_Rect=pObject->GetRect();

					//if there is collision between this torpedo and "my ship"
					if( (l_Rect.IntersectRect(pMyShip->GetRect(), l_Rect )) )
					{
						// here we first create explosion object to make score always above that explosion
						m_Objects[EXPLOSION].AddTail(new CExplosion(pMyShip->GetPos()));
						
						m_Objects[MYSHIP].RemoveAt(pos4);
						delete pMyShip;
						m_Objects[ENEMY_BOMB].RemoveAt(pos2);					
						delete pObject;

						// break is important! to avoid next recycle use a bomb not exist
						break;
											
					}//if
			}//for
						
	}//for
// Game over
	if(m_Objects[MYSHIP].GetCount()==0)
	{
		m_VirtualDC.SetTextColor(RGB(255, 0, 0));
		m_VirtualDC.SetTextAlign(TA_CENTER);
		m_VirtualDC.TextOut(GAME_WIDTH/2, GAME_HEIGHT/2,"GAME OVER");
	}
// restore old text align
	m_VirtualDC.SetTextAlign(TA_LEFT);
// sample: calculate objects numbers
	m_VirtualDC.SetBkMode(TRANSPARENT);
	m_VirtualDC.SetTextColor(RGB(255, 0, 0));
	CString string;
	string.Format("Total Enemy: %d , Total Bomb: %d", m_Objects[SUBMARINE].GetCount(),m_Objects[MY_BOMB].GetCount() );
	m_VirtualDC.TextOut(10, 10, string);
// sample: calculate total score
	string.Format("Your score: %d",CScore::GetTotalScore() );
	m_VirtualDC.TextOut(300, 10, string);


// blt the virtual dc on client dc
	dc.BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, &m_VirtualDC, 0, 0, SRCCOPY);

	nCreator--;

//	CWnd ::OnTimer(nIDEvent);  // no use, safely remove it
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bGamePause)
		return;
	if(m_Objects[MYSHIP].GetCount()==0)//my ship is not exist
		return;
//double "my ship"
	CMyShip* pShip1 = (CMyShip*)m_Objects[MYSHIP].GetHead(); // remember that our ship1 is the first object[MYSHIP] in object list
	CMyShip* pShip2 = (CMyShip*)m_Objects[MYSHIP].GetTail(); // remember that our ship2 is the last object[MYSHIP] in object list

	switch(nChar)
	{
	case VK_LEFT:       // turn left
		pShip1->SetMotion(-1);
		break;
	
	case VK_RIGHT: // turn right
		pShip1->SetMotion(1);
		break;
	case 'a':
	case 'A':       // turn left
		pShip2->SetMotion(-1);
		break;
	case 'd':
	case 'D':		// turn right
		pShip2->SetMotion(1);
		break;
	
	case VK_SPACE: // throw bomb from middle
		if( m_Objects[MY_BOMB].GetCount()<MAX_BOMBNUM )
		m_Objects[MY_BOMB].AddTail(new CBomb(pShip2->GetPos().x+SHIP_WIDTH/2-BOMB_WIDTH/2, 3));
		break;

	case VK_RETURN: // throw bomb from middle
		if( m_Objects[MY_BOMB].GetCount()<MAX_BOMBNUM )
		m_Objects[MY_BOMB].AddTail(new CBomb(pShip1->GetPos().x+SHIP_WIDTH/2-BOMB_WIDTH/2, 3));
		break;
	case 'o':
	case 'O':	// throw bomb on left side
		if( m_Objects[MY_BOMB].GetCount()<MAX_BOMBNUM )
		m_Objects[MY_BOMB].AddTail(new CBomb(pShip1->GetPos().x-BOMB_WIDTH/2, 3));
		break;
	
	case 'p':
	case 'P':	// throw bomb on right side
		if( m_Objects[MY_BOMB].GetCount()<MAX_BOMBNUM )
		m_Objects[MY_BOMB].AddTail(new CBomb(pShip1->GetPos().x+SHIP_WIDTH-BOMB_WIDTH/2, 3));
		break;
	
	case 'b':
	case 'B':	// throw bomb on left side
		if( m_Objects[MY_BOMB].GetCount()<MAX_BOMBNUM )
		m_Objects[MY_BOMB].AddTail(new CBomb(pShip2->GetPos().x-BOMB_WIDTH/2, 3));
		break;
	
	case 'n':
	case 'N':	// throw bomb on right side
		if( m_Objects[MY_BOMB].GetCount()<MAX_BOMBNUM )
		m_Objects[MY_BOMB].AddTail(new CBomb(pShip2->GetPos().x+SHIP_WIDTH-BOMB_WIDTH/2, 3));
		break;
	}//switch

//	CWnd ::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnDestroy() 
{
	CWnd ::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(100);
}

void CChildView::OnGamepause() 
{
	// TODO: Add your command handler code here
	m_bGamePause = !m_bGamePause;  // toggle pause status
	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);
	AfxGetMainWnd()->SetWindowText(m_bGamePause ? strTitle+_T(" - ÔÝÍ£") : strTitle);
}

void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bGamePause)
		return;
	if(m_Objects[MYSHIP].GetCount()==0)
		return;
//double "my ship"
	CMyShip* pShip1 = (CMyShip*)m_Objects[MYSHIP].GetHead();
	CMyShip* pShip2 = (CMyShip*)m_Objects[MYSHIP].GetTail();
	switch(nChar)
	{
	case 'a':
	case 'A':       // turn left
		if( pShip2->GetMotion() == -1 )       // turn left
			pShip2->SetMotion(0);
		break;
	case 'd':
	case 'D':		// turn right
		if( pShip2->GetMotion()==1 ) // turn right
			pShip2->SetMotion(0);
		break;
	case VK_LEFT:
		if( pShip1->GetMotion() == -1 )       // turn left
			pShip1->SetMotion(0);
		
		break;
	case VK_RIGHT:
		if( pShip1->GetMotion()==1 ) // turn right
			pShip1->SetMotion(0);
		
		break;
	}
	//	CWnd ::OnKeyUp(nChar, nRepCnt, nFlags);
}
