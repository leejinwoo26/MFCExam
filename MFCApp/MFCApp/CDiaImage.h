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
	CImage* GetImage()  { return nImage; }
	void Clear();
	void Save();
	void Load();

	bool Check(float x, float y);
	bool isInCircle(float x, float y, float centerX, float centerY, float radius);
public:
	void Draw(const Pos& position);
	void Action(Pos& position);
	afx_msg void OnDestroy();
};
