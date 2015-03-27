
// ASearchDoc.h : CASearchDoc ��Ľӿ�
//
#define MAP_WIDTH 300
#define MAP_HEIGHT 300
#define CELL_LENGTH 50
#define COLS MAP_WIDTH/CELL_LENGTH
#define ROWS MAP_HEIGHT/CELL_LENGTH
#define COST1 10
#define COST2 14

#pragma once


class CASearchDoc : public CDocument
{
protected: // �������л�����
	CASearchDoc();
	DECLARE_DYNCREATE(CASearchDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CASearchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	//��ͼ��ͼԪ������
	struct MapCell
	{
		//�Ƿ��ͨ��
		bool state; //1

		//�Ƿ�����
		bool lock; //1

		//����λ��
		CPoint point; //8
		
		//�����ɫ
		COLORREF color; //4

		int G; //4
		int H; //4

		MapCell * father; //4

		MapCell * next; //4
	};

	typedef MapCell Node;
	typedef MapCell * PtrToNode;

	//��ͼ��ά����
	MapCell map[MAP_HEIGHT / CELL_LENGTH][MAP_WIDTH / CELL_LENGTH];
	int startX, startY, endX, endY;
	MapCell * testPath;
	bool over;

	PtrToNode FindMinNode(void);
	void InsertNode(PtrToNode pNode);
	void FindRefCells(int cellX, int cellY);
	bool IsExist(PtrToNode pNode);
	void TestInsert(int x, int y, int fatherX, int fatherY, int cost);
	void FindPath(void);
	void InitialMap(void);
};
