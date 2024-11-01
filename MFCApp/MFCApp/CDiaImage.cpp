// CDiaImage.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApp.h"
#include "afxdialogex.h"
#include "CDiaImage.h"
#include <iostream>


// CDiaImage 대화 상자

IMPLEMENT_DYNAMIC(CDiaImage, CDialogEx)

CDiaImage::CDiaImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDiaImage, pParent)
{
	this->Create(IDD_CDiaImage, this);
	this->ShowWindow(SW_SHOW);
	this->SetWindowPos(NULL, 50, 387.5f, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	
	nImage = new CImage();
	nDc = new CClientDC(this);
	srand(NULL);
}

CDiaImage::~CDiaImage()
{
	OnDestroy();
}

void CDiaImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CDiaImage::Clear() 
{
	if (nImage != NULL)
		nImage->Destroy();
	CRect rect;
	GetClientRect(&rect);
	int nWidth = rect.Width();
	int nHeight = rect.Height();
	int nBpp = 8;
	nImage->Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i; // 그레이 스케일 설정
		}
		nImage->SetColorTable(0, 256, rgb);
	}
	unsigned char* fm = (unsigned char*)nImage->GetBits();
	memset(fm, 100, nWidth * nHeight);
}
void CDiaImage::Action(Pos& position)
{
	Clear();
	UpdateData(true);
	static float X = 0;
	static float Y = 0;
	this->ShowWindow(SW_SHOW);

	float radius = currentRadius;
	int nPitch = nImage->GetPitch();

	CRect circle(
		position.nX1 - radius,
		position.nY1 - radius,
		position.nX1 + radius,
		position.nY1 + radius);;
	unsigned char* fm = (unsigned char*)nImage->GetBits();
	int centerX = static_cast<int>(position.nX1 + X); // 원의 중심 X 좌표
	int centerY = static_cast<int>(position.nY1 + Y); // 원의 중심 Y 좌표

	float dx = position.nX2 - position.nX1;
	float dy = position.nY2 - position.nY1;

	float distance = std::sqrt(dx * dx + dy * dy);

	float speed = 1.0f;

	// 이동 완료 여부 체크
	if (distance > speed) {
		// 방향 정규화
		float length = std::sqrt(dx * dx + dy * dy);
		float normalX = dx / length; // 정규화된 X 성분
		float normalY = dy / length; // 정규화된 Y 성분

		// 위치 업데이트
		X += normalX * speed; // 이동
		Y += normalY * speed; // 이동
		position.nX1 = centerX;
		position.nY1 = centerY;
		UpdateData(false);
	}
	else {
		// 목표에 도달했을 때
		X = position.nX2 - position.nX1; // 현재 위치로 조정
		Y = position.nY2 - position.nY1; // 현재 위치로 조정
	}

	for (int y = centerY - radius; y < centerY + radius; y++)
	{
		for (int x = centerX - radius; x < centerX + radius; x++) {
			if (Check(x, y) && isInCircle(x, y, centerX, centerY, radius))
			{
				fm[y * nPitch + x] = 0xff; // 픽셀 색상 설정
			}
		}
	}
	nImage->Draw(*nDc, 0, 0);
	Save();
}
void CDiaImage::Save() 
{
	static int SaveNum = 0;
	CString fileName;
	fileName.Format(_T("image\\image_%d.bmp"), SaveNum);
	nImage->Save(fileName);
	SaveNum++;
}
void CDiaImage::Load() 
{
	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Image Files (*.bmp; *.jpg;)|*.bmp;*.jpg;|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();

		if (nImage != NULL)
			nImage->Destroy();

		if (nImage->Load(filePath) == S_OK)
		{
			nImage->Draw(*nDc, 0, 0);

			unsigned char* fm = (unsigned char*)nImage->GetBits();
			int nWidth = nImage->GetWidth();
			int nHeight = nImage->GetHeight();
			int nPitch = nImage->GetPitch(); 

			int count = 0;
			int sumX = 0, sumY = 0;
			CRect rect(0, 0, nWidth, nHeight);

			for (int y = rect.top; y < rect.bottom; y++) {
				for (int x = rect.left; x < rect.right; x++) {
					if (fm[y * nPitch + x] == 0xff) {
						sumX += x; 
						sumY += y; 
						count++;
					}
				}
			}

			if (count > 0) {
				double centerX = (double)sumX / count; 
				double centerY = (double)sumY / count; 
				std::cout << centerX << std::endl;
				std::cout << centerY << std::endl;
			}
		}
		else
		{
			AfxMessageBox(_T("파일을 로드할수 없습니다."));
		}
	}
}

bool CDiaImage::Check(float x, float y) 
{
	CRect rect;
	GetClientRect(&rect);
	return rect.PtInRect(CPoint(x,y));
}
bool CDiaImage::isInCircle(float x, float y, float centerX, float centerY, float radius) 
{
	int dx = x - centerX;
	int dy = y - centerY;
	if (dx * dx + dy * dy <= radius * radius)
		return true;
	else
		return false;
}
void CDiaImage::Draw(const Pos& position) 
{
	Clear();

	this->ShowWindow(SW_SHOW);

	float radius = rand() % 50 + 50;
	currentRadius = radius;
	int nPitch = nImage->GetPitch();

	CRect circle(
		position.nX1 - radius,
		position.nY1 - radius,
		position.nX1 + radius,
		position.nY1 + radius);;
	unsigned char* fm = (unsigned char*)nImage->GetBits();


	for (int y = position.nY1 -radius; y < position.nY1 +radius; y++)
	{
		for (int x = position.nX1 - radius; x < position.nX1 +radius; x++) {
			if (Check(x, y) && isInCircle(x,y, position.nX1, position.nY1,radius))
				fm[y * nPitch + x] = 0xff;  
		}
	}
	int nWidth = nImage->GetWidth();
	int nHeight = nImage->GetHeight();

	int count = 0;
	int sumX = 0, sumY = 0;
	CRect rect(0, 0, nWidth, nHeight);

	for (int y = rect.top; y < rect.bottom; y++) {
		for (int x = rect.left; x < rect.right; x++) {
			if (fm[y * nPitch + x] == 0xff) {
				sumX += x;
				sumY += y;
				count++;
			}
		}
	}

	if (count > 0) {
		double centerX = (double)sumX / count;
		double centerY = (double)sumY / count;
		std::cout << centerX << std::endl;
		std::cout << centerY << std::endl;
	}

	nImage->Draw(*nDc, 0, 0);
}
BEGIN_MESSAGE_MAP(CDiaImage, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CDiaImage::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (nImage)
		delete nImage;
	if(nDc)
		delete nDc;
}
