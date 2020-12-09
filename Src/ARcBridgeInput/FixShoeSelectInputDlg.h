// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_FIXSHOESELECTINPUTDLG_H__C84F2268_C4F0_4DB8_9560_1D8785608CF4__INCLUDED_)
#define AFX_FIXSHOESELECTINPUTDLG_H__C84F2268_C4F0_4DB8_9560_1D8785608CF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FixShoeSelectInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFixShoeSelectInputDlg dialog
class CARcBridgeInputStd;
class CFixShoeSelectInputDlg : public CUserDialog
{
// Construction
public:
	CFixShoeSelectInputDlg(CARcBridgeInputStd* pIn, CWnd* pParent = NULL);   // standard constructor
	
	CARcBridgeInputStd*		m_pIn;
	CStringArray			m_StrShoe;

// Dialog Data
	//{{AFX_DATA(CFixShoeSelectInputDlg)
	enum { IDD = IDD_DIALOG_SELECT_SHOE };
	CGridCtrl	m_Grid;
	CButton	m_btOK;
	CButton	m_btCancel;
	CButton	m_btAllFixFree;
	//}}AFX_DATA

private:
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();

	void SetShoeDirection(long nRow, long nCol);
	long GetShoeDirection(CString str);
	CString GetShoeDirection(long Dir);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFixShoeSelectInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFixShoeSelectInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAllFixFree();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIXSHOESELECTINPUTDLG_H__C84F2268_C4F0_4DB8_9560_1D8785608CF4__INCLUDED_)
