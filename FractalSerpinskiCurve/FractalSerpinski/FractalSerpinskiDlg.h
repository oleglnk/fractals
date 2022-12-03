
// FractalSerpinskiDlg.h : header file
//

#pragma once


// CFractalSerpinskiDlg dialog
class CFractalSerpinskiDlg : public CDialogEx
{
// Construction
public:
	CFractalSerpinskiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FRACTALSERPINSKI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	bool All255, IsCreate;
	int h, x, y, Z, A;
	float m_Scale;
	int CalculateZ(int level);
	void LineAB(int level, CDC *pdc);
	void LineCD(int level, CDC *pdc);
	void LineEF(int level, CDC *pdc);
	void LineGH(int level, CDC *pdc);
	void SegmBC(CDC *pdc);
	void SegmDE(CDC *pdc);
	void SegmFG(CDC *pdc);
	void SegmHA(CDC *pdc);
	void SegmEast(CDC *pdc);
	void SegmSouth(CDC *pdc);
	void SegmWest(CDC *pdc);
	void SegmNord(CDC *pdc);
	BOOL CheckData();
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_Red;
	int m_Green;
	int m_Blue;
	int m_Order;
	afx_msg void OnBnClickedDraw();
	CString m_ScaleString;
};
