
// KursachDlg.h : header file
//

#pragma once
#include "afxwin.h"

//����������� �����:
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

	//���������� ����� z � t, z - �������� ���������� ��� ��������, t - ���������   ����������
	TComplex z, t;
	//���������� x, y � ������� n
	int n, x, y;
	//��������� ������� ����������� ������, 0 - �������, 1 - �������, 2 - �����
	int SelectedList;
	//������������ �������� ��������� X � Y
	double maxX, maxY;
	//Cx - ������������ ����� ������������ ����� C, Cy - ������ �����.
	double Cx, Cy;
	//�������� �� ������������ ��������� ������
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
