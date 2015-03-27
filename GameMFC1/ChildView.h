
// ChildView.h : CChildView 类的接口
//


#pragma once


// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	struct shero
	{
		CImage hero; //保存英雄的图像
		int x;  //保存英雄的位置
		int y;
		int width;
		int height;
		int direct;  //保存英雄的方向
		int frame;  //运动到第几张图片
	} MyHero;
	CRect m_client; //客户区大小
	CDC m_cacheDC;  //缓存图片的源DC
	CBitmap m_cacheBitmap; //缓冲位图 
	CImage m_bg; //背景图片image

// 操作
public:
	static void TranspatentPNG(CImage *png);

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
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

