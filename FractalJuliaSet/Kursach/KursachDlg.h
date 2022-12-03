
// KursachDlg.h : header file
//

#pragma once
#include "afxwin.h"

//Комплексное число:
struct TComplex {
	double x;
	double y;
};

const int iter = 50;
const int max = 16;

// CKursachDlg dialog
class CKursachDlg : public CDialogEx
{
// Construction
public:
	CKursachDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KURSACH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	//Комлексное число z и t, z - основная переменная для расчетов, t - временная   переменная
	TComplex z, t;
	//Координаты x, y и счетчик n
	int n, x, y;
	//Выбранный элемент выпадающего списка, 0 - Красный, 1 - Зеленый, 2 - Синий
	int SelectedList;
	//Максимальные значения координат X и Y
	double maxX, maxY;
	//Cx - вещественная часть комплексного числа C, Cy - мнимая часть.
	double Cx, Cy;
	//Проверка на правильность введенных данных
	BOOL CheckData(CString Data);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_Red;
	int m_Green;
	int m_Blue;
	afx_msg void OnBnClickedDrawfractal();

	afx_msg void OnCbnSelchangeColorlist();
	CComboBox m_ColorList;
	CString m_CRe;
	CString m_CIm;
};
