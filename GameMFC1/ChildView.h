
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once


// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:
	CRect m_client; //�ͻ�����С
	CDC m_bgcDC;  //�ݴ�ͼƬ��ԴDC
	CBitmap m_bgBitmap;//����ͼƬbitmap
	CImage m_bg; //����ͼƬimage
	CRect m_heroPos; //����Ӣ�۵�λ��
	CImage m_hero; //Ӣ��ͼƬ
// ����
public:
	static void TranspatentPNG(CImage *png);

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

