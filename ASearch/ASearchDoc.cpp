
// ASearchDoc.cpp : CASearchDoc 类的实现
//

#include "stdafx.h"
#include "math.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ASearch.h"
#endif

#include "ASearchDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CASearchDoc

IMPLEMENT_DYNCREATE(CASearchDoc, CDocument)

BEGIN_MESSAGE_MAP(CASearchDoc, CDocument)
END_MESSAGE_MAP()


// CASearchDoc 构造/析构

CASearchDoc::CASearchDoc()
{
	InitialMap();
}

CASearchDoc::~CASearchDoc()
{
}

BOOL CASearchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CASearchDoc 序列化

void CASearchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CASearchDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CASearchDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CASearchDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CASearchDoc 诊断

#ifdef _DEBUG
void CASearchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CASearchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CASearchDoc 命令

//从测试列表testPath中移出最小的节点，时间复杂O(n)
CASearchDoc::PtrToNode CASearchDoc::FindMinNode(void)
{
	if(testPath == NULL)
		return NULL;
	else
	{
		PtrToNode minNode = testPath;
		PtrToNode pNode = testPath;
		PtrToNode p1 = NULL, p2 = NULL;
		while(pNode->next != NULL)
		{
			p1 = pNode;
			pNode = pNode->next;
			//以上使p1指向pNode前一个节点
			if(pNode->G + pNode->H < minNode->G + minNode->H)
			{
				//缓存minNode的前一个节点，以便删除
				p2 = p1;
				minNode = pNode;
			}
		}
		//测试列表中只有一个头元素
		if(p1 == NULL)
		{
			testPath = NULL;
		}
		else
		{
			//头元素最小
			if(p2 == NULL)
			{
				testPath = testPath->next;
			}
			else
			{
				//从testPath中删除minNode节点
				p2->next = minNode->next;
				minNode->next = NULL;
				
			}
		}
		minNode->lock = true;
		return minNode;
	}

}


//在testPath头部插入一个节点，时间复杂度O(1)
void CASearchDoc::InsertNode(PtrToNode pNode)
{
	pNode->next = testPath;
	testPath = pNode;
}

//找出map[cellX][cellY]相邻的节点并将可以通过的节点加入到测试列表中
void CASearchDoc::FindRefCells(int cellX, int cellY)
{
	if(cellX - 1 >= 0 && cellY - 1 >= 0 && map[cellX - 1][cellY - 1].state && !map[cellX - 1][cellY - 1].lock)
	{
		/*map[cellX - 1][cellY - 1].father = &map[cellX][cellY];
		map[cellX - 1][cellY - 1].G = map[cellX][cellY].G + COST2;
		InsertNode(&map[cellX - 1][cellY - 1]);*/
		TestInsert(cellX - 1, cellY - 1, cellX, cellY, COST2);
		if(cellX - 1 == endX && cellY - 1 == endY)
		{
			over = true;
		}
	}
	if(cellY - 1 >= 0 && map[cellX][cellY - 1].state && !map[cellX][cellY - 1].lock)
	{
		/*map[cellX][cellY - 1].father = &map[cellX][cellY];
		map[cellX][cellY - 1].G = map[cellX][cellY].G + COST1;
		InsertNode(&map[cellX][cellY - 1]);*/
		TestInsert(cellX, cellY - 1, cellX, cellY, COST1);
		if(cellX == endX && cellY - 1 == endY)
		{
			over = true;
		}
	}
	// 条件依次为 在地图范围内	图元可通过	图元未锁定	图元不在墙拐角角处
	if(cellX + 1 < COLS && cellY - 1 >= 0 && map[cellX + 1][cellY - 1].state && !map[cellX + 1][cellY - 1].lock && !(cellX == 1 && cellY == 2) && !(cellX == 2 && cellY == 5))
	{
		/*map[cellX + 1][cellY - 1].father = &map[cellX][cellY];
		map[cellX + 1][cellY - 1].G = map[cellX][cellY].G + COST2;
		InsertNode(&map[cellX + 1][cellY - 1]);*/
		TestInsert(cellX + 1, cellY - 1, cellX, cellY, COST2);
		if(cellX + 1 == endX && cellY - 1 == endY)
		{
			over = true;
		}
	}
	if(cellX + 1 < COLS && map[cellX + 1][cellY].state && !map[cellX + 1][cellY].lock)
	{
		/*map[cellX + 1][cellY].father = &map[cellX][cellY];
		map[cellX + 1][cellY].G = map[cellX][cellY].G + COST1;
		InsertNode(&map[cellX + 1][cellY]);*/
		TestInsert(cellX + 1, cellY, cellX, cellY, COST1);
		if(cellX + 1 == endX && cellY == endY)
		{
			over = true;
		}
	}
	if(cellX + 1 < COLS  && cellY + 1 < ROWS && map[cellX + 1][cellY + 1].state && !map[cellX + 1][cellY + 1].lock && !(cellX == 1 && cellY == 4) && !(cellX == 2 && cellY == 1)) 
	{
	/*	map[cellX + 1][cellY + 1].father = &map[cellX][cellY];
		map[cellX + 1][cellY + 1].G = map[cellX][cellY].G + COST2;
		InsertNode(&map[cellX + 1][cellY + 1]);*/
		TestInsert(cellX + 1, cellY + 1, cellX, cellY, COST2);
		if(cellX + 1 == endX && cellY + 1 == endY)
		{
			over = true;
		}
	}
	if(cellY + 1 < ROWS && map[cellX][cellY + 1].state && !map[cellX][cellY + 1].lock)
	{
		/*map[cellX][cellY + 1].father = &map[cellX][cellY];
		map[cellX][cellY + 1].G = map[cellX][cellY].G + COST1;
		InsertNode(&map[cellX][cellY + 1]);*/
		TestInsert(cellX, cellY + 1, cellX, cellY, COST1);
		if(cellX == endX && cellY + 1 == endY)
		{
			over = true;
		}
	}
	if(cellX - 1 >= 0 && cellY + 1 < ROWS && map[cellX - 1][cellY + 1].state && !map[cellX - 1][cellY + 1].lock)
	{
		/*map[cellX - 1][cellY + 1].father = &map[cellX][cellY];
		map[cellX - 1][cellY + 1].G = map[cellX][cellY].G + COST2;
		InsertNode(&map[cellX - 1][cellY + 1]);*/
		TestInsert(cellX - 1, cellY + 1, cellX, cellY, COST2);
		if(cellX - 1 == endX && cellY + 1 == endY)
		{
			over = true;
		}
	}
	if(cellX - 1 >= 0 && map[cellX - 1][cellY].state && !map[cellX - 1][cellY].lock)
	{
		/*map[cellX - 1][cellY].father = &map[cellX][cellY];
		map[cellX - 1][cellY].G = map[cellX][cellY].G + COST1;
		InsertNode(&map[cellX - 1][cellY]);*/
		TestInsert(cellX - 1, cellY, cellX, cellY, COST1);
		if(cellX - 1 == endX && cellY == endY)
		{
			over = true;
		}
	}
}

