
// ASearchView.h : CASearchView ��Ľӿ�
//

#pragma once


class CASearchView : public CView
{
protected: // �������л�����
	CASearchView();
	DECLARE_DYNCREATE(CASearchView)

// ����
public:
	CASearchDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CASearchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // ASearchView.cpp �еĵ��԰汾
inline CASearchDoc* CASearchView::GetDocument() const
   { return reinterpret_cast<CASearchDoc*>(m_pDocument); }
#endif

