
// FractalSerpinskiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FractalSerpinski.h"
#include "FractalSerpinskiDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFractalSerpinskiDlg dialog



CFractalSerpinskiDlg::CFractalSerpinskiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFractalSerpinskiDlg::IDD, pParent)
	, m_Red(0)
	, m_Green(0)
	, m_Blue(0)
	, m_Order(0)
	, m_ScaleString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFractalSerpinskiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ID_RED, m_Red);
	DDX_Text(pDX, ID_GREEN, m_Green);
	DDX_Text(pDX, ID_BLUE, m_Blue);
	DDX_Text(pDX, ID_ORDER, m_Order);
	DDX_Text(pDX, ID_SCALE, m_ScaleString);
}

BEGIN_MESSAGE_MAP(CFractalSerpinskiDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_DRAW, &CFractalSerpinskiDlg::OnBnClickedDraw)
END_MESSAGE_MAP()


// CFractalSerpinskiDlg message handlers

BOOL CFractalSerpinskiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	All255 = false;
	IsCreate = false;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFractalSerpinskiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		CClientDC *pdc = new CClientDC(this);

		CRect temp;
		GetClientRect(temp);
		CBrush *pbr;
		CRect r_CL(0, 0, temp.right - 165, temp.bottom);

		CDC memDC;
		if (!memDC.CreateCompatibleDC(pdc))
			return;
		CBitmap* oldBmp;
		CBitmap memBmp;
		if (!memBmp.CreateCompatibleBitmap(pdc, r_CL.Width(), r_CL.Height()))
			return;

		oldBmp = memDC.SelectObject(&memBmp);
		if (All255)
			pbr = new CBrush(RGB(0, 0, 0));
		else
			pbr = new CBrush(RGB(255, 255, 255));
		memDC.SelectObject(pbr);
		memDC.FillRect(r_CL, pbr);

		if (IsCreate)
		{
			CPen MyPen(PS_SOLID, 3, RGB(m_Red, m_Green, m_Blue));
			memDC.SelectObject(MyPen);

			A = m_Scale / 100 * r_CL.bottom;
			/*Коэффициент диагонали кривой Серпинского*/
			Z = CalculateZ(m_Order);
			/*Проекция наклонного отрезка*/
			h = A / (Z + 1);
			/*Находим координаты начальной точки кривой, используя координаты левой верхней точки опорного квадрата*/
			x = (r_CL.bottom / 2 - A / 2); y = (r_CL.right / 2 - A / 2) + h;

			/*Ставим графический курсор в начальную точку*/
			memDC.MoveTo(x, y);
			/*Строим кривую*/
			LineAB(m_Order, &memDC); SegmBC(&memDC); LineCD(m_Order, &memDC);
			SegmDE(&memDC);
			LineEF(m_Order, &memDC); SegmFG(&memDC); LineGH(m_Order, &memDC);
			SegmHA(&memDC);
		}	

		pdc->BitBlt(r_CL.left, r_CL.top, r_CL.Width(), r_CL.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldBmp);
		memBmp.DeleteObject();
		memDC.DeleteDC();

		delete pbr;
		delete pdc;
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFractalSerpinskiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFractalSerpinskiDlg::OnBnClickedDraw()
{
	UpdateData(true);
	if (m_Order == 0)
		MessageBox(_T("Введенно неправильное значение порядка фрактала.\nПопробуйте ещё раз."), _T("Ошибка"), MB_ICONERROR);
	else
		if (m_Order <= 8)
		{
			if (m_ScaleString.IsEmpty())
				MessageBox(_T("Невведенно значение масштаба.\nПопробуйте ещё раз."), _T("Ошибка"), MB_ICONERROR);
			else
			if (CheckData())
			{
				m_Scale = _wtof(m_ScaleString);
				if (m_Scale <= 1000)
				{
					if (m_Red > 255)
						m_Red = 255;
					if (m_Green > 255)
						m_Green = 255;
					if (m_Blue > 255)
						m_Blue = 255;
					if ((m_Red == 255) && (m_Green == 255) && (m_Blue == 255))
						All255 = true;
					else
						All255 = false;
					IsCreate = true;
					Invalidate(1);
					UpdateData(false);
				}
				else
					MessageBox(_T("Введенно слишком большое значение масштаба.\nПопробуйте ещё раз."), _T("Ошибка"), MB_ICONERROR);
			}
			else
				MessageBox(_T("Введенны неправильные данные."), _T("Ошибка"), MB_ICONERROR);
		}
		else
			MessageBox(_T("Введенно слишком большое значение порядка фрактала.\nПопробуйте ещё раз."), _T("Ошибка"), MB_ICONERROR);
}