//遍历链表测试节点是否存在，时间复杂度O(n)
bool CASearchDoc::IsExist(PtrToNode pNode)
{
	PtrToNode p1 = testPath;
	while(p1 != NULL)
	{
		if(p1 == pNode)
		{
			return true;
		}
		p1 = p1->next;
	}
	return false;
}

//测试插入节点是否存在与开放列表中，如果存在，则需要判断G值是否需要更新，如果不存在则直接加入
void CASearchDoc::TestInsert(int x, int y, int fatherX, int fatherY, int cost)
{
	if(IsExist(&map[x][y]))
	{
		int temp = map[fatherX][fatherY].G + cost;
		if(map[x][y].G > temp)
		{
			//更新G
			map[x][y].G = temp;
			map[x][y].father = &map[fatherX][fatherY];
			//InsertNode(&map[x][y]);	
		}
	}
	else
	{
		//新加入的要计算H
		map[x][y].H = COST1 * (int)(fabs((float)(map[x][y].point.x - map[endX][endY].point.x) / CELL_LENGTH) + fabs((float)(map[x][y].point.y - map[endX][endY].point.y) / CELL_LENGTH));
		map[x][y].G = map[fatherX][fatherY].G + cost;
		map[x][y].father = &map[fatherX][fatherY];
		InsertNode(&map[x][y]);	
	}
}


void CASearchDoc::FindPath(void)
{
	PtrToNode pNode = NULL;
	if(startX == endX && startY == endY)
	{
		over = true;
	}
	while(!over)
	{
		pNode = FindMinNode();
		if(pNode != NULL)
		{
			FindRefCells(pNode->point.x / CELL_LENGTH, pNode->point.y / CELL_LENGTH);
		}
	}
}


void CASearchDoc::InitialMap(void)
{
	int xBase = CELL_LENGTH / 2;
	int yBase = CELL_LENGTH / 2;
	startX = 0;
	startY = 3;
	endX = 0;
	endY = 3;
	
	// TODO: 在此添加一次性构造代码
	for(int i = 0; i < COLS; i ++)
	{
		for(int j = 0; j < ROWS; j ++)
		{
			map[i][j].point.x = xBase + CELL_LENGTH * i;
			map[i][j].point.y = yBase + CELL_LENGTH * j;
			//初始化黑色填充
			map[i][j].color = RGB(0, 0, 0);
			map[i][j].state = true;
			map[i][j].lock = false;
			map[i][j].father = NULL;
			map[i][j].next = NULL;
			
			map[i][j].G = 0;
			map[i][j].H = 0;
		}
	}

	//设置不可通过墙体
	map[2][2].color = RGB(0, 100, 0);
	map[2][2].state = false;
	map[2][3].color = RGB(0, 100, 0);
	map[2][3].state = false;
	map[2][4].color = RGB(0, 100, 0);
	map[2][4].state = false;

	//设置起点
	map[startX][startY].color = RGB(0, 0, 255);

	//设置终点
//	map[endX][endY].color = RGB(255, 0, 0);

	//起点加入testPath中
	testPath = &map[startX][startY];

	//未完成
	over = false;
}
