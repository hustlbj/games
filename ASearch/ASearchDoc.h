
// ASearchDoc.h : CASearchDoc 类的接口
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
protected: // 仅从序列化创建
	CASearchDoc();
	DECLARE_DYNCREATE(CASearchDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CASearchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	//地图的图元，方格
	struct MapCell
	{
		//是否可通过
		bool state; //1

		//是否锁定
		bool lock; //1

		//中心位置
		CPoint point; //8
		
		//填充颜色
		COLORREF color; //4

		int G; //4
		int H; //4

		MapCell * father; //4

		MapCell * next; //4
	};

	typedef MapCell Node;
	typedef MapCell * PtrToNode;

	//地图二维数组
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
