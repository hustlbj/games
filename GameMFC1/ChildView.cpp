
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "GameMFC1.h"
#include "ChildView.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_PAINT 1
#define TIMER_HEROMOVE 2

#define DOWN 0
#define LEFT 1
#define RIGHT 2
#define UP 3

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
//	ON_WM_CREATE()
ON_WM_CREATE()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	m_bg.Load("res\\bg.bmp"); //载入背景图片
	MyHero.hero.Load("res\\heroMove.png"); //使用CImage对象载入png图片，资源相对路径
	CChildView::TranspatentPNG(&MyHero.hero); //使背景色透明，原始图像的背景本身要是透明的

	//初始化英雄状态
	MyHero.direct = UP;
	MyHero.frame = 0;
	//设置英雄的初始位置
	MyHero.x = 100;
	MyHero.y = 400;
	MyHero.width = 80;
	MyHero.height = 80;

	return TRUE;
}

void CChildView::OnPaint() 
{
	//CPaintDC dc(this); // 用于绘制的设备上下文
	
	//绘制图形使用的DC，目的DC
	CDC *cDC = this->GetDC();
	GetClientRect(&m_client); //获得窗口尺寸

	//创建缓冲DC
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
	m_cacheDC.SelectObject(&m_cacheBitmap);

	//m_bg.Draw(*cDC, m_client); //画背景
	//将背景先画在缓冲DC上
	m_bg.Draw(m_cacheDC, m_client);
	//将英雄贴在缓冲DC上
	
	//如果不是CImage类贴图而是CBitmap，还可以使用cDC->TransparentBlt()来绘制背景色透明的贴图
	//函数参数列表：目的DC对象，起始x，起始y，宽度，高度，源图是根据帧数和方向从大图中截取一部分
	MyHero.hero.Draw(m_cacheDC, MyHero.x, MyHero.y, MyHero.width, MyHero.height, MyHero.frame*MyHero.width, MyHero.direct*MyHero.height, MyHero.width, MyHero.height); //画英雄
	
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
	//绘制完一遍后，使窗口有效。没有这一句，系统会一直产生WM_PAINT消息。。。
	ValidateRect(&m_client);

	//释放缓冲DC
	m_cacheDC.DeleteDC();
	//释放背景图片对象
	m_cacheBitmap.DeleteObject();
	//释放窗口DC
	ReleaseDC(cDC);
	
}

void CChildView::TranspatentPNG(CImage *png)
{
	for (int i = 0; i < png->GetWidth(); i++)
	{
		for (int j = 0; j < png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char*>(png->GetPixelAddress(i, j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'd':
	case 'D':
		MyHero.direct = RIGHT;
		MyHero.x  += 5;
		break;
	case 'a':
	case 'A':
		MyHero.direct = LEFT;
		MyHero.x -= 5;
		break;
	case 'w':
	case 'W':
		MyHero.direct = UP;
		MyHero.y -= 5;
		break;
	case 's':
	case 'S':
		MyHero.direct = DOWN;
		MyHero.y += 5;
		break;
	case 't':
	case 'T':
		SetTimer(1, 100, NULL);
		break;
	case 'i':
	case 'I':
		KillTimer(1);
		break;
	default:
		break;
	}
	InvalidateRect(&m_client);
	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	char bufPos[50];
	sprintf(bufPos, "你单机了点X:%d,Y:%d", point.x, point.y);
	AfxMessageBox(bufPos);
	//InvalidateRect(&m_client);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_PAINT:
		InvalidateRect(&m_client); //使窗口无效，产生WM_PAINT消息，重绘窗口
		break;
	case TIMER_HEROMOVE:
		MyHero.frame ++;
		if (MyHero.frame == 4)
			MyHero.frame = 0;
	default:
		break;
	}
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(TIMER_PAINT, 10, NULL);
	SetTimer(TIMER_HEROMOVE, 100, NULL);
	return 0;
}
