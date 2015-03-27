
// ASearchView.cpp : CASearchView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ASearch.h"
#endif

#include "ASearchDoc.h"
#include "ASearchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CASearchView

IMPLEMENT_DYNCREATE(CASearchView, CView)

BEGIN_MESSAGE_MAP(CASearchView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CASearchView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CASearchView ����/����

CASearchView::CASearchView()
{
	// TODO: �ڴ˴���ӹ������

}

CASearchView::~CASearchView()
{
}

BOOL CASearchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CASearchView ����

void CASearchView::OnDraw(CDC* /*pDC*/)
{
	CASearchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

	DrawMap();
	DrawMove();

	pDoc->FindPath();
	DrawPath();
}


// CASearchView ��ӡ


void CASearchView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CASearchView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CASearchView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CASearchView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CASearchView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CASearchView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CASearchView ���

#ifdef _DEBUG
void CASearchView::AssertValid() const
{
	CView::AssertValid();
}

void CASearchView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CASearchDoc* CASearchView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CASearchDoc)));
	return (CASearchDoc*)m_pDocument;
}
#endif //_DEBUG


// CASearchView ��Ϣ�������


void CASearchView::DrawMap(void)
{
	CClientDC aDC(this);
	CASearchDoc * pDoc = GetDocument();
	CPen * pen = new CPen(1, 2, RGB(0, 255, 0));
	aDC.SelectObject(pen);

	for(int i = 0; i < COLS; i ++)
	{
		for(int j = 0; j < ROWS; j ++)
		{
			aDC.Rectangle((pDoc->map[i][j].point.x - CELL_LENGTH / 2), (pDoc->map[i][j].point.y - CELL_LENGTH / 2), 
				(pDoc->map[i][j].point.x + CELL_LENGTH / 2), (pDoc->map[i][j].point.y + CELL_LENGTH / 2));

			CBrush * brush = new CBrush(pDoc->map[i][j].color);
			aDC.FillRect(CRect((pDoc->map[i][j].point.x - CELL_LENGTH / 2 + 2), (pDoc->map[i][j].point.y - CELL_LENGTH / 2 + 2), 
				(pDoc->map[i][j].point.x + CELL_LENGTH / 2 - 2), (pDoc->map[i][j].point.y + CELL_LENGTH / 2 - 2)), brush);
			/*aDC.FillSolidRect((pDoc->map[i][j].point.x - CELL_LENGTH / 2 + 2), (pDoc->map[i][j].point.y - CELL_LENGTH / 2 + 2), 
				(pDoc->map[i][j].point.x + CELL_LENGTH / 2 - 2), (pDoc->map[i][j].point.y + CELL_LENGTH / 2 - 2), pDoc->map[i][j].color);*/
			brush->DeleteObject();
		}
	}

	pen->DeleteObject();

	CBrush * brush = new CBrush(pDoc->map[pDoc->endX][pDoc->endY].color);
	aDC.FillRect(CRect((pDoc->map[pDoc->endX][pDoc->endY].point.x - CELL_LENGTH / 2 + 2), (pDoc->map[pDoc->endX][pDoc->endY].point.y - CELL_LENGTH / 2 + 2), (pDoc->map[pDoc->endX][pDoc->endY].point.x + CELL_LENGTH / 2 - 2), (pDoc->map[pDoc->endX][pDoc->endY].point.y + CELL_LENGTH / 2 - 2)), brush);
	brush->DeleteObject();
}


void CASearchView::DrawMove(void)
{
	CClientDC aDC(this);
	CASearchDoc * pDoc = GetDocument();
	CBrush * brush = new CBrush(pDoc->map[pDoc->startX][pDoc->startY].color);
	aDC.FillRect(CRect((pDoc->map[pDoc->startX][pDoc->startY].point.x - CELL_LENGTH / 2 + 2), (pDoc->map[pDoc->startX][pDoc->startY].point.y - CELL_LENGTH / 2 + 2), (pDoc->map[pDoc->startX][pDoc->startY].point.x + CELL_LENGTH / 2 - 2), (pDoc->map[pDoc->startX][pDoc->startY].point.y + CELL_LENGTH / 2 - 2)), brush);
	brush->DeleteObject();
}


void CASearchView::DrawPath(void)
{
	CASearchDoc* pDoc = GetDocument();
	CASearchDoc::PtrToNode pNode = &(pDoc->map[pDoc->endX][pDoc->endY]);
	int i = 1;
	CString temp;

	CClientDC aDC(this);
	CBrush * brush = new CBrush(RGB(100, 100, 100));
	while(pNode->father != NULL)
	{
		pNode = pNode->father;
		aDC.FillRect(CRect(pNode->point.x - CELL_LENGTH / 2 + 2, pNode->point.y - CELL_LENGTH / 2 + 2, pNode->point.x + CELL_LENGTH / 2 - 2, pNode->point.y + CELL_LENGTH / 2 - 2), brush);
		temp.Format(_T("%d"), i++);
	//	aDC.DrawText(temp, CRect(pNode->point.x - CELL_LENGTH / 2 + 2, pNode->point.y - CELL_LENGTH / 2 + 2, pNode->point.x + CELL_LENGTH / 2 - 2, pNode->point.y + CELL_LENGTH / 2 - 2), 2);
	}
	brush->DeleteObject();
}


//void CASearchView::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CView::OnLButtonDblClk(nFlags, point);
//}


void CASearchView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CASearchDoc * pDoc = GetDocument();
	pDoc->InitialMap();
	if(point.x < MAP_WIDTH && point.y < MAP_HEIGHT && IsAvaliable(point.x / CELL_LENGTH, point.y / CELL_LENGTH))
	{		
		pDoc->over = false;
		pDoc->endX = point.x / CELL_LENGTH;
		pDoc->endY = point.y / CELL_LENGTH;
		pDoc->map[pDoc->endX][pDoc->endY].color = RGB(255, 0, 0);
	}
	pDoc->UpdateAllViews(NULL);
	CView::OnLButtonDown(nFlags, point);
}


bool CASearchView::IsAvaliable(int x, int y)
{
	CASearchDoc * pDoc = GetDocument();
	if(x == 2 && (y == 2 || y == 3 || y == 4))
	{
		return false;
	}
	return true;
}
