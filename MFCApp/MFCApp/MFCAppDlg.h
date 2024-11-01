
// MFCAppDlg.h: 헤더 파일
//

#pragma once

struct Pos
{
	float nX1;
	float nY1;
	float nX2;
	float nY2;
	Pos(float x1, float y1, float x2, float y2) :
		nX1(x1), nX2(x2), nY1(y1), nY2(y2) {}
};

// CMFCAppDlg 대화 상자
class CMFCAppDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCAppDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
private:
	class CDiaImage* dig;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	void MoveCircle();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedDraw();
	afx_msg void OnBnClickedAction();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	Pos position;
	afx_msg void OnDestroy();
};
