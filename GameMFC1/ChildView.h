
// ChildView.h : CChildView 类的接口
//


#pragma once

#define SNOW_NUMBER 100

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	struct snow
	{
		int x;
		int y;
		int number; //粒子的编号，共七种粒子
	}Snow[SNOW_NUMBER];
	CImage m_snowMap[7]; //雪花图片

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

	int m_xMapStart; //x方向上地图的起始点
	int m_mapWidth; //背景地图的宽度

// 操作
public:
	static void TranspatentPNG(CImage *png);
	void GetMapStartX();
	int GetScreenX(int xHero, int mapWidth);
	void initSnow();
	void DrawSnow();

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

