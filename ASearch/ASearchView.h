
// ASearchView.h : CASearchView 类的接口
//

#pragma once


class CASearchView : public CView
{
protected: // 仅从序列化创建
	CASearchView();
	DECLARE_DYNCREATE(CASearchView)

// 特性
public:
	CASearchDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CASearchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void DrawMap(void);
	void DrawMove(void);
	void DrawPath(void);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool IsAvaliable(int x, int y);
};

#ifndef _DEBUG  // ASearchView.cpp 中的调试版本
inline CASearchDoc* CASearchView::GetDocument() const
   { return reinterpret_cast<CASearchDoc*>(m_pDocument); }
#endif

