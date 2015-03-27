
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
	struct shero
	{
		CImage hero; //����Ӣ�۵�ͼ��
		int x;  //����Ӣ�۵�λ��
		int y;
		int width;
		int height;
		int direct;  //����Ӣ�۵ķ���
		int frame;  //�˶����ڼ���ͼƬ
	} MyHero;
	CRect m_client; //�ͻ�����С
	CDC m_cacheDC;  //����ͼƬ��ԴDC
	CBitmap m_cacheBitmap; //����λͼ 
	CImage m_bg; //����ͼƬimage

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

