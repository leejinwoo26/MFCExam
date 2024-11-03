// CDiaImage.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApp.h"
#include "afxdialogex.h"
#include "CDiaImage.h"
#include <iostream>

using namespace std;


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
	//이미지 있을경우 파괴
	if (nImage != NULL)
		nImage->Destroy();

	CRect rect;
	GetClientRect(&rect);

	int nWidth = rect.Width();
	int nHeight = rect.Height();
	int nBpp = 8;
	nImage->Create(nWidth, -nHeight, nBpp);

	//그레이 스케일 설정
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i; // 그레이 스케일 설정
		}
		nImage->SetColorTable(0, 256, rgb);
	}
	unsigned char* fm = (unsigned char*)nImage->GetBits();
	//그레이 색으로 덮음
	memset(fm, 100, nWidth * nHeight);
}
void CDiaImage::Action(Pos& position)
{
	Clear();
	//데이터 받아와줌
	UpdateData(true);

	static float X = 0;
	static float Y = 0;

	CRect rect;
	GetClientRect(&rect);

	float X1 = position.nX1; // 시작 위치
	float Y1 = position.nY1;

	float targetX = position.nX2; // 목표 위치
	float targetY = position.nY2;

	float centerX = X1;
	float centerY = Y1;

	//밖으로 나가는것 방지
	if (centerX - currentRadius < rect.left || centerX + currentRadius > rect.right ||
		centerY - currentRadius < rect.top || centerY - currentRadius  > rect.bottom) {
		return;
	}

	float dx = targetX - centerX;
	float dy = targetY - centerY;

	float distance = std::sqrt(dx * dx + dy * dy);
	float speed = 5.f;

	if (distance > speed) {
		// 방향 정규화
		float length = std::sqrt(dx * dx + dy * dy);
		float normalX = dx / length;
		float normalY = dy / length;

		// 위치 업데이트
		centerX += normalX * speed;
		centerY += normalY * speed;

		// 갱신된 좌표를 position 구조체에 반영
		position.nX1 = centerX;
		position.nY1 = centerY;
	}
	else {
		//위치 고정
		position.nX1 = targetX;
		position.nY1 = targetY;
	}

	// 원 그리기
	int nPitch = nImage->GetPitch();
	unsigned char* fm = (unsigned char*)nImage->GetBits();
	float radius = currentRadius;

	for (int y = centerY - radius; y < centerY + radius; y++) {
		for (int x = centerX - radius; x < centerX + radius; x++) {
			if (Check(x, y) && isInCircle(x, y, centerX, centerY, radius)) {
				fm[y * nPitch + x] = 0xff; // 픽셀 색상 설정
			}
		}
	}

	nImage->Draw(*nDc, 0, 0);
	Save();
}
void CDiaImage::Save() 
{
	//번호순으로 저장
	static int SaveNum = 0;
	CString fileName;
	fileName.Format(_T("image\\image_%d.bmp"), SaveNum);
	nImage->Save(fileName);
	SaveNum++;
}
void CDiaImage::DrawCenter(CImage* image)
{
	unsigned char* fm = (unsigned char*)image->GetBits();
	int nWidth = image->GetWidth();
	int nHeight = image->GetHeight();
	int nPitch = image->GetPitch();
	int count = 0;
	int sumX = 0, sumY = 0;
	CRect rect(0, 0, nWidth, nHeight);

	//중점 계산
	for (int y = rect.top; y < rect.bottom; y++) {
		for (int x = rect.left; x < rect.right; x++) {
			if (fm[y * nPitch + x] == 0xff) {
				sumX += x;
				sumY += y;
				count++;
			}
		}
	}
	double centerX = 0.f;
	double centerY = 0.f;
	if (count > 0) {
		centerX = (double)sumX / count;
		centerY = (double)sumY / count;

		//X 도트선
		for (int x = 0; x < (int)centerX; x += 2)
		{
			int pixelIndex = (int)centerY * nPitch + x;
			fm[pixelIndex] = 0;
		}
		//Y 도트선
		for (int y = 0; y < (int)centerY; y += 2)
		{
			int pixelIndex = y * nPitch + centerX;
			fm[pixelIndex] = 0;
		}
	}

	// X 그림사이즈
	int xSize = 5;
	CRect xRect(centerX - xSize, centerY - xSize, centerX + xSize, centerY + xSize);

	//위 아래 대각선
	for (int i = xRect.left; i < xRect.right; i++)
	{
		int pixelIndex = (i * nPitch) + i;
		fm[pixelIndex] = 0;
	}

	//아래 위 대각선
	for (int i = xRect.right, j = xRect.left; i >= xRect.left; i--,j++)
	{
		int pixelIndex = (i * nPitch) + j;
		fm[pixelIndex] = 0;
	}

	//이미지 드로우
	nImage->Draw(*nDc, 0, 0);

	// 텍스트 출력
	nDc->SetTextColor(RGB(0, 255, 0));
	nDc->SetBkColor(RGB(0, 0, 0));
	TCHAR buffer[100];
	swprintf(buffer, sizeof(buffer), _T("X: %.1f , Y : %.1f"), centerX, centerY);
	nDc->TextOutW(centerX, centerY, buffer);
}
void CDiaImage::Load() 
{
	//파일 오픈 다이얼로그 생성
	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Image Files (*.bmp; *.jpg;)|*.bmp;*.jpg;|All Files (*.*)|*.*||"));

	//오픈 성공시
	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();

		if (nImage != NULL)
			nImage->Destroy();

		if (nImage->Load(filePath) == S_OK)
		{
			DrawCenter(nImage);
		}
		//이미지 로드 실패시
		else
		{
			AfxMessageBox(_T("파일을 로드할수 없습니다."));
		}
	}
}

bool CDiaImage::Check(float x, float y) 
{
	//클라 영역 체크
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
	CRect rect;
	GetClientRect(&rect);

	//화면 안에 들어가는 최소 반지름 구하기
	float radius = fmin(rand() % 100 + 10,
		fmin(fmin(position.nX1, rect.right - position.nX1),fmin(position.nY1, rect.bottom - position.nY1)));

	//현재 반지름
	currentRadius = radius;
	int nPitch = nImage->GetPitch();
	unsigned char* fm = (unsigned char*)nImage->GetBits();

	for (int y = position.nY1 -radius; y < position.nY1 +radius; y++)
	{
		for (int x = position.nX1 - radius; x < position.nX1 +radius; x++) {
			if (Check(x, y) && isInCircle(x, y, position.nX1, position.nY1, radius)){
				fm[y * nPitch + x] = 0xff;
			}
		}
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
