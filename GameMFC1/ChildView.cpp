
// ChildView.cpp : CChildView ���ʵ��
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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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



// CChildView ��Ϣ�������

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	m_bg.Load("res\\bigbg.png"); //���뱳��ͼƬ
	m_mapWidth = m_bg.GetWidth();
	m_xMapStart = 0; //���õ�ͼ��ʼ������˿�ʼ��ʾ
	MyHero.hero.Load("res\\heroMove.png"); //ʹ��CImage��������pngͼƬ����Դ���·��
	CChildView::TranspatentPNG(&MyHero.hero); //ʹ����ɫ͸����ԭʼͼ��ı�������Ҫ��͸����

	//��ʼ��Ӣ��״̬
	MyHero.direct = UP;
	MyHero.frame = 0;
	//����Ӣ�۵ĳ�ʼλ��
	MyHero.x = 100;
	MyHero.y = 400;
	MyHero.width = 80;
	MyHero.height = 80;

	return TRUE;
}

//�����ͼ���x��ʼλ��
void CChildView::GetMapStartX()
{
	//������ﲻ������ߺ����ұ߰����Ļ��ʱ����ͼ����ʼ��������Ҫ��������λ�ü����
	if (MyHero.x < m_mapWidth - WINDOW_WIDTH / 2 && MyHero.x > WINDOW_WIDTH / 2)
		m_xMapStart = MyHero.x - WINDOW_WIDTH / 2;
}

//��ȡ��������Ļ�ϵ����꣨xHero�������ŵ�ͼ�ϵľ������꣬GetScreenX��������Ļ�ϵ�������꣩
int CChildView::GetScreenX(int xHero, int mapWidth)
{
	//������ﲻ������ߺ����ұ߰����Ļ��ʱ����ô����ͳ�����Ļ�м�
	if (xHero < mapWidth - WINDOW_WIDTH / 2 && xHero > WINDOW_WIDTH / 2)
		return WINDOW_WIDTH / 2;
	else if (xHero <= WINDOW_WIDTH / 2 && xHero >= 0) //������߰����Ļʱ����������Ļ�ϵ�λ�þ����Լ���x����
	{
		return xHero;
	}
	else if (xHero < 0) //������������Ӣ��ͼƬ�ܹ����ܳ������
	{
		MyHero.x = 0;
		return MyHero.x;
	}
	else if (xHero <= mapWidth - MyHero.width) //�����ұ߰����Ļ
	{
		return WINDOW_WIDTH - (mapWidth - xHero);
	}
	else //������������Ӣ��ͼƬ�ܹ����ܳ����ұ�
	{
		MyHero.x = mapWidth - MyHero.width;
		return WINDOW_WIDTH - (mapWidth - MyHero.x);
	}
		
}

void CChildView::OnPaint() 
{
	//CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	//����ͼ��ʹ�õ�DC��Ŀ��DC
	CDC *cDC = this->GetDC();
	GetClientRect(&m_client); //��ô��ڳߴ�

	//��������DC
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
	m_cacheDC.SelectObject(&m_cacheBitmap);

	GetMapStartX();

	//m_bg.Draw(*cDC, m_client); //������
	//�������Ȼ��ڻ���DC��
	//m_bg.Draw(m_cacheDC, m_client);
	m_bg.Draw(m_cacheDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, m_xMapStart, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//��Ӣ�����ڻ���DC��
	//�������CImage����ͼ����CBitmap��������ʹ��cDC->TransparentBlt()�����Ʊ���ɫ͸������ͼ
	//���������б�Ŀ��DC������ʼx����ʼy����ȣ��߶ȣ�Դͼ�Ǹ���֡���ͷ���Ӵ�ͼ�н�ȡһ����
	MyHero.hero.Draw(m_cacheDC, GetScreenX(MyHero.x, m_mapWidth), MyHero.y, MyHero.width, MyHero.height, MyHero.frame*MyHero.width, MyHero.direct*MyHero.height, MyHero.width, MyHero.height); //��Ӣ��
	
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);
	
	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
	//������һ���ʹ������Ч��û����һ�䣬ϵͳ��һֱ����WM_PAINT��Ϣ������
	ValidateRect(&m_client);

	//�ͷŻ���DC
	m_cacheDC.DeleteDC();
	//�ͷű���ͼƬ����
	m_cacheBitmap.DeleteObject();
	//�ͷŴ���DC
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
	sprintf(bufPos, "�㵥���˵�X:%d,Y:%d", point.x, point.y);
	AfxMessageBox(bufPos);
	//InvalidateRect(&m_client);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_PAINT:
		InvalidateRect(&m_client); //ʹ������Ч������WM_PAINT��Ϣ���ػ洰��
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

	//
	SetTimer(TIMER_PAINT, 10, NULL);
	SetTimer(TIMER_HEROMOVE, 100, NULL);
	return 0;
}
