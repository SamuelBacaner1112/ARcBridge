#if !defined(AFX_DIALOGHDANLENGTH_H__6EB26AFA_2B49_49C6_BA63_5D065465D40E__INCLUDED_)
#define AFX_DIALOGHDANLENGTH_H__6EB26AFA_2B49_49C6_BA63_5D065465D40E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogHDanLength.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogHDanLength dialog

class CDialogHDanLength : public CDialog
{
public:
	void SetARcStructureInputStd(CARcBridgeInputStd *pIn);
private:
	CARcBridgeInputStd *m_pIn;
	CRcBridgeApp *GetRcBridgeApp();
	void SetGridData();
	void SetDataInit();

// Construction
public:
	CDialogHDanLength(CWnd* pParent = NULL);   // standard constructor
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDialogHDanLength)
	enum { IDD = IDD_DIALOG_HDAN_LENGTH };
	double	m_dAng;
	double	m_dSta;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogHDanLength)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogHDanLength)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApply();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGHDANLENGTH_H__6EB26AFA_2B49_49C6_BA63_5D065465D40E__INCLUDED_)
