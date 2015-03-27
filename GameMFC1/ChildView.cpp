
// ChildView.cpp : CChildView ���ʵ��
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



// CChildView ��Ϣ�������

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	//m_bgcDC.CreateCompatibleDC(NULL); //����һ�����ݵ�DC����ʱ�����Ҫ���Ƶ���Դ��ԴDC
	//m_bgBitmap.LoadBitmap(IDB_BITMAP1); //������ͼƬ�����ڴ�
	m_bg.Load("res\\bg.bmp"); //���뱳��ͼƬ
	m_hero.Load("res\\hero.png"); //ʹ��CImage��������pngͼƬ����Դ���·��
	CChildView::TranspatentPNG(&m_hero); //ʹ����ɫ͸����ԭʼͼ��ı�������Ҫ��͸����

	//����Ӣ�۵ĳ�ʼλ��
	m_heroPos.left = 100;
	m_heroPos.right = 100 + 60;
	m_heroPos.top = 400;
	m_heroPos.bottom = 400 + 60;

	return TRUE;
}

void CChildView::OnPaint() 
{
	//CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	//����ͼ��ʹ�õ�DC��Ŀ��DC
	CDC *cDC = this->GetDC();
	GetClientRect(&m_client); //��ô��ڳߴ�
	//m_bgcDC.SelectObject(&m_bgBitmap);
	//���������б�Ŀ��DC����ʼx��Ŀ��DC��ʼy����ȣ��߶ȣ�ԴDCָ�룬ԴDC��ʼx��ԴDC��ʼy����ͼ��ʽ
	//cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_bgcDC, 0, 0, SRCCOPY);
	m_bg.Draw(*cDC, m_client); //������

	//�������CImage����ͼ����CBitmap��������ʹ��cDC->TransparentBlt()�����Ʊ���ɫ͸������ͼ
	//���������б�Ŀ��DC������ʼx����ʼy����ȣ��߶�
	m_hero.Draw(*cDC, m_heroPos); //��Ӣ��
	

	ReleaseDC(cDC);
	// ��ҪΪ������Ϣ������ CWnd::OnPaint()

	//������һ���ʹ������Ч��û����һ�䣬ϵͳ��һֱ����WM_PAINT��Ϣ������
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
		InvalidateRect(&m_client); //ʹ������Ч������WM_PAINT��Ϣ���ػ洰��
		break;
	default:
		break;
	}
}

