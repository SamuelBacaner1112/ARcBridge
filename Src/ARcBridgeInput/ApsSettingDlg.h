// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_APSSETTINGDLG_H__9F9664A9_CD61_4E59_BC90_6F661D2CF94E__INCLUDED_)
#define AFX_APSSETTINGDLG_H__9F9664A9_CD61_4E59_BC90_6F661D2CF94E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApsSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CApsSettingDlg dialog

class CApsSettingDlg : public CDialog
{
// Construction
public:
	CApsSettingDlg(CWnd* pParent = NULL);   // standard constructor
	CApsSettingDlg(CApsApp *pAps, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CApsSettingDlg)
	enum { IDD = IDD_DIALOG_APSSETTING };
	CXTPComboBox	m_comboNum;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnRecommand;
	CGridCtrlMng	m_Grid;
	CGridCtrlMng	m_GridShear;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApsSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CApsSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRecommond();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetGridData();
	void SetDataSave();
	void SetDataInit();
	CApsApp* m_pAps;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APSSETTINGDLG_H__9F9664A9_CD61_4E59_BC90_6F661D2CF94E__INCLUDED_)
