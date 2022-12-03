
// KursachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Kursach.h"
#include "KursachDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKursachDlg dialog



CKursachDlg::CKursachDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKursachDlg::IDD, pParent)
	, m_Red(0)
	, m_Green(0)
	, m_Blue(0)
	, m_CRe(_T(""))
	, m_CIm(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKursachDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ID_RED, m_Red);
	DDX_Text(pDX, ID_GREEN, m_Green);
	DDX_Text(pDX, ID_BLUE, m_Blue);
	DDX_Control(pDX, ID_COLORLIST, m_ColorList);
	DDX_Text(pDX, ID_RECOMPLEX, m_CRe);
	DDX_Text(pDX, ID_IMCOMPLEX, m_CIm);
}

BEGIN_MESSAGE_MAP(CKursachDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_DRAWFRACTAL, &CKursachDlg::OnBnClickedDrawfractal)
	ON_CBN_SELCHANGE(ID_COLORLIST, &CKursachDlg::OnCbnSelchangeColorlist)
END_MESSAGE_MAP()


// CKursachDlg message handlers

BOOL CKursachDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Выставление стандартных данных и обновление полей:
	m_ColorList.InsertString(0, _T("Красный"));
	m_ColorList.InsertString(1, _T("Зеленый"));
	m_ColorList.InsertString(2, _T("Синий"));

	m_ColorList.SelectString(0, _T("Красный"));
	m_Red = 0;
	m_Green = 0;
	m_Blue = 0;
	SelectedList = 0;
	m_CRe = "0";
	m_CIm = "-0.66";
	Cx = _wtof(m_CRe);
	Cy = _wtof(m_CIm);
	
	UpdateData(false);

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

void CKursachDlg::OnPaint()
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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKursachDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//Нажаатие на клавишу прорисовки фрактала:
void CKursachDlg::OnBnClickedDrawfractal()
{
	//Считывание данных из полей:
	UpdateData(true);
	//Проверка на правильность введенных данных:
	if ((CheckData(m_CRe)) && (CheckData(m_CIm)))
	{
		//Считывание вещественного числа C
		Cx = _wtof(m_CRe);
		Cy = _wtof(m_CIm);
		//Если какой-либо из цветов больше 255, ставим значение 255
		if (m_Red > 255)
			m_Red = 255;
		if (m_Green > 255)
			m_Green = 255;
		if (m_Blue > 255)
			m_Blue = 255;

		//Алгоритм прорисовки:
		CClientDC *pdc = new CClientDC(this);

		CRect temp;
		GetClientRect(temp);
		CRect r_CL(0, 0, temp.right - 205, temp.bottom);


		CDC memDC;
		if (!memDC.CreateCompatibleDC(pdc))
			return;
		CBitmap* oldBmp;
		CBitmap memBmp;
		if (!memBmp.CreateCompatibleBitmap(pdc, r_CL.Width(), r_CL.Height()))
			return;

		oldBmp = memDC.SelectObject(&memBmp);

		//Алгоритм рассчета и прорисовки фрактала:
		maxX = r_CL.right / 2;
		maxY = r_CL.bottom / 2;
		for (y = -maxY; y < maxY; y++)
			for (x = -maxX; x < maxX; x++)
			{
				n = 0;
				z.x = x * 0.005;
				z.y = y * 0.005;
				while ((pow(z.x, 2) + pow(z.y, 2) < max) && (n<iter))
				{
					t = z;
					z.x = pow(t.x, 2) - pow(t.y, 2) + Cx;
					z.y = 2 * t.x*t.y + Cy;
					n++;
				}
				if (n < iter)
				{
					//Расчет и выставление цвета:
					int col = n * 6 % 255;
					switch (SelectedList)
					{
					case 0:
						memDC.SetPixel(maxX + x, maxY + y, RGB(col, m_Green, m_Blue));
						break;
					case 1:
						memDC.SetPixel(maxX + x, maxY + y, RGB(m_Red, col, m_Blue));
						break;
					case 2:
						memDC.SetPixel(maxX + x, maxY + y, RGB(m_Red, m_Green, col));
						break;
					}
				}
			}

		pdc->BitBlt(r_CL.left, r_CL.top, r_CL.Width(), r_CL.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldBmp);
		memBmp.DeleteObject();
		memDC.DeleteDC();

		delete pdc;
	}
	else
	{
		MessageBox(_T("Введенны неправильные данные."), _T("Ошибка"), MB_ICONERROR);
	}
	//Обновление полей:
	UpdateData(false);
}

//Выбор цвета из списка:
void CKursachDlg::OnCbnSelchangeColorlist()
{
	switch (m_ColorList.GetCurSel())
	{
	case 0:
		GetDlgItem(ID_RED)->EnableWindow(0);
		GetDlgItem(ID_GREEN)->EnableWindow(1);
		GetDlgItem(ID_BLUE)->EnableWindow(1);
		m_Red = 0;
		SelectedList = 0;
		break;
	case 1:
		GetDlgItem(ID_RED)->EnableWindow(1);
		GetDlgItem(ID_GREEN)->EnableWindow(0);
		GetDlgItem(ID_BLUE)->EnableWindow(1);
		m_Green = 0;
		SelectedList = 1;
		break;
	case 2:
		GetDlgItem(ID_RED)->EnableWindow(1);
		GetDlgItem(ID_GREEN)->EnableWindow(1);
		GetDlgItem(ID_BLUE)->EnableWindow(0);
		m_Blue = 0;
		SelectedList = 2;
		break;
	}
	UpdateData(false);
}

//Алгоритм проверки введенных данных:
BOOL CKursachDlg::CheckData(CString Data)
{
	//Проверка каждого введенного символа на разрешенность
	for (int i = 0; i < Data.GetLength(); i++)
	{
		//Проверка является-ли символ: 0 1 2 3 4 5 6 7 8 9 - .
		//Если нет то ошибка, если да то идем дальше по циклу:
		if ((Data.GetAt(i) != '0') && (Data.GetAt(i) != '1') && (Data.GetAt(i) != '2') &&
			(Data.GetAt(i) != '3') && (Data.GetAt(i) != '4') && (Data.GetAt(i) != '5') &&
			(Data.GetAt(i) != '6') && (Data.GetAt(i) != '7') && (Data.GetAt(i) != '8') &&
			(Data.GetAt(i) != '9') && (Data.GetAt(i) != '-') && (Data.GetAt(i) != '.'))
			return FALSE;
	}
	return TRUE;
}