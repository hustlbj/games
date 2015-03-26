
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "GameMFC1.h"
#include "ChildView.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

	//m_bgcDC.CreateCompatibleDC(NULL); //创建一个兼容的DC，暂时存放需要绘制的资源，源DC
	//m_bgBitmap.LoadBitmap(IDB_BITMAP1); //将背景图片载入内存
	m_bg.Load("res\\bg.bmp"); //载入背景图片
	m_hero.Load("res\\hero.png"); //使用CImage对象载入png图片，资源相对路径
	CChildView::TranspatentPNG(&m_hero); //使背景色透明，原始图像的背景本身要是透明的

	//设置英雄的初始位置
	m_heroPos.left = 100;
	m_heroPos.right = 100 + 60;
	m_heroPos.top = 400;
	m_heroPos.bottom = 400 + 60;

	return TRUE;
}

void CChildView::OnPaint() 
{
	//CPaintDC dc(this); // 用于绘制的设备上下文
	
	//绘制图形使用的DC，目的DC
	CDC *cDC = this->GetDC();
	GetClientRect(&m_client); //获得窗口尺寸
	//m_bgcDC.SelectObject(&m_bgBitmap);
	//函数参数列表：目的DC的起始x，目的DC起始y，宽度，高度，源DC指针，源DC起始x，源DC起始y，贴图方式
	//cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_bgcDC, 0, 0, SRCCOPY);
	m_bg.Draw(*cDC, m_client); //画背景

	//如果不是CImage类贴图而是CBitmap，还可以使用cDC->TransparentBlt()来绘制背景色透明的贴图
	//函数参数列表：目的DC对象，起始x，起始y，宽度，高度
	m_hero.Draw(*cDC, m_heroPos); //画英雄
	

	ReleaseDC(cDC);
	// 不要为绘制消息而调用 CWnd::OnPaint()

	//绘制完一遍后，使窗口有效。没有这一句，系统会一直产生WM_PAINT消息。。。
	ValidateRect(&m_client);
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
		m_heroPos.left += 10;
		m_heroPos.right += 10;
		break;
	case 'a':
	case 'A':
		m_heroPos.left -= 10;
		m_heroPos.right -= 10;
		break;
	case 'w':
	case 'W':
		m_heroPos.top -= 10;
		m_heroPos.bottom -= 10;
		break;
	case 's':
	case 'S':
		m_heroPos.top += 10;
		m_heroPos.bottom += 10;
		break;
	case 't':
	case 'T':
		SetTimer(1, 100, NULL);
	case 'i':
	case 'I':
		KillTimer(1);
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
	m_heroPos.left = point.x;
	m_heroPos.right = m_heroPos.left + 60;
	m_heroPos.top = point.y;
	m_heroPos.bottom = m_heroPos.top + 60;
	InvalidateRect(&m_client);
	//CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		m_heroPos.left += 10;
		m_heroPos.right += 10;
		InvalidateRect(&m_client); //使窗口无效，产生WM_PAINT消息，重绘窗口
		//ee
		break;
	default:
		break;
	}
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#3016

	@R0
		return -1;

#3016

	@R0

#3016

		@R0
		// TODO:  在此添加您专用的创建代码

		@R0
		return 0;
}
