
// ASearchDoc.cpp : CASearchDoc ���ʵ��
//

#include "stdafx.h"
#include "math.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CASearchDoc ����/����

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

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CASearchDoc ���л�

void CASearchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CASearchDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CASearchDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CASearchDoc ���

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


// CASearchDoc ����

//�Ӳ����б�testPath���Ƴ���С�Ľڵ㣬ʱ�临��O(n)
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
			//����ʹp1ָ��pNodeǰһ���ڵ�
			if(pNode->G + pNode->H < minNode->G + minNode->H)
			{
				//����minNode��ǰһ���ڵ㣬�Ա�ɾ��
				p2 = p1;
				minNode = pNode;
			}
		}
		//�����б���ֻ��һ��ͷԪ��
		if(p1 == NULL)
		{
			testPath = NULL;
		}
		else
		{
			//ͷԪ����С
			if(p2 == NULL)
			{
				testPath = testPath->next;
			}
			else
			{
				//��testPath��ɾ��minNode�ڵ�
				p2->next = minNode->next;
				minNode->next = NULL;
				
			}
		}
		minNode->lock = true;
		return minNode;
	}

}


//��testPathͷ������һ���ڵ㣬ʱ�临�Ӷ�O(1)
void CASearchDoc::InsertNode(PtrToNode pNode)
{
	pNode->next = testPath;
	testPath = pNode;
}

//�ҳ�map[cellX][cellY]���ڵĽڵ㲢������ͨ���Ľڵ���뵽�����б���
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
	// ��������Ϊ �ڵ�ͼ��Χ��	ͼԪ��ͨ��	ͼԪδ����	ͼԪ����ǽ�սǽǴ�
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

//����������Խڵ��Ƿ���ڣ�ʱ�临�Ӷ�O(n)
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

//���Բ���ڵ��Ƿ�����뿪���б��У�������ڣ�����Ҫ�ж�Gֵ�Ƿ���Ҫ���£������������ֱ�Ӽ���
void CASearchDoc::TestInsert(int x, int y, int fatherX, int fatherY, int cost)
{
	if(IsExist(&map[x][y]))
	{
		int temp = map[fatherX][fatherY].G + cost;
		if(map[x][y].G > temp)
		{
			//����G
			map[x][y].G = temp;
			map[x][y].father = &map[fatherX][fatherY];
			//InsertNode(&map[x][y]);	
		}
	}
	else
	{
		//�¼����Ҫ����H
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
	
	// TODO: �ڴ����һ���Թ������
	for(int i = 0; i < COLS; i ++)
	{
		for(int j = 0; j < ROWS; j ++)
		{
			map[i][j].point.x = xBase + CELL_LENGTH * i;
			map[i][j].point.y = yBase + CELL_LENGTH * j;
			//��ʼ����ɫ���
			map[i][j].color = RGB(0, 0, 0);
			map[i][j].state = true;
			map[i][j].lock = false;
			map[i][j].father = NULL;
			map[i][j].next = NULL;
			
			map[i][j].G = 0;
			map[i][j].H = 0;
		}
	}

	//���ò���ͨ��ǽ��
	map[2][2].color = RGB(0, 100, 0);
	map[2][2].state = false;
	map[2][3].color = RGB(0, 100, 0);
	map[2][3].state = false;
	map[2][4].color = RGB(0, 100, 0);
	map[2][4].state = false;

	//�������
	map[startX][startY].color = RGB(0, 0, 255);

	//�����յ�
//	map[endX][endY].color = RGB(255, 0, 0);

	//������testPath��
	testPath = &map[startX][startY];

	//δ���
	over = false;
}
