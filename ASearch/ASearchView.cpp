
// ASearchView.cpp : CASearchView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CASearchView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CASearchView 构造/析构

CASearchView::CASearchView()
{
	// TODO: 在此处添加构造代码

}

CASearchView::~CASearchView()
{
}

BOOL CASearchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CASearchView 绘制

void CASearchView::OnDraw(CDC* /*pDC*/)
{
	CASearchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	DrawMap();
	DrawMove();

	pDoc->FindPath();
	DrawPath();
}


// CASearchView 打印


void CASearchView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CASearchView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CASearchView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CASearchView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CASearchView 诊断

#ifdef _DEBUG
void CASearchView::AssertValid() const
{
	CView::AssertValid();
}

void CASearchView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CASearchDoc* CASearchView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CASearchDoc)));
	return (CASearchDoc*)m_pDocument;
}
#endif //_DEBUG


// CASearchView 消息处理程序


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
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CView::OnLButtonDblClk(nFlags, point);
//}


void CASearchView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
