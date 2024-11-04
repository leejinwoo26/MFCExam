#pragma once
#include "afxdialogex.h"
#include "MFCAppDlg.h"
// CDiaImage 대화 상자

class CDiaImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaImage)

public:
	CDiaImage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDiaImage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDiaImage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	CImage* nImage;
	CClientDC* nDc;
	float currentRadius;
public:
	struct Vector2
	{
		float x;
		float y;
		Vector2(float x, float y) :
			x(x), y(y) {}
	};
private:
	void Clear(); // 배경클리어
	void DrawCenter(CImage* image);// 도트선 및 X모양선 그림
	bool Check(float x, float y);//영역 검사
	bool isInCircle(float x, float y, float centerX, float centerY, float radius);//원안에 있는지 검사
public:
	void Draw(const Pos& position); //Draw 버튼 클릭시
	void Save();//Save 버튼 클릭시
	void Load();//Load 버튼 클릭시
	void Action(Pos& position);//Action 버튼 클릭시
	afx_msg void OnDestroy();
};