int CFractalSerpinskiDlg::CalculateZ(int level)
{
	if (level == 0)
		return 1;
	else
		return 2 * CalculateZ(level - 1) + 3;
}
void CFractalSerpinskiDlg::LineAB(int level, CDC *pdc)
{
	if (level > 0)
	{
		LineAB(level - 1, pdc); SegmBC(pdc); LineCD(level - 1, pdc);
		SegmEast(pdc);
		LineGH(level - 1, pdc); SegmHA(pdc); LineAB(level - 1, pdc);
	}
}

void CFractalSerpinskiDlg::LineCD(int level, CDC *pdc)
{
	if (level>0)
	{
		LineCD(level - 1, pdc); SegmDE(pdc); LineEF(level - 1, pdc);
		SegmSouth(pdc);
		LineAB(level - 1, pdc); SegmBC(pdc); LineCD(level - 1, pdc);
	}
}

void CFractalSerpinskiDlg::LineEF(int level, CDC *pdc)
{
	if (level>0)
	{
		LineEF(level - 1, pdc); SegmFG(pdc); LineGH(level - 1, pdc);
		SegmWest(pdc);
		LineCD(level - 1, pdc); SegmDE(pdc); LineEF(level - 1, pdc);
	}
}

void CFractalSerpinskiDlg::LineGH(int level, CDC *pdc)
{
	if (level>0)
	{
		LineGH(level - 1, pdc); SegmHA(pdc); LineAB(level - 1, pdc);
		SegmNord(pdc);
		LineEF(level - 1, pdc); SegmFG(pdc); LineGH(level - 1, pdc);
	}
}

void CFractalSerpinskiDlg::SegmBC(CDC *pdc)
{
	pdc->MoveTo(x, y);
	pdc->LineTo(x + h, y + h);
	x += h;
	y += h;
}

void CFractalSerpinskiDlg::SegmDE(CDC *pdc)
{
	pdc->MoveTo(x, y);
	pdc->LineTo(x - h, y + h);
	x += -h;
	y += h;
}

void CFractalSerpinskiDlg::SegmFG(CDC *pdc)
{
	pdc->MoveTo(x, y);
	pdc->LineTo(x - h, y - h);
	x += -h;
	y += -h;
}

void CFractalSerpinskiDlg::SegmHA(CDC *pdc)
{
	pdc->MoveTo(x, y);
	pdc->LineTo(x + h, y - h);
	x += h;
	y += -h;
}

void CFractalSerpinskiDlg::SegmEast(CDC *pdc)
{
	pdc->MoveTo(x, y);
	pdc->LineTo(x + 2 * h, y + 0);
	x += 2 * h;
	y += 0;
}

void CFractalSerpinskiDlg::SegmSouth(CDC *pdc)
{
	pdc->MoveTo(x, y);
	pdc->LineTo(x + 0, y + 2 * h);
	x += 0;
	y += 2 * h;
}

void CFractalSerpinskiDlg::SegmWest(CDC *pdc)
{
	pdc->MoveTo(x, y);
	pdc->LineTo(x - 2 * h, y + 0);
	y += 0;
	x += -2 * h;
}

void CFractalSerpinskiDlg::SegmNord(CDC *pdc)
{
	pdc->MoveTo(x, y);
	pdc->LineTo(x + 0, y - 2 * h);
	x += 0;
	y += -2 * h;

}

BOOL CFractalSerpinskiDlg::CheckData()
{
	//Проверка каждого введенного символа на разрешенность
	for (int i = 0; i < m_ScaleString.GetLength(); i++)
	{
		//Проверка является-ли символ: 0 1 2 3 4 5 6 7 8 9 .
		//Если нет то ошибка, если да то идем дальше по циклу:
		if ((m_ScaleString.GetAt(i) != '0') && (m_ScaleString.GetAt(i) != '1') && (m_ScaleString.GetAt(i) != '2') &&
			(m_ScaleString.GetAt(i) != '3') && (m_ScaleString.GetAt(i) != '4') && (m_ScaleString.GetAt(i) != '5') &&
			(m_ScaleString.GetAt(i) != '6') && (m_ScaleString.GetAt(i) != '7') && (m_ScaleString.GetAt(i) != '8') &&
			(m_ScaleString.GetAt(i) != '9') && (m_ScaleString.GetAt(i) != '.'))
			return FALSE;
	}
	return TRUE;
}